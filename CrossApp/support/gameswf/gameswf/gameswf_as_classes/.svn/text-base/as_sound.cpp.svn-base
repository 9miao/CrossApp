// as_sound.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#include "gameswf/gameswf_as_classes/as_sound.h"
#include "gameswf/gameswf_log.h"
#include "gameswf/gameswf_sound.h"
#include "gameswf/gameswf_movie_def.h"

namespace gameswf
{

	void	sound_start(const fn_call& fn)
	{
		sound_handler* s = get_sound_handler();
		if (s != NULL)
		{
			as_sound*	snd = cast_to<as_sound>(fn.this_ptr);
			if (snd)
			{
				int offset = 0;
				int loops = 0;
				if (fn.nargs >= 2)
				{
					offset = fn.arg(0).to_int();
					loops = fn.arg(1).to_int();
				}
				s->play_sound(snd, snd->m_id, loops);
			}
		}
	}


	void	sound_stop(const fn_call& fn)
	{
		sound_handler* s = get_sound_handler();
		if (s != NULL)
		{
			as_sound*	snd = cast_to<as_sound>(fn.this_ptr);
			assert(snd);
			s->stop_sound(snd->m_id);
		}
	}

	void	sound_attach(const fn_call& fn)
	{
		if (fn.nargs < 1)
		{
			log_error("attach sound needs one argument\n");
			return;
		}

		as_sound*	snd = cast_to<as_sound>(fn.this_ptr);
		assert(snd);

		assert(fn.env);

		// find target movieclip
		character* target = snd->m_target.get_ptr();
		if (target == NULL)
		{
			// try to find a sound from the caller target
			target = fn.env->get_target();
		}
			
		// find resource
		character_def* res = NULL;
		if (target)
		{
			res = target->find_exported_resource(fn.arg(0).to_string());
		}

		if (res == NULL)
		{
			log_error("can't find sound '%s'\n", fn.arg(0).to_string());
			return;
		}

		int si = 0;
		sound_sample* ss = cast_to<sound_sample>(res);

		if (ss != NULL)
		{
			si = ss->m_sound_handler_id;
		}
		else
		{
			log_error("sound sample is NULL\n");
			return;
		}

		snd->clear();

		// sanity check
		if (si >= 0 && si < 10000)
		{
			snd->m_id = si;
			snd->m_is_loaded_sound = false;
		}
		else
		{
			log_error("invalid sound sample id: %d\n", si);
		}
	}

	void	sound_volume(const fn_call& fn)
	{
		if (fn.nargs < 1)
		{
			log_error("set volume of sound needs one argument\n");
			return;
		}
		
		int volume = fn.arg(0).to_int();

		// sanity check
		if (volume >= 0 && volume <= 100)
		{
			sound_handler* s = get_sound_handler();
			if (s != NULL)
			{
				as_sound*	snd = cast_to<as_sound>(fn.this_ptr);
				assert(snd);
				s->set_volume(snd->m_id, volume);
			}
		}
	}

	// public loadSound(url:String, isStreaming:Boolean) : Void
	void	sound_load(const fn_call& fn)
	{
		if (fn.nargs > 1)
		{
			sound_handler* s = get_sound_handler();
			if (s != NULL)
			{
				as_sound*	snd = cast_to<as_sound>(fn.this_ptr);
				assert(snd);

				tu_string full_url = get_full_url(fn.get_player()->get_workdir(), 
					fn.arg(0).to_string());
				int id = s->load_sound(full_url.c_str());
				if (id >= 0)
				{
					snd->clear();
					snd->m_id = id;
					snd->m_is_loaded_sound = true;
				}
			}
		}
	}

	// The number of milliseconds a sound has been playing. 
	// If the sound is looped, the position is reset to 0 at the beginning of each loop.
	void	get_position(const fn_call& fn)
	{
		sound_handler* s = get_sound_handler();
		if (s != NULL)
		{
			as_sound*	snd = cast_to<as_sound>(fn.this_ptr);
			assert(snd);

			int ms = s->get_position(snd->m_id);
			fn.result->set_int(ms);
		}
	}

	// Sound([target:Object])
	//  Creates a new Sound object for a specified movie clip.
	void	as_global_sound_ctor(const fn_call& fn)
	{
		gc_ptr<as_sound> snd = new as_sound(fn.get_player());

		snd->m_target = fn.env->get_target();	// initial value
		if (fn.nargs > 0)
		{
			assert(fn.env);
			snd->m_target = cast_to<character>(fn.env->find_target(fn.arg(0)));
		}

		// methods
		snd->builtin_member("attachSound", sound_attach);
		snd->builtin_member("start", sound_start);
		snd->builtin_member("stop", sound_stop);
		snd->builtin_member("setVolume", sound_volume);
		snd->builtin_member("loadSound", sound_load);
		snd->builtin_member("position", as_value(get_position, as_value()));

		fn.result->set_as_object(snd.get_ptr());
	}

}
