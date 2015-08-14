// gameswf_avm2_jit.cpp	-- Julien Hamaide <julien.hamaide@gmail.com>	2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// AVM2 JIT-compiler implementation

#include "gameswf_avm2.h"
#include "gameswf_stream.h"
#include "gameswf_log.h"
#include "gameswf_abc.h"
#include "gameswf_disasm.h"
#include "gameswf_character.h"
#include "gameswf_jit.h"

namespace gameswf
{

	template< typename T >
	void stack_push_back_value( swf_array<as_value> & stack, T value )
	{
		stack.push_back( value );
	}

	template< typename T >
	void stack_push_back_ref( swf_array<as_value> & stack, T & value )
	{
		stack.push_back( value );
	}

	typedef void ( *stack_int_function )( swf_array<as_value> & stack, int value );
	typedef void ( *stack_charp_function )( swf_array<as_value> & stack, char * value );
	typedef void (swf_array<as_value>::*push_value_function )( as_value & );
	typedef as_value & (swf_array<as_value>::*index_function)(int);

	template<typename T> struct constructor_helper
	{
		static void construct( void * object )
		{
			new( object ) T;
		}

		static void destruct( T * object )
		{
			delete object;
		}
	};


	struct callpropvoid
	{
		static void call( const as_object *_this, const char* name, int arg_count, swf_array<as_value> & stack )
		{
			as_environment env(_this->get_player());
			for (int i = 0; i < arg_count; i++)
			{
				as_value& val = stack[stack.size() - 1 - i];
				env.push(val);
			}
			stack.resize(stack.size() - arg_count);

			as_object* obj = stack.back().to_object();
			stack.resize(stack.size() - 1);

			as_value func;
			if (obj && obj->get_member(name, &func))
			{
				call_method(func, &env, obj,	arg_count, env.get_top_index());
			}

			IF_VERBOSE_ACTION(log_msg("EX: callpropvoid\t 0x%p.%s(args:%d)\n", obj, name, arg_count));

		}
	};

