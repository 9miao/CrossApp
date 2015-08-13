// gameswf_sound.h   -- Thatcher Ulrich, Vitaly Alexeev

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#ifndef GAMESWF_SOUND_H
#define GAMESWF_SOUND_H


#include "gameswf_impl.h"


namespace gameswf
{
	int get_sample_rate(int index);

	struct sound_envelope
	{
		Uint32 m_mark44;
		Uint16 m_level0;
		Uint16 m_level1;
	};

	struct sound_sample : public character_def
	{
		// Unique id of a gameswf resource
		enum { m_class_id = AS_SOUND_SAMPLE };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return character_def::is(class_id);
		}

		int	m_sound_handler_id;

		sound_sample(player* player, int id) :
			character_def(player),
			m_sound_handler_id(id)
		{
		}

		virtual ~sound_sample();
	};
}


#endif // GAMESWF_SOUND_H
