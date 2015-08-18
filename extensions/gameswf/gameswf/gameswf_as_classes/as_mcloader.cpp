// as_mcloader.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Action Script MovieClipLoader implementation code for the gameswf SWF player library.

#include "as_mcloader.h"
#include "../gameswf_root.h"
#include "../gameswf_character.h"
#include "../gameswf_sprite.h"
#include "../gameswf_action.h"
#include "../gameswf_function.h"
//#include "gameswf_log.h"

namespace gameswf
{

	void	as_mcloader_addlistener(const fn_call& fn)
	{
		as_mcloader* mcl = cast_to<as_mcloader>(fn.this_ptr);
		assert(mcl);

		if (fn.nargs == 1)
		{
			mcl->m_listeners.add(fn.arg(0).to_object());
			fn.result->set_bool(true);
			mcl->get_root()->add_listener(mcl);
			return;
		}
		fn.result->set_bool(false);
	}

	void	as_mcloader_removelistener(const fn_call& fn)
	{
		as_mcloader* mcl = cast_to<as_mcloader>(fn.this_ptr);
		assert(mcl);

		if (fn.nargs == 1)
		{
			mcl->m_listeners.remove(fn.arg(0).to_object());
			fn.result->set_bool(true);
			return;
		}
		fn.result->set_bool(false);
	}

	void	as_mcloader_loadclip(const fn_call& fn)
	{
		as_mcloader* mcl = cast_to<as_mcloader>(fn.this_ptr);
		assert(mcl);

		fn.result->set_bool(false);	// on default
		if (fn.nargs == 2)
		{
			swf_array<as_value> event_args;	// for event handler args
			event_args.push_back(as_value());	// undefined

			tu_string infile = get_full_url(fn.get_player()->get_workdir(), fn.arg(0).to_string());
			movie_definition*	md = fn.get_player()->create_movie(infile.c_str());
			if (md == NULL)
			{
				IF_VERBOSE_ACTION(log_msg("can't create movie from %s\n", fn.arg(0).to_string()));
				event_args.push_back("URLNotFound");	// 2-d param
				mcl->m_listeners.notify(event_id(event_id::ONLOAD_ERROR, &event_args));
				return;
			}

			as_mcloader::loadable_movie lm;
			lm.m_def = cast_to<movie_def_impl>(md);
			lm.m_target = cast_to<character>(fn.env->find_target(fn.arg(1)));
			mcl->m_lm.push_back(lm);

			mcl->m_listeners.notify(event_id(event_id::ONLOAD_START, &event_args));
			fn.result->set_bool(true);
		}
	}

	void	as_mcloader_unloadclip(const fn_call& fn)
	{
		if (fn.nargs == 1)
		{
			fn.env->load_file("", fn.arg(0));
			fn.result->set_bool(true);
			return;
		}
		fn.result->set_bool(false);
	}

	void	as_mcloader_getprogress(const fn_call& fn)
	{
		if (fn.nargs == 1)
		{
			sprite_instance* m = cast_to<sprite_instance>(fn.arg(0).to_object());
			if (m)
			{
				as_object* info = new as_object(fn.get_player());
				info->set_member("bytesLoaded", (int) m->get_loaded_bytes());
				info->set_member("bytesTotal", (int) m->get_file_bytes());
				fn.result->set_as_object(info);
				return;
			}
		}
		fn.result->set_as_object(NULL);
	}

	void	as_global_mcloader_ctor(const fn_call& fn)
	// Constructor for ActionScript class MovieClipLoader
	{
		fn.result->set_as_object(new as_mcloader(fn.get_player()));
	}

	as_mcloader::as_mcloader(player* player) :
		as_object(player)
	{
		builtin_member("addListener", as_mcloader_addlistener);
		builtin_member("removeListener", as_mcloader_removelistener);
		builtin_member("loadClip", as_mcloader_loadclip);
		builtin_member("unloadClip", as_mcloader_unloadclip);
		builtin_member("getProgress", as_mcloader_getprogress);
	}

	as_mcloader::~as_mcloader()
	{
	}
	
	void	as_mcloader::advance(float delta_time)
	{
		if (m_lm.size() == 0)
		{
			get_root()->remove_listener(this);
			return;
		}

		for (int i = 0; i < m_lm.size();)
		{
			swf_array<as_value> event_args;		// for event handler args
			event_args.push_back(m_lm[i].m_ch);

			int nframe = m_lm[i].m_def->get_loading_frame();
			if (nframe > 0 && m_lm[i].m_ch == NULL)
			{
				if (m_lm[i].m_target != NULL)
				{
					m_lm[i].m_ch = m_lm[i].m_target->replace_me(m_lm[i].m_def.get_ptr());
				}
				m_listeners.notify(event_id(event_id::ONLOAD_INIT, &event_args));
			}

			int loaded = m_lm[i].m_def->get_loaded_bytes();
			int total = m_lm[i].m_def->get_file_bytes();

			if (loaded < total)
			{
				event_args.push_back(loaded);
				event_args.push_back(total);
				m_listeners.notify(event_id(event_id::ONLOAD_PROGRESS, &event_args));
			}
			else
			{
				event_args.push_back(loaded);
				event_args.push_back(total);
				m_listeners.notify(event_id(event_id::ONLOAD_PROGRESS, &event_args));
				m_listeners.notify(event_id(event_id::ONLOAD_COMPLETE, &event_args));

				m_lm.remove(i);
				continue;
			}

			i++;

		}
	}

};
