// gameswf_disasm.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

//
// Disassembler
//

#include "gameswf_disasm.h"
#include "gameswf_log.h"
#include <stdarg.h>

namespace gameswf
{

#define COMPILE_DISASM 1

#ifndef COMPILE_DISASM

	void	log_disasm(const unsigned char* instruction_data)
	// No disassembler in this version...
	{
		log_msg("<no disasm>\n");
	}

#else // COMPILE_DISASM

	//
	// Action Script 3.0
	//

	enum arg_format_avm2
	{
		ARG_END=0,
		ARG_MULTINAME,
		ARG_NAMESPACE,
		ARG_BYTE,
		ARG_SHORT,
		ARG_INT,
		ARG_UINT,
		ARG_DOUBLE,
		ARG_STRING,
		ARG_COUNT,
		ARG_CLASSINFO,
		ARG_FUNCTION,
		ARG_EXCEPTION,
		ARG_REGISTER,
		ARG_SLOTINDEX,
		ARG_OFFSET,
		ARG_OFFSETLIST,
	};

	int read_vu30( int& result, const uint8 *args )
	{
		result = args[0];

		if ((result & 0x00000080) == 0)
		{
			return 1;
		}

		result = (result & 0x0000007F) | args[1] << 7;
		if ((result & 0x00004000) == 0)
		{
			return 2;
		}

		result = (result & 0x00003FFF) | args[2] << 14;
		if ((result & 0x00200000) == 0)
		{
			return 3;
		}

		result = (result & 0x001FFFFF) | args[3] << 21;
		if ((result & 0x10000000) == 0)
		{
			return 4;
		}

		result = (result & 0x0FFFFFFF) | args[4] << 28;
		return 5;
	}

	struct inst_info_avm2
	{
		const char*	m_instruction;
		swf_array<arg_format_avm2> m_arg_formats;

		inst_info_avm2(const char* opname) :
			m_instruction(opname)
		{
		}

		inst_info_avm2(const char* opname, const arg_format_avm2 arg_format_0, ...) :
			m_instruction(opname)
		{
			va_list arg;

			m_arg_formats.push_back( arg_format_0 );
			va_start( arg, arg_format_0 );

			arg_format_avm2 current_format;
			while( ( current_format = (arg_format_avm2)va_arg( arg, int ) ) != ARG_END )
			{
				m_arg_formats.push_back( current_format );
			}
		}

