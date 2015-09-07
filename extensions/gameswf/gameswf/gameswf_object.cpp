// gameswf_action.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// A generic bag of attributes.	 Base-class for ActionScript
// script-defined objects.

#include "gameswf_object.h"
#include "gameswf_action.h"
#include "gameswf_function.h"
#include "gameswf_log.h"
#include "gameswf_movie_def.h"

namespace gameswf
{

	const char*	next_slash_or_dot(const char* word);

	void	as_object_addproperty(const fn_call& fn)
	{
		if (fn.nargs == 3)
		{
			assert(fn.this_ptr);

			// creates unbinded property
			// fn.this_ptr->set_member(fn.arg(0).to_string(), as_value(fn.arg(1), fn.arg(2)));
			// force direct rewriting of member
			fn.this_ptr->builtin_member(fn.arg(0).to_string(), as_value(fn.arg(1), fn.arg(2)));
			
			fn.result->set_bool(true);
			return;
		}
		fn.result->set_bool(false);
	}
	
	//static registerClass(name:String, theClass:Function) : Boolean
	void	as_object_registerclass(const fn_call& fn)
	{
		fn.result->set_bool(false);
		if (fn.nargs == 2 && fn.env->get_target() != NULL)
		{
			character_def* def = fn.env->get_target()->find_exported_resource(fn.arg(0).to_tu_string());
			if (def)
			{
				as_function* func = cast_to<as_function>(fn.arg(1).to_object());
				if (func)
				{
					IF_VERBOSE_ACTION(log_msg("registerClass '%s'\n",	fn.arg(0).to_string()));
					fn.result->set_bool(true);
					def->set_registered_class_constructor(func);
				}
			}
			else
			{
				log_error("can't find exported resource '%s'\n", fn.arg(0).to_string());
			}
		}
	}

	// public hasOwnProperty(name:String) : Boolean
	// Indicates whether an object has a specified property defined. 
	// This method returns true if the target object has a property that
	// matches the string specified by the name parameter, and false otherwise.
	// This method does not check the object's prototype chain and returns true only 
	// if the property exists on the object itself.
	void	as_object_hasownproperty(const fn_call& fn)
	{
		if (fn.nargs == 1)
		{
			assert(fn.this_ptr);
			as_value m;
			if (fn.this_ptr->m_members.get(fn.arg(0).to_tu_stringi(), &m))
			{
				fn.result->set_bool(true);
				return;
			}
		}
		fn.result->set_bool(false);
	}


	// public watch(name:String, callback:Function, [userData:Object]) : Boolean
	// Registers an event handler to be invoked when a specified property of
	// an ActionScript object changes. When the property changes,
	// the event handler is invoked with myObject as the containing object. 
	void	as_object_watch(const fn_call& fn)
	{
		bool ret = false;
		if (fn.nargs >= 2)
		{
			assert(fn.this_ptr);

			ret = fn.this_ptr->watch(fn.arg(0).to_tu_string(),
							fn.arg(1).to_function(), fn.nargs > 2 ? fn.arg(2) : as_value());
		}
		fn.result->set_bool(ret);
	}

	//public unwatch(name:String) : Boolean
	// Removes a watchpoint that Object.watch() created.
	// This method returns a value of true if the watchpoint is successfully removed,
	// false otherwise.
	void	as_object_unwatch(const fn_call& fn)
	{
		bool ret = false;
		if (fn.nargs == 1)
		{
			assert(fn.this_ptr);
			ret = fn.this_ptr->unwatch(fn.arg(0).to_tu_string());
		}
		fn.result->set_bool(ret);
	}

	// for debugging
	void	as_object_dump(const fn_call& fn)
	{
		if (fn.this_ptr)
		{
			tu_string tabs;
			fn.this_ptr->dump(tabs);
		}
	}

	void	as_global_object_ctor(const fn_call& fn)
	// Constructor for ActionScript class Object.
	{
		fn.result->set_as_object(new as_object(fn.get_player()));
	}

	// flash9
	void	as_object_add_event_listener(const fn_call& fn)
	// add 'this' as listener of as_event object
	{
		if (fn.nargs >= 2)
		{
			// find event handler

			assert(fn.this_ptr);
			as_value val;
			if (fn.this_ptr->get_global()->get_member("flash", &val) == false)
			{
				return;
			}

			as_object* flash_package = val.to_object();
			if (flash_package->get_global()->get_member("MouseEvent", &val) == false)
			{
				return;
			}


		}
	}


