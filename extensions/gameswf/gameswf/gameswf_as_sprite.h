// gameswf_as_sprite.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Some implementation for SWF player.

// Useful links:
//
// http://sswf.sourceforge.net/SWFalexref.html
// http://www.openswf.org

#ifndef GAMESWF_AS_SPRITE_H
#define GAMESWF_AS_SPRITE_H

namespace gameswf
{

	//
	// sprite built-in ActionScript methods
	//

	void	as_global_movieclip_ctor(const fn_call& fn);
	void	sprite_hit_test(const fn_call& fn);
	void	sprite_start_drag(const fn_call& fn);
	void	sprite_stop_drag(const fn_call& fn);
	void	sprite_play(const fn_call& fn);
	void	sprite_stop(const fn_call& fn);
	void	sprite_goto_and_play(const fn_call& fn);
	void	sprite_goto_and_stop(const fn_call& fn);
	void	sprite_next_frame(const fn_call& fn);
	void	sprite_prev_frame(const fn_call& fn);
	void	sprite_get_bytes_loaded(const fn_call& fn);
	void	sprite_get_bytes_total(const fn_call& fn);
	void sprite_swap_depths(const fn_call& fn) ;
	void sprite_duplicate_movieclip(const fn_call& fn);
	void sprite_get_depth(const fn_call& fn);
	void sprite_create_empty_movieclip(const fn_call& fn);
	void sprite_remove_movieclip(const fn_call& fn);
	void sprite_loadmovie(const fn_call& fn);
	void sprite_unloadmovie(const fn_call& fn);
	void sprite_getnexthighestdepth(const fn_call& fn);
	void sprite_create_text_field(const fn_call& fn);
	void sprite_attach_movie(const fn_call& fn);
	void sprite_local_global(const fn_call& fn);
	void sprite_global_local(const fn_call& fn);
	void sprite_get_bounds(const fn_call& fn);
	void sprite_get_rect(const fn_call& fn);
	void sprite_set_mask(const fn_call& fn);

	// drawing API
	void sprite_begin_fill(const fn_call& fn);
	void sprite_end_fill(const fn_call& fn);
	void sprite_clear(const fn_call& fn);
	void sprite_move_to(const fn_call& fn);
	void sprite_line_to(const fn_call& fn);
	void sprite_curve_to(const fn_call& fn);
	void sprite_line_style(const fn_call& fn);

	// gameSWF extension
	void sprite_set_fps(const fn_call& fn);
	void sprite_get_play_state(const fn_call& fn);

	// flash9
	void sprite_add_script(const fn_call& fn);

}

#endif
