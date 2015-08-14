// gameswf_video_impl.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// video implementation

#include "gameswf_video_impl.h"
#include "gameswf_stream.h"
#include "gameswf_as_classes/as_netstream.h"

namespace gameswf
{
	void video_stream_definition::get_bound(rect* bound)
	{
		bound->m_x_min = 0;
		bound->m_y_min = 0;
		bound->m_x_max = PIXELS_TO_TWIPS(m_width);
		bound->m_y_max = PIXELS_TO_TWIPS(m_height);
	}

	void video_stream_definition::read(stream* in, int tag, movie_definition* m)
	{
		// Character ID has been read already 
	
		assert(tag == 60 ||	tag == 61);
	
		if (tag == 60)
		{
	
			Uint16 numframes = in->read_u16();
			m_frames.resize(numframes);
	
			m_width = in->read_u16();
			m_height = in->read_u16();
			Uint8 reserved_flags = in->read_uint(5);
                        UNUSED(reserved_flags);
			m_deblocking_flags = in->read_uint(2);
			m_smoothing_flags = in->read_uint(1) ? true : false;
	
			m_codec_id = in->read_u8();
		}
		else if (tag == 61)
		{
			Uint16 n = in->read_u16();
			m_frames[n] = NULL;
		}
	
	}
	
	character* video_stream_definition::create_character_instance(character* parent, int id)
	{
		character* ch = new video_stream_instance(get_player(), this, parent, id);
		// instanciate_registered_class(ch);	//TODO: test it
		return ch;
	}

	void attach_video(const fn_call& fn)
	{
		video_stream_instance* video = cast_to<video_stream_instance>(fn.this_ptr);
		if (video && fn.nargs > 0)
		{
			// fn.arg(0) may be null
			video->attach_netstream(cast_to<as_netstream>(fn.arg(0).to_object()));
		}
	}

	void clear_video_background(const fn_call& fn)
	{
		video_stream_instance* video = cast_to<video_stream_instance>(fn.this_ptr);
		if (video && fn.nargs > 0)
		{
			assert(video->m_video_handler);
			video->m_video_handler->clear_background(fn.arg(0).to_bool());
		}
	}

	video_stream_instance::video_stream_instance(player* player, video_stream_definition* def, 
			character* parent, int id)	:
		character(player, parent, id),
		m_def(def)
	{
		assert(m_def != NULL);

		m_video_handler = render::create_video_handler();
		if (m_video_handler == NULL)
		{
			log_error("No available video render\n");
		}

		builtin_member("attachVideo", attach_video);

		// gameswf extension, set video background cleanup option, true/false
		builtin_member("clearBackgroundColor", as_value(as_value(), clear_video_background));
	}

	video_stream_instance::~video_stream_instance()
	{
	}

	void video_stream_instance::display()
	{
		if (m_ns != NULL && m_video_handler != NULL)	// is video attached ?
		{
			rect bounds;
			bounds.m_x_min = 0.0f;
			bounds.m_y_min = 0.0f;
			bounds.m_x_max = PIXELS_TO_TWIPS(m_def->m_width);
			bounds.m_y_max = PIXELS_TO_TWIPS(m_def->m_height);

			cxform cx = get_world_cxform();
			gameswf::rgba color = cx.transform(gameswf::rgba());

			matrix m = get_world_matrix();

			Uint8* video_data = m_ns->get_video_data();

			// video_data==NULL means that video is not updated and video_handler will draw the last video frame
			m_video_handler->display(video_data, m_ns->get_width(), m_ns->get_height(), 
							 &m, &bounds, color);

			if (video_data)
			{
				free(video_data);
			}
		}
	}


} // end of namespace gameswf