	// this stuff should be high optimized
	// therefore we can't use here set_member(...)
	as_object::as_object(player* player) :
		m_watch(NULL),
		m_player(player)
	{
		// as_c_function has no pointer to player
//		assert(player);
	}

	as_object::~as_object()
	{
		delete m_watch;
	}

	// called from a object constructor only
	void	as_object::builtin_member(const tu_stringi& name, const as_value& val)
	{
		val.set_flags(as_value::DONT_ENUM);
		m_members.set(name, val);
	}

	void as_object::call_watcher(const tu_stringi& name, const as_value& old_val, as_value* new_val)
	{
		if (m_watch)
		{
			as_watch watch;
			m_watch->get(name, &watch);
			if (watch.m_func)
			{
				as_environment env(get_player());
				env.push(watch.m_user_data);	// params
				env.push(*new_val);		// newVal
				env.push(old_val);	// oldVal
				env.push(name);	// property
				new_val->set_undefined();
				(*watch.m_func)(fn_call(new_val, this, &env, 4, env.get_top_index()));
			}
		}
	}

	bool	as_object::set_member(const tu_stringi& name, const as_value& new_val)
	{
//		printf("SET MEMBER: %s at %p for object %p\n", name.c_str(), val.to_object(), this);
		as_value val(new_val);
		as_value old_val;
		if (as_object::get_member(name, &old_val))
		{
			if (old_val.is_property())
			{
				old_val.set_property(val);
				return true;
			}
		}

		// try watcher
		call_watcher(name, old_val, &val);

		stringi_hash<as_value>::const_iterator it = this->m_members.find(name);
		if (it != this->m_members.end())
		{
			// update a old members
			// is the member read-only ?
			if (it->second.is_readonly() == false)
			{
				m_members.set(name, val);
			}
		}
		else
		{
			// create a new members
			m_members.set(name, val);
		}
		return true;
	}

	as_object* as_object::get_proto() const
	{
		return m_proto.get_ptr();
	}

	bool	as_object::get_member(const tu_stringi& name, as_value* val)
	{
		//printf("GET MEMBER: %s at %p for object %p\n", name.c_str(), val, this);
		
		// first try built-ins object methods
		if (get_builtin(BUILTIN_OBJECT_METHOD, name, val))
		{
			return true;
		}

		if (m_members.get(name, val) == false)
		{
			as_object* proto = get_proto();
			if (proto == NULL)
			{
				return false;
			}

			if (proto->get_member(name, val) == false)
			{
				return false;
			}
		}

		if (val->is_property())
		{
			val->set_property_target(this);
		}

		return true;
	}

	bool	as_object::find_property( const tu_stringi & name, as_value * val )
	{
		as_value dummy;
		if( get_member(name, &dummy) )
		{
			val->set_as_object( this );
			return true;
		}

		if(m_instance.get_ptr() != NULL)
		{
			// create traits
			for (int i = 0; i < m_instance->m_trait.size(); i++)
			{
				traits_info* ti = m_instance->m_trait[i].get();
				const char* traits_name = m_instance->m_abc->get_multiname(ti->m_name);

				if( name == traits_name)
				{
					if(ti->m_kind == traits_info::Trait_Slot)
					{
						val->set_as_object( this );
						return true;
					}
					return false;
				}
			}
		}

		as_object *proto = get_proto();
		if( proto )
		{
			return proto->find_property( name, val );
		}

		return false;
	}

	void	as_object::clear_refs(gameswf::hash<as_object*, bool>* visited_objects, as_object* this_ptr)
	{
		// Is it a reentrance ?
		if (visited_objects->get(this, NULL))
		{
			return;
		}
		visited_objects->set(this, true);

		as_value undefined;
		for (stringi_hash<as_value>::iterator it = m_members.begin();
			it != m_members.end(); ++it)
		{
			as_object* obj = it->second.to_object();
			if (obj)
			{
				if (obj == this_ptr)
				{
					it->second.set_undefined();
				}
				else
				{
					obj->clear_refs(visited_objects, this_ptr);
				}
				continue;
			}

			as_property* prop = it->second.to_property();
			if (prop)
			{
				if (it->second.get_property_target() == this_ptr)
				{
					const_cast<as_value&>(it->second).set_property_target(NULL);
				}
			}
		}
	}

