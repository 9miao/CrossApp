// gameswf_button.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// SWF buttons.  Mouse-sensitive update/display, actions, etc.


#ifndef GAMESWF_BUTTON_H
#define GAMESWF_BUTTON_H


#include "gameswf_impl.h"
#include "gameswf_sound.h"


namespace gameswf
{
	//
	// button characters
	//
	enum mouse_state
	{
		MOUSE_UP,
		MOUSE_DOWN,
		MOUSE_OVER
	};

	struct button_record
	{
		bool	m_has_blend_mode;
		bool	m_has_filter_list;
		bool	m_hit_test;
		bool	m_down;
		bool	m_over;
		bool	m_up;
		int	m_character_id;
		character_def*	m_character_def;
		int	m_button_layer;
		matrix	m_button_matrix;
		cxform	m_button_cxform;
		int	m_blend_mode;

		bool	read(stream* in, int tag_type, movie_definition_sub* m);
	};
	

	struct button_action
	{
		enum condition
		{
			IDLE_TO_OVER_UP = 1 << 0,
			OVER_UP_TO_IDLE = 1 << 1,
			OVER_UP_TO_OVER_DOWN = 1 << 2,
			OVER_DOWN_TO_OVER_UP = 1 << 3,
			OVER_DOWN_TO_OUT_DOWN = 1 << 4,
			OUT_DOWN_TO_OVER_DOWN = 1 << 5,
			OUT_DOWN_TO_IDLE = 1 << 6,
			IDLE_TO_OVER_DOWN = 1 << 7,
			OVER_DOWN_TO_IDLE = 1 << 8,
		};
		int	m_conditions;
		swf_array<action_buffer*>	m_actions;

		~button_action();
		void	read(stream* in, int tag_type);
	};


	struct button_character_definition : public character_def
	{
		struct sound_info
		{
			void read(stream* in);

			bool m_no_multiple;
			bool m_stop_playback;
			bool m_has_envelope;
			bool m_has_loops;
			bool m_has_out_point;
			bool m_has_in_point;
			Uint32 m_in_point;
			Uint32 m_out_point;
			Uint16 m_loop_count;
			swf_array<sound_envelope> m_envelopes;
		};

		struct button_sound_info
		{
			Uint16 m_sound_id;
			sound_sample*	m_sam;
			sound_info m_sound_style;
		};

		struct button_sound_def
		{
			void	read(stream* in, movie_definition_sub* m);
			button_sound_info m_button_sounds[4];
		};


		bool m_menu;
		swf_array<button_record>	m_button_records;
		swf_array<button_action>	m_button_actions;
		button_sound_def*	m_sound;

		button_character_definition(player* player);
		virtual ~button_character_definition();
		character*	create_character_instance(character* parent, int id);
		void	read(stream* in, int tag_type, movie_definition_sub* m);
	};

};	// end namespace gameswf


#endif // GAMESWF_BUTTON_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