		int process(const abc_def* def, const uint8 *args)
		{
			int byte_count = 1;
			for( int i=0; i<m_arg_formats.size();++i)
			{
				int value;
				switch( m_arg_formats[i] )
				{
				case ARG_MULTINAME:
					byte_count += read_vu30(value, &args[byte_count]);
					if( value >= def->m_multiname.size() )
					{
						log_msg( "\t\tmultiname: runtime %i\n", value);
					}
					else
					{
						switch( def->m_multiname[value].m_kind )
						{
							case multiname::CONSTANT_Multiname:
							case multiname::CONSTANT_QName:
								log_msg( "\t\tmultiname: %s\n", def->m_string[def->m_multiname[value].m_name].c_str());
								break;

							default:
								log_msg( "\t\tmultiname ( todo kind: %i )\n", def->m_multiname[value].m_kind );
						};
					}
					break;

				case ARG_NAMESPACE:
					byte_count += read_vu30(value, &args[byte_count]);
					log_msg( "\t\tnamespace: %s\n", def->m_string[ def->m_namespace[value].m_name ].c_str());
					break;

				case ARG_BYTE:
					value = args[byte_count];
					log_msg("\t\tvalue: %i\n", value);
					byte_count++;
					break;

				case ARG_SHORT:
					byte_count += read_vu30(value, &args[byte_count]);
					log_msg("\t\tvalue: %i\n", value);
					break;

				case ARG_INT:
					byte_count += read_vu30(value, &args[byte_count]);
					log_msg("\t\tvalue: %i\n", def->m_integer[value]);
					break;

				case ARG_UINT:
					byte_count += read_vu30(value, &args[byte_count]);
					log_msg("\t\tvalue: %ui\n", def->m_uinteger[value]);
					break;

				case ARG_DOUBLE:
					byte_count += read_vu30(value, &args[byte_count]);
					log_msg("\t\tvalue: %f\n", def->m_double[value]);
					break;

				case ARG_STRING:
					byte_count += read_vu30(value, &args[byte_count]);
					log_msg( "\t\tstring: %s\n", def->m_string[value].c_str());
					break;

				case ARG_COUNT:
					byte_count += read_vu30(value, &args[byte_count]);
					log_msg( "\t\tcount: %i\n", value);
					break;

				case ARG_CLASSINFO:
					byte_count += read_vu30(value, &args[byte_count]);
					log_msg( "\t\tclass: %i\n", value);
					break;

				case ARG_FUNCTION:
					byte_count += read_vu30( value, &args[byte_count] );
					//TODO: print signature
					log_msg( "\t\tfunction: %s\n", def->m_string[def->m_method[value]->m_name].c_str());
					break;

				case ARG_EXCEPTION:
					byte_count += read_vu30( value, &args[byte_count] );
					//TODO: print exception info
					log_msg( "\t\texception: %i\n", value);
					break;

				case ARG_REGISTER:
					byte_count += read_vu30( value, &args[byte_count] );
					log_msg( "\t\tregister: %i\n", value);
					break;

				case ARG_SLOTINDEX:
					byte_count += read_vu30( value, &args[byte_count] );
					log_msg( "\t\tslot index: %i\n", value);
					break;

				case ARG_OFFSET:
					value = args[byte_count] | args[byte_count+1]<<8 | args[byte_count+2]<<16;
					byte_count += 3;
					log_msg( "\t\toffset: %i\n", value);
					break;

				case ARG_OFFSETLIST:
					value = args[byte_count] | args[byte_count+1]<<8 | (*(int8*)&args[byte_count+2])<<16; //sign extend the high byte
					log_msg( "\t\tdefault offset: %i\n", value);
					byte_count += 3;

					int offset_count;
					byte_count += read_vu30( offset_count, &args[byte_count] );

					for(int i=0;i<offset_count+1;i++)
					{
						value = args[byte_count] | args[byte_count+1]<<8 | (*(int8*)&args[byte_count+2])<<16;
						log_msg("\t\toffset %i: %i\n", i, value);
						byte_count +=3;
					}
					break;

				case ARG_END:
					assert(0 && "forbidden");
					break;
				}
			}

			return byte_count;
		}

		bool has_argument() const { return m_arg_formats.size() != 0;}
	};

	static gameswf::hash<int, inst_info_avm2> s_instr;

	// it's called on exit from player
	void clear_disasm()
	{
		s_instr.clear();
	}