	void as_3_function::compile()
	{
#define var_stack jit_getarg( 1 )
#define var_scope jit_getarg( 2 )

#ifdef __GAMESWF_ENABLE_JIT__
		int ip = 0;
		jit_prologue( m_compiled_code );

		if( m_code.size() == 0 ) return;
		do
		{
			Uint8 opcode = m_code[ip++];
			switch (opcode)
			{
				case 0x24:	// pushbyte
				{
					int byte_value;
					ip += read_vu30(byte_value, &m_code[ip]);

					//stack.push_back(byte_value);
					jit_pushargi( m_compiled_code, byte_value );
					jit_load( m_compiled_code, jit_eax, jit_getarg( 1 ) );
					jit_pusharg( m_compiled_code, jit_eax );
					jit_call( m_compiled_code, stack_int_function(stack_push_back_value) );
					jit_popargs( m_compiled_code, 2 );

					//IF_VERBOSE_ACTION(log_msg("EX: pushbyte\t %d\n", byte_value));

					break;
				}

				case 0x2D:	// pushint
				{
					int index;
					ip += read_vu30(index, &m_code[ip]);
					int val = m_abc->get_integer(index);

					//stack.push_back(val);
					jit_pushargi( m_compiled_code, val );
					jit_load( m_compiled_code, jit_eax, jit_getarg( 1 ) );
					jit_pusharg( m_compiled_code, jit_eax );
					jit_call( m_compiled_code, stack_int_function(stack_push_back_value) );
					jit_popargs( m_compiled_code, 2 );

					//IF_VERBOSE_ACTION(log_msg("EX: pushint\t %d\n", val));

					break;
				}

				case 0x2C:	// pushstring
				{
					int index;
					ip += read_vu30(index, &m_code[ip]);
					const char* val = m_abc->get_string(index);

					//stack.push_back(val);
					jit_pushargi( m_compiled_code, val );
					jit_load( m_compiled_code, jit_eax, jit_getarg( 1 ) );
					jit_pusharg( m_compiled_code, jit_eax );
					jit_call( m_compiled_code, stack_charp_function( &stack_push_back_value ) );
					jit_popargs( m_compiled_code, 2 );

					//IF_VERBOSE_ACTION(log_msg("EX: pushstring\t '%s'\n", val));

					break;
				}

				case 0x2F:	// pushdouble
				{
					int index;
					ip += read_vu30(index, &m_code[ip]);
					double val = m_abc->get_double(index);
					
					jit_pushargi( m_compiled_code, val );
					jit_load( m_compiled_code, jit_esi, var_stack );
					jit_pusharg( m_compiled_code, jit_esi );
					jit_call( m_compiled_code, &(stack_push_back_value<double>) );
					jit_popargs( m_compiled_code, 3 ); //double counts for 2;
					//stack.push_back(val);

					//IF_VERBOSE_ACTION(log_msg("EX: pushdouble\t %f\n", val));

					break;
				}

				case 0x30:	// pushscope
				{
					//as_value& val = stack.back();
					typedef as_value& ( swf_array<as_value>::*back_function )();
					jit_load( m_compiled_code, jit_this_pointer, jit_getarg( 1 ) );
					jit_this_call( m_compiled_code, (back_function)&swf_array<as_value>::back );

					jit_load( m_compiled_code, jit_this_pointer, var_scope );
					jit_push( m_compiled_code, jit_result );
					jit_call( m_compiled_code, (push_value_function)&swf_array<as_value>::push_back );
					jit_popargs( m_compiled_code, 1 );


					//IF_VERBOSE_ACTION(log_msg("EX: pushscope\t %s\n", val.to_xstring()));
					//TODO: Ecx is caller-saved, copy it to esi or edi before calling to prevent reload from memory
					//stack.resize(stack.size() - 1); 
					compile_stack_resize( 1 );

					break;
				}

				case 0x47:	// returnvoid
				{
					//IF_VERBOSE_ACTION(log_msg("EX: returnvoid\t\n"));

					//result->set_undefined();
					jit_load( m_compiled_code, jit_this_pointer, jit_getarg( 3 ) );
					jit_this_call( m_compiled_code, &as_value::set_undefined );

					break;
				}

				case 0x49:	// constructsuper
				{
					// stack: object, arg1, arg2, ..., argn
					int arg_count;
					ip += read_vu30(arg_count, &m_code[ip]);

					struct constructsuper
					{
					
						static void call( swf_array<as_value> & stack, int arg_count )
						{
							as_object* obj = stack.back().to_object();
							stack.resize(stack.size() - 1);
							for (int i = 0; i < arg_count; i++)
							{
								as_value& val = stack.back();
								stack.resize(stack.size() - 1);
							}

							//TODO: construct super of obj

							IF_VERBOSE_ACTION(log_msg("EX: constructsuper\t 0x%p(args:%d)\n", obj, arg_count));
						}
					};

					jit_pushi( m_compiled_code, arg_count );
					jit_load( m_compiled_code, jit_esi, var_stack );
					jit_push( m_compiled_code, jit_esi );
					jit_call( m_compiled_code, &constructsuper::call );
					jit_popargs( m_compiled_code, 2 );

					break;
				}

				case 0x4F:	// callpropvoid, Call a property, discarding the return value.
				// Stack: …, obj, [ns], [name], arg1,...,argn => …
				{
					int index;
					ip += read_vu30(index, &m_code[ip]);
					const char* name = m_abc->get_multiname(index);

					int arg_count;
					ip += read_vu30(arg_count, &m_code[ip]);

					jit_load( m_compiled_code, jit_esi, var_stack );
					jit_pusharg( m_compiled_code, jit_esi );
					jit_pushargi( m_compiled_code, arg_count );
					jit_pushargi( m_compiled_code, name );
					jit_pushargi( m_compiled_code, this );
					jit_call( m_compiled_code, &callpropvoid::call );
					jit_popargs( m_compiled_code, 4 );

					/*
					struct local
					{
						static void construct_env( as_environment *env, player * player )
						{
							new( env ) as_environment( player );
						}

						static void destruct_env( as_environment *env )
						{
							env->~as_environment();
						}
					};

					//as_environment env(get_player());
					int env_offset = jit_allocate_stack_object_memory( m_compiled_code, sizeof( as_environment ) );
					//Do we query it each it or it won't change? I decided it won't change
					jit_mov( m_compiled_code, jit_edi, jit_stack_pointer );
					jit_pushi( m_compiled_code, (uint32)get_player() );
					jit_push( m_compiled_code, jit_edi );
					jit_call( m_compiled_code, &local::construct_env );
					jit_popargs( m_compiled_code, 2 );

					//as_value* val = stack.back();
					jit_load( m_compiled_code, jit_this_pointer, jit_getarg( 1 ) );
					typedef as_value& ( swf_array<as_value>::*back_function )();
					jit_this_call( m_compiled_code, (back_function)&swf_array<as_value>::back );
					jit_mov( m_compiled_code, jit_esi, jit_result );

					for (int i = 0; i < arg_count; i++)
					{
					 //env.push( *val);
					 jit_mov( m_compiled_code, jit_this_pointer, jit_edi );
					 jit_push( m_compiled_code, jit_esi );
					 jit_this_call( m_compiled_code, & as_environment::push<as_value&> );
					 
					 //val--;
					 jit_subi( m_compiled_code, jit_esi, sizeof( as_value ) );
					}

					//stack.resize(stack.size() - arg_count);
					compile_stack_resize( arg_count );

					//as_object* obj = stack.back().to_object();
					jit_load( m_compiled_code, jit_this_pointer, jit_getarg( 1 ) );
					typedef as_value& ( swf_array<as_value>::*back_function )();
					jit_this_call( m_compiled_code, (back_function)&swf_array<as_value>::back );
					jit_mov( m_compiled_code, jit_this_pointer, jit_result );
					jit_this_call( m_compiled_code, &as_value::to_object );
					jit_mov( m_compiled_code, jit_esi, jit_result );

					 //stack.resize(stack.size() - 1);
					compile_stack_resize( 1 );


					//as_value func;
					//if (obj && obj->get_member(name, &func))
					//{
					//	call_method(func, &env, obj,	arg_count, env.get_top_index());
					//}

					// Destruct env
					jit_getaddress( m_compiled_code, jit_result, jit_stack_pointer, env_offset );
					jit_push( m_compiled_code ,jit_result );
					jit_call( m_compiled_code, &local::destruct_env );
					jit_popargs( m_compiled_code, 1 );

 
					//IF_VERBOSE_ACTION(log_msg("EX: callpropvoid\t 0x%p.%s(args:%d)\n", obj, name, arg_count));
					*/
					break;
				}

				case 0x5D:	// findpropstrict
				{
					int index;
					ip += read_vu30(index, &m_code[ip]);
					const char* name = m_abc->get_multiname(index);

					// search property in scope
					
					// TODO: inline
					struct findpropstrict
					{
						static as_object * call( char* name, swf_array<as_value> & scope )
						{
							for (int i = scope.size() - 1; i >= 0; i--)
							{
								as_value val;
								if (scope[i].find_property(name, &val))
								{
									return scope[i].to_object();
								}
							}

							return NULL;
						}
					};

					jit_load( m_compiled_code, jit_esi, var_scope );
					jit_push( m_compiled_code, jit_esi );
					jit_pushi( m_compiled_code, name);
					jit_call( m_compiled_code, &findpropstrict::call );
					jit_popargs( m_compiled_code, 2 );

					//IF_VERBOSE_ACTION(log_msg("EX: findpropstrict\t %s, obj=0x%p\n", name, obj));

					//stack.push_back(obj);
					jit_load( m_compiled_code, jit_esi, var_stack );
					jit_push( m_compiled_code, jit_result );
					jit_push( m_compiled_code, jit_esi );
					jit_call( m_compiled_code, &stack_push_back_value<as_object*> );
					jit_popargs( m_compiled_code, 2 );

					break;
				}

				case 0x5E:	// findproperty, Search the scope stack for a property
				{
					int index;
					ip += read_vu30(index, &m_code[ip]);
					const char* name = m_abc->get_multiname(index);

					struct findproperty
					{
						static void call( const char* name, swf_array<as_value> & scope, swf_array<as_value> & stack )
						{
							as_object* obj = NULL;
							for (int i = scope.size() - 1; i >= 0; i--)
							{
								as_value val;
								if (scope[i].find_property(name, &val))
								{
									obj = scope[i].to_object();
									break;
								}
							}

							IF_VERBOSE_ACTION(log_msg("EX: findproperty\t %s, obj=0x%p\n", name, obj));

							stack.push_back(obj);
						}
					};

					jit_load( m_compiled_code, jit_esi, var_stack );
					jit_push( m_compiled_code, jit_esi );
					jit_load( m_compiled_code, jit_esi, var_scope );
					jit_push( m_compiled_code, jit_esi );
					jit_pushi( m_compiled_code, name );
					jit_call( m_compiled_code, &findproperty::call );
					jit_popargs( m_compiled_code, 3 );

					break;

				}

				case 0x60:	// getlex, Find and get a property.
				{
					int index;
					ip += read_vu30(index, &m_code[ip]);
					const char* name = m_abc->get_multiname(index);

					struct getlex
					{
						static void call( const char* name, swf_array<as_value> & scope, swf_array<as_value> &stack )
						{
							// search property in scope
							as_value val;
							for (int i = scope.size() - 1; i >= 0; i--)
							{
								if (scope[i].find_property(name, &val))
								{
									break;
								}
							}

							IF_VERBOSE_ACTION(log_msg("EX: getlex\t %s, value=%s\n", name, val.to_xstring()));

							stack.push_back(val);
						}
					};

					jit_load( m_compiled_code, jit_esi, var_stack );
					jit_push( m_compiled_code, jit_esi );
					jit_load( m_compiled_code, jit_esi, var_scope );
					jit_push( m_compiled_code, jit_esi );
					jit_pushi( m_compiled_code, name );
					jit_call( m_compiled_code, &getlex::call );
					jit_popargs( m_compiled_code, 2 );
					break;
				}

				case 0x66:	// getproperty
				{
					//					 int index;
					//					 ip += read_vu30(index, &m_code[ip]);
					//					 const char* name = m_abc->get_multiname(index);
					// 
					//					 as_object* obj = stack.back().to_object();
					//					 if (obj)
					//					 {
					//						 obj->get_member(name, &stack.back());
					//					 }
					//					 else
					//					 {
					//						 stack.back().set_undefined();
					//					 }
					// 
					//					 IF_VERBOSE_ACTION(log_msg("EX: getproperty\t %s, value=%s\n", name, stack.back().to_xstring()));
					// 
					break;
				}

				case 0x68:	// initproperty, Initialize a property.
				{
					int index;
					ip += read_vu30(index, &m_code[ip]);
					const char* name = m_abc->get_multiname(index);

					struct initproperty
					{
						static void call( const char * name, swf_array<as_value> & stack )
						{
							as_value& val = stack[stack.size() - 1];
							as_object* obj = stack[stack.size() - 2].to_object();
							if (obj)
							{
								obj->set_member(name, val);
							}

							IF_VERBOSE_ACTION(log_msg("EX: initproperty\t 0x%p.%s=%s\n", obj, name, val.to_xstring()));

							stack.resize(stack.size() - 2);
						}
					};

					jit_load( m_compiled_code, jit_esi, var_stack );
					jit_push( m_compiled_code, jit_esi );
					jit_pushi( m_compiled_code, name );
					jit_call( m_compiled_code, &initproperty::call );
					jit_popargs( m_compiled_code, 2 );
					break;
				}

				case 0xD0:	// getlocal_0
				case 0xD1:	// getlocal_1
				case 0xD2:	// getlocal_2
				case 0xD3:	// getlocal_3
				{
					//as_value& val = lregister[opcode & 0x03];
					jit_load( m_compiled_code, jit_this_pointer, jit_getarg( 0 ) );
					jit_pushi( m_compiled_code, opcode & 0x03 );
					jit_this_call( m_compiled_code, (index_function)&swf_array<as_value>::operator[] );
					jit_popargs( m_compiled_code, 1 );

					//stack.push_back(val);
					jit_push( m_compiled_code, jit_result );
					jit_load( m_compiled_code, jit_this_pointer, jit_getarg( 1 ) );
					jit_this_call( m_compiled_code, (push_value_function) &swf_array<as_value>::push_back );
					jit_popargs( m_compiled_code, 1 );

					//IF_VERBOSE_ACTION(log_msg("EX: getlocal_%d\t %s\n", opcode & 0x03, val.to_xstring()));
					break;
				}

				default:
					log_msg("TODO opcode 0x%02X\n", opcode);
					break;
			}

		}
		while (ip < m_code.size());

		jit_return( m_compiled_code );
		m_compiled_code.initialize();
#else
		assert( false );
#endif
	}

	void as_3_function::compile_stack_resize( int count )
	{
#ifdef __GAMESWF_ENABLE_JIT__
		jit_load( m_compiled_code, jit_this_pointer, jit_getarg( 1 ) );
		jit_this_call( m_compiled_code, &swf_array<as_value>::size );
		jit_subi( m_compiled_code, jit_eax, count );
		jit_pusharg( m_compiled_code, jit_eax );
		jit_load( m_compiled_code, jit_this_pointer, jit_getarg( 1 ) );
		jit_this_call( m_compiled_code,  &swf_array<as_value>::resize );
		jit_popargs( m_compiled_code, 1 );
#endif
	}
}
