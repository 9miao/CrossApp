// gameswf_video_impl.h	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.


#ifndef GAMESWF_VIDEO_H
#define GAMESWF_VIDEO_H

#include "gameswf_impl.h"
#include "gameswf_as_classes/as_netstream.h"

namespace gameswf
{
	struct video_stream_definition : public character_def
	{
		// Unique id of a gameswf resource
		enum { m_class_id = AS_VIDEO_DEF };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return character_def::is(class_id);
		}

		video_stream_definition(player* player) :
			character_def(player)
		{
		}
		//	virtual ~video_stream_definition();


		character* create_character_instance(character* parent, int id);
		void	read(stream* in, int tag, movie_definition* m);
		virtual void get_bound(rect* bound);

		Uint16 m_width;
		Uint16 m_height;

	private:

		//	uint8_t reserved_flags;
		Uint8 m_deblocking_flags;
		bool m_smoothing_flags;

		// 0: extern file
		// 2: H.263
		// 3: screen video (Flash 7+ only)
		// 4: VP6
		Uint8 m_codec_id;
		array<void*>	m_frames;
	};

	struct video_stream_instance : public character
	{
		// Unique id of a gameswf resource
		enum { m_class_id = AS_VIDEO_INST };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return character::is(class_id);
		}

		video_stream_instance(player* player, video_stream_definition* def,	character* parent, int id);
		~video_stream_instance();

		void	display();
		virtual character_def* get_character_def() { return m_def.get_ptr();	}

		//
		// ActionScript overrides
		//

		// To drop the connection to the Video object, pass null for source.
		void attach_netstream(as_netstream* ns)
		{
			m_ns = ns;
		}

		gc_ptr<video_handler> m_video_handler;

		private:

		gc_ptr<video_stream_definition>	m_def;

		// A Camera object that is capturing video data or a NetStream object.
		gc_ptr<as_netstream> m_ns;
	};

}	// end namespace gameswf


#endif // GAMESWF_VIDEO_H