	void	log_disasm_avm2(const membuf& data, const abc_def* def)
	// Disassemble one instruction to the log, AVM2
	{
		if (s_instr.size() == 0)
		{
			s_instr.add(0x03, inst_info_avm2("throw"));
			s_instr.add(0x04, inst_info_avm2("getsuper", ARG_MULTINAME, ARG_END ));
			s_instr.add(0x05, inst_info_avm2("setsuper" ));
			s_instr.add(0x06, inst_info_avm2("dxns"));
			s_instr.add(0x07, inst_info_avm2("dxnslate"));
			s_instr.add(0x08, inst_info_avm2("kill", ARG_REGISTER, ARG_END));
			s_instr.add(0x09, inst_info_avm2("label"));
			// no inst for 0x0A, 0x0B
			s_instr.add(0x0C, inst_info_avm2("ifnlt", ARG_OFFSET, ARG_END));
			s_instr.add(0x0D, inst_info_avm2("ifnle", ARG_OFFSET, ARG_END));
			s_instr.add(0x0E, inst_info_avm2("ifngt", ARG_OFFSET, ARG_END));
			s_instr.add(0x0F, inst_info_avm2("ifnge", ARG_OFFSET, ARG_END));
			s_instr.add(0x10, inst_info_avm2("jump", ARG_OFFSET, ARG_END));
			s_instr.add(0x11, inst_info_avm2("iftrue", ARG_OFFSET, ARG_END));
			s_instr.add(0x12, inst_info_avm2("iffalse", ARG_OFFSET, ARG_END));
			s_instr.add(0x13, inst_info_avm2("ifeq", ARG_OFFSET, ARG_END));
			s_instr.add(0x14, inst_info_avm2("ifne", ARG_OFFSET, ARG_END));
			s_instr.add(0x15, inst_info_avm2("iflt", ARG_OFFSET, ARG_END));
			s_instr.add(0x16, inst_info_avm2("ifle", ARG_OFFSET, ARG_END));
			s_instr.add(0x17, inst_info_avm2("ifgt", ARG_OFFSET, ARG_END));
			s_instr.add(0x18, inst_info_avm2("ifge", ARG_OFFSET, ARG_END));
			s_instr.add(0x19, inst_info_avm2("ifstricteq", ARG_OFFSET, ARG_END));
			s_instr.add(0x1A, inst_info_avm2("ifstrictne", ARG_OFFSET, ARG_END));
			s_instr.add(0x1B, inst_info_avm2("lookupswitch", ARG_OFFSETLIST, ARG_END));
			s_instr.add(0x1C, inst_info_avm2("pushwith"));
			s_instr.add(0x1D, inst_info_avm2("popscope"));
			s_instr.add(0x1E, inst_info_avm2("nextname"));
			s_instr.add(0x1F, inst_info_avm2("hasnext"));
			s_instr.add(0x20, inst_info_avm2("pushnull"));
			s_instr.add(0x21, inst_info_avm2("pushundefined"));
			// no inst for 0x22
			s_instr.add(0x23, inst_info_avm2("nextvalue"));
			s_instr.add(0x24, inst_info_avm2("pushbyte", ARG_BYTE, ARG_END));
			s_instr.add(0x25, inst_info_avm2("pushshort", ARG_SHORT, ARG_END));
			s_instr.add(0x26, inst_info_avm2("pushtrue"));
			s_instr.add(0x27, inst_info_avm2("pushfalse"));
			s_instr.add(0x28, inst_info_avm2("pushnan"));
			s_instr.add(0x29, inst_info_avm2("pop"));
			s_instr.add(0x2A, inst_info_avm2("dup"));
			s_instr.add(0x2B, inst_info_avm2("swap"));
			s_instr.add(0x2C, inst_info_avm2("pushstring", ARG_STRING, ARG_END));
			s_instr.add(0x2D, inst_info_avm2("pushint", ARG_INT, ARG_END));
			s_instr.add(0x2E, inst_info_avm2("pushuint", ARG_UINT, ARG_END));
			s_instr.add(0x2F, inst_info_avm2("pushdouble", ARG_DOUBLE, ARG_END));
			s_instr.add(0x30, inst_info_avm2("pushscope"));
			s_instr.add(0x31, inst_info_avm2("pushnamespace", ARG_NAMESPACE, ARG_END));
			s_instr.add(0x32, inst_info_avm2("hasnext2", ARG_REGISTER, ARG_REGISTER, ARG_END));
			// no inst for 0x33 -> 0X3F
			s_instr.add(0x40, inst_info_avm2("newfunction", ARG_FUNCTION, ARG_END));
			s_instr.add(0x41, inst_info_avm2("call", ARG_COUNT, ARG_END));
			s_instr.add(0x42, inst_info_avm2("construct", ARG_COUNT, ARG_END));
			s_instr.add(0x43, inst_info_avm2("callmethod", ARG_SHORT, ARG_COUNT, ARG_END));
			s_instr.add(0x44, inst_info_avm2("callstatic", ARG_FUNCTION, ARG_COUNT, ARG_END));
			s_instr.add(0x45, inst_info_avm2("callsuper", ARG_MULTINAME, ARG_COUNT, ARG_END));
			s_instr.add(0x46, inst_info_avm2("callproperty", ARG_MULTINAME, ARG_COUNT, ARG_END));
			s_instr.add(0x47, inst_info_avm2("returnvoid"));
			s_instr.add(0x48, inst_info_avm2("returnvalue"));
			s_instr.add(0x49, inst_info_avm2("constructsuper", ARG_COUNT, ARG_END));
			s_instr.add(0x4A, inst_info_avm2("constructprop", ARG_MULTINAME, ARG_COUNT, ARG_END));
			// no inst for 0x4B
			s_instr.add(0x4C, inst_info_avm2("callproplex", ARG_MULTINAME, ARG_COUNT, ARG_END));
			// no inst for 0x4E
			s_instr.add(0x4E, inst_info_avm2("callsupervoid", ARG_MULTINAME, ARG_COUNT, ARG_END));
			s_instr.add(0x4F, inst_info_avm2("callpropvoid", ARG_MULTINAME, ARG_COUNT, ARG_END));
			// no inst for 0x50 -> 0x54
			s_instr.add(0x55, inst_info_avm2("newobject", ARG_COUNT, ARG_END));
			s_instr.add(0x56, inst_info_avm2("newarray", ARG_COUNT, ARG_END));
			s_instr.add(0x57, inst_info_avm2("newactivation"));
			s_instr.add(0x58, inst_info_avm2("newclass", ARG_CLASSINFO, ARG_END));
			s_instr.add(0x59, inst_info_avm2("getdescendants", ARG_MULTINAME, ARG_END));
			s_instr.add(0x5A, inst_info_avm2("newcatch", ARG_EXCEPTION, ARG_END));
			// no inst for 0x5B -> 0x5C
			s_instr.add(0x5D, inst_info_avm2("findpropstrict", ARG_MULTINAME, ARG_END));
			s_instr.add(0x5E, inst_info_avm2("findproperty", ARG_MULTINAME, ARG_END));
			// no inst for 0x5F
			s_instr.add(0x60, inst_info_avm2("getlex", ARG_MULTINAME, ARG_END));
			s_instr.add(0x61, inst_info_avm2("setproperty", ARG_MULTINAME, ARG_END));
			s_instr.add(0x62, inst_info_avm2("getlocal", ARG_REGISTER, ARG_END));
			s_instr.add(0x63, inst_info_avm2("setlocal", ARG_REGISTER, ARG_END));
			s_instr.add(0x64, inst_info_avm2("getglobalscope"));
			s_instr.add(0x65, inst_info_avm2("getscopeobject", ARG_BYTE, ARG_END));
			s_instr.add(0x66, inst_info_avm2("getproperty", ARG_MULTINAME, ARG_END));
			// no inst for 0x67
			s_instr.add(0x68, inst_info_avm2("initproperty", ARG_MULTINAME, ARG_END));
			// no inst for 0x69
			s_instr.add(0x6A, inst_info_avm2("deleteproperty", ARG_MULTINAME, ARG_END));
			// no inst for 0x6B
			s_instr.add(0x6C, inst_info_avm2("getslot", ARG_SLOTINDEX, ARG_END));
			s_instr.add(0x6D, inst_info_avm2("setslot", ARG_SLOTINDEX, ARG_END));
			s_instr.add(0x6E, inst_info_avm2("getglobalslot", ARG_SLOTINDEX, ARG_END));
			s_instr.add(0x6F, inst_info_avm2("setglobalslot", ARG_SLOTINDEX, ARG_END));
			s_instr.add(0x70, inst_info_avm2("convert_s"));
			s_instr.add(0x71, inst_info_avm2("esc_xelem"));
			s_instr.add(0x72, inst_info_avm2("esc_xattr"));
			s_instr.add(0x73, inst_info_avm2("convert_i"));
			s_instr.add(0x74, inst_info_avm2("convert_u"));
			s_instr.add(0x75, inst_info_avm2("convert_d"));
			s_instr.add(0x76, inst_info_avm2("convert_b"));
			s_instr.add(0x77, inst_info_avm2("convert_o"));
			s_instr.add(0x78, inst_info_avm2("checkfilter"));
			// no inst for 0x79 - 0x7F
			s_instr.add(0x80, inst_info_avm2("coerce", ARG_MULTINAME, ARG_END));
			// no inst for 0x81
			s_instr.add(0x82, inst_info_avm2("coerce_a"));
			// no inst for 0x83, 0x84
			s_instr.add(0x85, inst_info_avm2("coerce_s"));
			s_instr.add(0x86, inst_info_avm2("astype", ARG_MULTINAME, ARG_END));
			s_instr.add(0x87, inst_info_avm2("astypelate", ARG_MULTINAME, ARG_END));
			// no inst for 0x88->0x8F
			s_instr.add(0x90, inst_info_avm2("negate"));
			s_instr.add(0x91, inst_info_avm2("increment"));
			s_instr.add(0x92, inst_info_avm2("inclocal", ARG_REGISTER, ARG_END));
			s_instr.add(0x93, inst_info_avm2("decrement"));
			s_instr.add(0x94, inst_info_avm2("declocal", ARG_REGISTER, ARG_END));
			s_instr.add(0x95, inst_info_avm2("typeof"));
			s_instr.add(0x96, inst_info_avm2("not"));
			s_instr.add(0x97, inst_info_avm2("bitnot"));
			// no inst for 0x99->0x0x9F
			s_instr.add(0xA0, inst_info_avm2("add"));
			s_instr.add(0xA1, inst_info_avm2("subtract"));
			s_instr.add(0xA2, inst_info_avm2("multiply"));
			s_instr.add(0xA3, inst_info_avm2("divide"));
			s_instr.add(0xA4, inst_info_avm2("modulo"));
			s_instr.add(0xA5, inst_info_avm2("lshift"));
			s_instr.add(0xA6, inst_info_avm2("rshift"));
			s_instr.add(0xA7, inst_info_avm2("urshift"));
			s_instr.add(0xA8, inst_info_avm2("bitand"));
			s_instr.add(0xA9, inst_info_avm2("bitor"));
			s_instr.add(0xAA, inst_info_avm2("bitxor"));
			s_instr.add(0xAB, inst_info_avm2("equals"));
			s_instr.add(0xAC, inst_info_avm2("strictequals"));
			s_instr.add(0xAD, inst_info_avm2("lessthan"));
			s_instr.add(0xAE, inst_info_avm2("lessequals"));
			s_instr.add(0xAF, inst_info_avm2("greaterequals"));
			// no inst for 0xB0
			s_instr.add(0xB1, inst_info_avm2("instanceof"));
			s_instr.add(0xB2, inst_info_avm2("istype", ARG_MULTINAME, ARG_END));
			s_instr.add(0xB3, inst_info_avm2("istypelate"));
			s_instr.add(0xB4, inst_info_avm2("in"));
			// no inst for 0xB5
			s_instr.add(0xC0, inst_info_avm2("increment_i"));
			s_instr.add(0xC1, inst_info_avm2("decrement_i"));
			s_instr.add(0xC2, inst_info_avm2("inclocal_i", ARG_REGISTER, ARG_END));
			s_instr.add(0xC3, inst_info_avm2("declocal_i", ARG_REGISTER, ARG_END));
			s_instr.add(0xC4, inst_info_avm2("negate_i"));
			s_instr.add(0xC5, inst_info_avm2("add_i"));
			s_instr.add(0xC6, inst_info_avm2("subtract_i"));
			s_instr.add(0xC7, inst_info_avm2("multiply_i"));
			// no inst for 0xC8 - > 0xCF
			s_instr.add(0xD0, inst_info_avm2("getlocal_0"));
			s_instr.add(0xD1, inst_info_avm2("getlocal_1"));
			s_instr.add(0xD2, inst_info_avm2("getlocal_2"));
			s_instr.add(0xD3, inst_info_avm2("getlocal_3"));
			s_instr.add(0xD4, inst_info_avm2("setlocal_0"));
			s_instr.add(0xD5, inst_info_avm2("setlocal_1"));
			s_instr.add(0xD6, inst_info_avm2("setlocal_2"));
			s_instr.add(0xD7, inst_info_avm2("setlocal_3"));
			// no inst for 0xD8 - > 0xEE
			s_instr.add(0xEF, inst_info_avm2("debug", ARG_BYTE, ARG_STRING, ARG_BYTE, ARG_SHORT));
			s_instr.add(0xF0, inst_info_avm2("debugline", ARG_SHORT, ARG_END));
			s_instr.add(0xF1, inst_info_avm2("debugfile", ARG_STRING, ARG_END));

		}

		assert(data.size() > 0);

		int ip = 0;
		do
		{
			int opcode = data[ip];
			inst_info_avm2 ii(0);
			if (s_instr.get(opcode, &ii))
			{
				printf(":	%s\n", ii.m_instruction);
				if( ii.has_argument() )
				{
					ip += ii.process( def, &data[ip] );
				}
				else
				{
					ip++;
				}
			}
			else
			{
				log_msg(":	unknown opcode 0x%02X\n", opcode);
				ip++;
			}

		}
		while (ip < data.size());
	}