	bool	as_object::on_event(const event_id& id)
	{
		// Check for member function.
		bool called = false;
		{
			const tu_stringi&	method_name = id.get_function_name().to_tu_stringi();
			if (method_name.length() > 0)
			{
				as_value	method;
				if (get_member(method_name, &method))
				{
//					as_environment env(get_player());

					// use _root environment
					character* mroot = get_player()->get_root_movie();
					as_environment* env = mroot->get_environment();
					
					// keep stack size
					int stack_size = env->get_stack_size();

					int nargs = 0;
					if (id.m_args)
					{
						nargs = id.m_args->size();
						for (int i = nargs - 1; i >=0; i--)
						{
							env->push((*id.m_args)[i]);
						}
					}
					call_method(method, env, this, nargs, env->get_top_index());
					called = true;

					// restore stack size
					env->set_stack_size(stack_size);
				}
			}
		}

		return called;
	}

	void as_object::enumerate(as_environment* env)
	// retrieves members & pushes them into env
	{
		stringi_hash<as_value>::const_iterator it = m_members.begin();
		while (it != m_members.end())
		{
			if (it->second.is_enum())
			{
				env->push(it->first);

				IF_VERBOSE_ACTION(log_msg("-------------- enumerate - push: %s\n",
					it->first.c_str()));
			}

			++it;
		}

//		as_object_interface* proto = get_proto();
//		if (proto)
//		{
//			proto->enumerate(env);
//		}
	}

	bool as_object::watch(const tu_string& name, as_function* callback,
		const as_value& user_data)
	{
		if (callback == NULL)
		{
			return false;
		}

		as_watch watch;
		watch.m_func = callback;
		watch.m_user_data = user_data;
		
		if (m_watch == NULL)
		{
			m_watch = new stringi_hash<as_watch>;
		}
		m_watch->set(name, watch);
		return true;
	}

	bool as_object::unwatch(const tu_string& name)
	{
		if (m_watch)
		{
			as_watch watch;
			if (m_watch->get(name, &watch))
			{
				m_watch->erase(name);
				return true;
			}
		}
		return false;
	}

	void as_object::copy_to(as_object* target)
	// Copy all members from 'this' to target
	{
		if (target)
		{
			for (stringi_hash<as_value>::const_iterator it = m_members.begin(); 
				it != m_members.end(); ++it ) 
			{ 
				target->set_member(it->first, it->second); 
			} 
		}
	}

	void as_object::dump()
	// for internal using
	{
		tu_string tab;
		dump(tab);
	}

	void as_object::dump(tu_string& tabs)
	// for debugging, used from action script
	// retrieves members & print them
	{
		tabs += "  ";
		printf("%s*** object 0x%p ***\n", tabs.c_str(), this);
		for (stringi_hash<as_value>::const_iterator it = m_members.begin(); 
			it != m_members.end(); ++it)
		{
			const as_value& val = it->second;
			if (val.is_property())
			{
				printf("%s%s: <as_property 0x%p, target 0x%p, getter 0x%p, setter 0x%p>\n",
								tabs.c_str(), 
								it->first.c_str(), val.to_property(), val.get_property_target(),
								val.to_property()->m_getter.get_ptr(), val.to_property()->m_setter.get_ptr());
			}
			else
			if (val.is_function())
			{
				if (cast_to<as_s_function>(val.to_object()))
				{
					printf("%s%s: <as_s_function 0x%p>\n", tabs.c_str(), 
						it->first.c_str(), val.to_object());
				}
				else
				if (cast_to<as_3_function>(val.to_object()))
				{
					printf("%s%s: <as_3_function 0x%p>\n", tabs.c_str(), 
						it->first.c_str(), val.to_object());
				}
				else
				{
					printf("%s%s: <as_c_function 0x%p>\n", tabs.c_str(), 
						it->first.c_str(), val.to_object());
				}
			}
			else if (val.is_object())
			{
				printf("%s%s: <as_object 0x%p>\n",
					tabs.c_str(), 
					it->first.c_str(), val.to_object());
			}
			else
			{
				printf("%s%s: %s\n", 
					tabs.c_str(), 
					it->first.c_str(), it->second.to_string());
			}
		}

		// dump proto
		if (m_proto != NULL)
		{
			m_proto->dump(tabs);
		}
		tabs.resize(tabs.size() - 2);
	}

	as_object*	as_object::find_target(const as_value& target)
	// Find the object referenced by the given target.
	{
		if (target.is_string() == false)
		{
			return target.to_object();
		}

		const tu_string& path = target.to_tu_string();
		if (path.length() == 0)
		{
			return this;
		}

		as_value val;
		as_object* tar = NULL;

		// absolute path ?
		if (*path.c_str() == '/')
		{
			return m_player->get_root_movie()->find_target(path.c_str() + 1);
		}

		const char* slash = strchr(path.c_str(), '/');
		if (slash == NULL)
		{
			slash = strchr(path.c_str(), '.');
			if (slash)
			{
				if (slash[1] == '.')
				{
					slash = NULL;
				}
			}
		}

		if (slash)
		{
			tu_string name(path.c_str(), int(slash - path.c_str()));
			get_member(name, &val);
			tar = val.to_object();
			if (tar)	
			{
				return tar->find_target(slash + 1);
			}
		}
		else
		{
			get_member(path, &val);
			tar = val.to_object();
		}

		if (tar == NULL)
		{
			log_error("can't find target %s\n", path.c_str());
		}
		return tar;
	}

	// mark 'this' as alive
	void as_object::this_alive()
	{
		// Whether there were we here already ?
		if (m_player != NULL && m_player->is_garbage(this))
		{
			// 'this' and its members is alive
			m_player->set_alive(this);
			for (stringi_hash<as_value>::iterator it = m_members.begin();
				it != m_members.end(); ++it)
			{
				as_object* obj = it->second.to_object();
				if (obj)
				{
					obj->this_alive();
				}
			}
		}
	}

	double	as_object::to_number()
	{
		const char* str = to_string();
		if (str)
		{
			return atof(str);
		}
		return 0;
	}

	// for optimization I don't want to put '__constructor__' into as_object
	// so the missing of '__constructor__' in a object means 
	// that it is a instance of as_object
	bool	as_object::is_instance_of(const as_function* constructor) const
	{
		// by default ctor is as_global_object_ctor
		as_value ctor;
		get_ctor(&ctor);
		if (ctor.is_undefined())
		{
			ctor.set_as_c_function(as_global_object_ctor);
		}

		const as_s_function* sf = cast_to<as_s_function>(constructor);
		if (sf && sf == cast_to<as_s_function>(ctor.to_function()))
		{
			return true;
		}

		const as_c_function* cf1 = cast_to<as_c_function>(constructor);
		const as_c_function* cf2 = cast_to<as_c_function>(ctor.to_function());
		if (cf1 && cf2 && cf1->m_func == cf2->m_func)
		{
			return true;
		}

		as_object* proto = get_proto();
		if (proto)
		{
			return proto->is_instance_of(constructor);
		}
		return false;
	}

	as_object* as_object::get_global() const
	{
		assert(m_player != NULL);
		return m_player->get_global();
	}

	root* as_object::get_root() const
	{
		assert(m_player != NULL);
		return m_player->get_root(); 
	}

	static tu_string s_constructor("__constructor__");
	bool as_object::get_ctor(as_value* val) const
	{
		return m_members.get(s_constructor, val);
	}

	void as_object::set_ctor(const as_value& val)
	{
		builtin_member(s_constructor, val);
	}

	void as_object::set_instance(instance_info * info)
	{
		m_instance = info;
	}

	as_object* as_object::create_proto(const as_value& constructor)
	{ 	 
		m_proto = new as_object(get_player()); 	 
		m_proto->m_this_ptr = m_this_ptr; 	

		if (constructor.to_object()) 	 
		{ 	 
			// constructor is as_s_function 	 
			as_value val;
			constructor.to_object()->get_member("prototype", &val);
			as_object* prototype = val.to_object();
			assert(prototype);
			prototype->copy_to(this);

			as_value prototype_constructor;
			if (prototype->get_ctor(&prototype_constructor))
			{
				m_proto->set_ctor(prototype_constructor);
			}
		}

		set_ctor(constructor);
		return m_proto.get_ptr();
	}

}