	//
	// Action Script 2.0
	//

	enum arg_format
	{
		ARG_NONE = 0,
		ARG_STR,
		ARG_HEX,	// default hex dump, in case the format is unknown or unsupported
		ARG_U8,
		ARG_U16,
		ARG_S16,
		ARG_PUSH_DATA,
		ARG_DECL_DICT,
		ARG_FUNCTION2
	};

	struct inst_info
	{
		int	m_action_id;
		const char*	m_instruction;

		arg_format	m_arg_format;
	};


	void	log_disasm(const unsigned char* instruction_data)
	// Disassemble one instruction to the log.
	{
		static inst_info	s_instruction_table[] = {
			{ 0x04, "next_frame", ARG_NONE },
			{ 0x05, "prev_frame", ARG_NONE },
			{ 0x06, "play", ARG_NONE },
			{ 0x07, "stop", ARG_NONE },
			{ 0x08, "toggle_qlty", ARG_NONE },
			{ 0x09, "stop_sounds", ARG_NONE },
			{ 0x0A, "add", ARG_NONE },
			{ 0x0B, "sub", ARG_NONE },
			{ 0x0C, "mul", ARG_NONE },
			{ 0x0D, "div", ARG_NONE },
			{ 0x0E, "equ", ARG_NONE },
			{ 0x0F, "lt", ARG_NONE },
			{ 0x10, "and", ARG_NONE },
			{ 0x11, "or", ARG_NONE },
			{ 0x12, "not", ARG_NONE },
			{ 0x13, "str_eq", ARG_NONE },
			{ 0x14, "str_len", ARG_NONE },
			{ 0x15, "substr", ARG_NONE },
			{ 0x17, "pop", ARG_NONE },
			{ 0x18, "floor", ARG_NONE },
			{ 0x1C, "get_var", ARG_NONE },
			{ 0x1D, "set_var", ARG_NONE },
			{ 0x20, "set_target_exp", ARG_NONE },
			{ 0x21, "str_cat", ARG_NONE },
			{ 0x22, "get_prop", ARG_NONE },
			{ 0x23, "set_prop", ARG_NONE },
			{ 0x24, "dup_sprite", ARG_NONE },
			{ 0x25, "rem_sprite", ARG_NONE },
			{ 0x26, "trace", ARG_NONE },
			{ 0x27, "start_drag", ARG_NONE },
			{ 0x28, "stop_drag", ARG_NONE },
			{ 0x29, "str_lt", ARG_NONE },
			{ 0x2B, "cast_object", ARG_NONE },
			{ 0x30, "random", ARG_NONE },
			{ 0x31, "mb_length", ARG_NONE },
			{ 0x32, "ord", ARG_NONE },
			{ 0x33, "chr", ARG_NONE },
			{ 0x34, "get_timer", ARG_NONE },
			{ 0x35, "substr_mb", ARG_NONE },
			{ 0x36, "ord_mb", ARG_NONE },
			{ 0x37, "chr_mb", ARG_NONE },
			{ 0x3A, "delete", ARG_NONE },
			{ 0x3B, "delete_all", ARG_NONE },
			{ 0x3C, "set_local", ARG_NONE },
			{ 0x3D, "call_func", ARG_NONE },
			{ 0x3E, "return", ARG_NONE },
			{ 0x3F, "mod", ARG_NONE },
			{ 0x40, "new", ARG_NONE },
			{ 0x41, "decl_local", ARG_NONE },
			{ 0x42, "decl_array", ARG_NONE },
			{ 0x43, "decl_obj", ARG_NONE },
			{ 0x44, "type_of", ARG_NONE },
			{ 0x45, "get_target", ARG_NONE },
			{ 0x46, "enumerate", ARG_NONE },
			{ 0x47, "add_t", ARG_NONE },
			{ 0x48, "lt_t", ARG_NONE },
			{ 0x49, "eq_t", ARG_NONE },
			{ 0x4A, "number", ARG_NONE },
			{ 0x4B, "string", ARG_NONE },
			{ 0x4C, "dup", ARG_NONE },
			{ 0x4D, "swap", ARG_NONE },
			{ 0x4E, "get_member", ARG_NONE },
			{ 0x4F, "set_member", ARG_NONE },
			{ 0x50, "inc", ARG_NONE },
			{ 0x51, "dec", ARG_NONE },
			{ 0x52, "call_method", ARG_NONE },
			{ 0x53, "new_method", ARG_NONE },
			{ 0x54, "is_inst_of", ARG_NONE },
			{ 0x55, "enum_object", ARG_NONE },
			{ 0x60, "bit_and", ARG_NONE },
			{ 0x61, "bit_or", ARG_NONE },
			{ 0x62, "bit_xor", ARG_NONE },
			{ 0x63, "shl", ARG_NONE },
			{ 0x64, "asr", ARG_NONE },
			{ 0x65, "lsr", ARG_NONE },
			{ 0x66, "eq_strict", ARG_NONE },
			{ 0x67, "gt_t", ARG_NONE },
			{ 0x68, "gt_str", ARG_NONE },
			{ 0x69, "extends", ARG_NONE },
			
			{ 0x81, "goto_frame", ARG_U16 },
			{ 0x83, "get_url", ARG_STR },
			{ 0x87, "store_register", ARG_U8 },
			{ 0x88, "decl_dict", ARG_DECL_DICT },
			{ 0x8A, "wait_for_frame", ARG_HEX },
			{ 0x8B, "set_target", ARG_STR },
			{ 0x8C, "goto_frame_lbl", ARG_STR },
			{ 0x8D, "wait_for_fr_exp", ARG_HEX },
			{ 0x8E, "function2", ARG_FUNCTION2 },
			{ 0x94, "with", ARG_U16 },
			{ 0x96, "push_data", ARG_PUSH_DATA },
			{ 0x99, "goto", ARG_S16 },
			{ 0x9A, "get_url2", ARG_HEX },
			// { 0x8E, "function2", ARG_HEX },
			{ 0x9B, "func", ARG_HEX },
			{ 0x9D, "branch_if_true", ARG_S16 },
			{ 0x9E, "call_frame", ARG_HEX },
			{ 0x9F, "goto_frame_exp", ARG_HEX },
			{ 0x00, "<end>", ARG_NONE }
		};

		int	action_id = instruction_data[0];
		inst_info*	info = NULL;

		for (int i = 0; ; i++)
		{
			if (s_instruction_table[i].m_action_id == action_id)
			{
				info = &s_instruction_table[i];
			}

			if (s_instruction_table[i].m_action_id == 0)
			{
				// Stop at the end of the table and give up.
				break;
			}
		}

		arg_format	fmt = ARG_HEX;

		// Show instruction.
		if (info == NULL)
		{
			log_msg("<unknown>[0x%02X]", action_id);
		}
		else
		{
			log_msg("%-15s", info->m_instruction);
			fmt = info->m_arg_format;
		}

		// Show instruction argument(s).
		if (action_id & 0x80)
		{
			assert(fmt != ARG_NONE);

			int	length = instruction_data[1] | (instruction_data[2] << 8);

			// log_msg(" [%d]", length);

			if (fmt == ARG_HEX)
			{
				for (int i = 0; i < length; i++)
				{
					log_msg(" 0x%02X", instruction_data[3 + i]);
				}
				log_msg("\n");
			}
			else if (fmt == ARG_STR)
			{
				log_msg(" \"");
				for (int i = 0; i < length; i++)
				{
					log_msg("%c", instruction_data[3 + i]);
				}
				log_msg("\"\n");
			}
			else if (fmt == ARG_U8)
			{
				int	val = instruction_data[3];
				log_msg(" %d\n", val);
			}
			else if (fmt == ARG_U16)
			{
				int	val = instruction_data[3] | (instruction_data[4] << 8);
				log_msg(" %d\n", val);
			}
			else if (fmt == ARG_S16)
			{
				int	val = instruction_data[3] | (instruction_data[4] << 8);
				if (val & 0x8000) val |= ~0x7FFF;	// sign-extend
				log_msg(" %d\n", val);
			}
			else if (fmt == ARG_PUSH_DATA)
			{
				log_msg("\n");
				int i = 0;

				while (i < length)
				{
					int	type = instruction_data[3 + i];
					i++;
					log_msg("\t\t");	// indent
					if (type == 0)
					{
						// string
						log_msg("\"");
						while (instruction_data[3 + i])
						{
							log_msg("%c", instruction_data[3 + i]);
							i++;
						}
						i++;
						log_msg("\"\n");
					}
					else if (type == 1)
					{
						// float (little-endian)
						union {
							float	f;
							Uint32	i;
						} u;
						compiler_assert(sizeof(u) == sizeof(u.i));

						memcpy(&u.i, instruction_data + 3 + i, 4);
						u.i = swap_le32(u.i);
						i += 4;

						log_msg("(float) %f\n", u.f);
					}
					else if (type == 2)
					{
						log_msg("NULL\n");
					}
					else if (type == 3)
					{
						log_msg("undef\n");
					}
					else if (type == 4)
					{
						// contents of register
						int	reg = instruction_data[3 + i];
						i++;
						log_msg("reg[%d]\n", reg);
					}
					else if (type == 5)
					{
						int	bool_val = instruction_data[3 + i];
						i++;
						log_msg("bool(%d)\n", bool_val);
					}
					else if (type == 6)
					{
						// double
						// wacky format: 45670123
						union {
							double	d;
							Uint64	i;
							struct {
								Uint32	lo;
								Uint32	hi;
							} sub;
						} u;
						compiler_assert(sizeof(u) == sizeof(u.i));

						memcpy(&u.sub.hi, instruction_data + 3 + i, 4);
						memcpy(&u.sub.lo, instruction_data + 3 + i + 4, 4);
						u.i = swap_le64(u.i);
						i += 8;


						log_msg("(double) %f\n", u.d);
					}
					else if (type == 7)
					{
						// int32
						Sint32	val = instruction_data[3 + i]
							| (instruction_data[3 + i + 1] << 8)
							| (instruction_data[3 + i + 2] << 16)
							| (instruction_data[3 + i + 3] << 24);
						i += 4;
						log_msg("(int) %d\n", val);
					}
					else if (type == 8)
					{
						int	id = instruction_data[3 + i];
						i++;
						log_msg("dict_lookup[%d]\n", id);
					}
					else if (type == 9)
					{
						int	id = instruction_data[3 + i] | (instruction_data[3 + i + 1] << 8);
						i += 2;
						log_msg("dict_lookup_lg[%d]\n", id);
					}
				}
			}
			else if (fmt == ARG_DECL_DICT)
			{
				int	i = 0;
				int	count = instruction_data[3 + i] | (instruction_data[3 + i + 1] << 8);
				i += 2;

				log_msg(" [%d]\n", count);

				// Print strings.
				for (int ct = 0; ct < count; ct++)
				{
					log_msg("\t\t");	// indent

					log_msg("\"");
					while (instruction_data[3 + i])
					{
						// safety check.
						if (i >= length)
						{
							log_msg("<disasm error -- length exceeded>\n");
							break;
						}

						log_msg("%c", instruction_data[3 + i]);
						i++;
					}
					log_msg("\"\n");
					i++;
				}
			}
			else if (fmt == ARG_FUNCTION2)
			{
				// Signature info for a function2 opcode.
				int	i = 0;
				const char*	function_name = (const char*) &instruction_data[3 + i];
				i += (int) strlen(function_name) + 1;

				int	arg_count = instruction_data[3 + i] | (instruction_data[3 + i + 1] << 8);
				i += 2;

				int	reg_count = instruction_data[3 + i];
				i++;

				log_msg("\n\t\tname = '%s', arg_count = %d, reg_count = %d\n",
					function_name, arg_count, reg_count);

				uint16	flags = (instruction_data[3 + i]) | (instruction_data[3 + i + 1] << 8);
				i += 2;

				// @@ What is the difference between "super" and "_parent"?

				bool	preload_global = (flags & 0x100) != 0;
				bool	preload_parent = (flags & 0x80) != 0;
				bool	preload_root   = (flags & 0x40) != 0;

				bool	suppress_super = (flags & 0x20) != 0;
				bool	preload_super  = (flags & 0x10) != 0;
				bool	suppress_args  = (flags & 0x08) != 0;
				bool	preload_args   = (flags & 0x04) != 0;
				bool	suppress_this  = (flags & 0x02) != 0;
				bool	preload_this   = (flags & 0x01) != 0;

				log_msg("\t\t		pg = %d\n"
					"\t\t		pp = %d\n"
					"\t\t		pr = %d\n"
					"\t\tss = %d, ps = %d\n"
					"\t\tsa = %d, pa = %d\n"
					"\t\tst = %d, pt = %d\n",
					int(preload_global),
					int(preload_parent),
					int(preload_root),
					int(suppress_super),
					int(preload_super),
					int(suppress_args),
					int(preload_args),
					int(suppress_this),
					int(preload_this));

				for (int argi = 0; argi < arg_count; argi++)
				{
					int	arg_register = instruction_data[3 + i];
					i++;
					const char*	arg_name = (const char*) &instruction_data[3 + i];
					i += (int) strlen(arg_name) + 1;

					log_msg("\t\targ[%d] - reg[%d] - '%s'\n", argi, arg_register, arg_name);
				}

				int	function_length = instruction_data[3 + i] | (instruction_data[3 + i + 1] << 8);
				i += 2;

				log_msg("\t\tfunction length = %d\n", function_length);
			}
		}
		else
		{
			log_msg("\n");
		}
	}

#endif // COMPILE_DISASM

};


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
