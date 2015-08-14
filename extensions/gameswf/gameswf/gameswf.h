// gameswf.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// SWF (Shockwave Flash) player library.  The info for this came from
// http://www.openswf.org, the flashsource project, and swfparse.cpp

//  Modify by chenee (chenee543216@gmail.com) on 13-05-20
//  @chenee: 1\change Objective-C to C++ ;2\change opengl es 1.1 to opengl es 2.0
//


#ifndef GAMESWF_H
#define GAMESWF_H

// for memory leaks debugging
#if _WIN32 && _DEBUG && !USE_STACKWALKER
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include <ctype.h>	// for poxy wchar_t
#include <assert.h>
#include "../base/image.h"	// for delete m_suspended_image
#include "../base/container.h"	// for hash<...>
#include "../base/tu_gc.h"
#include "../base/tu_gc_singlethreaded_refcount.h"
#include "../base/weak_ptr.h"

class tu_file;
class render_handler;

// @@ forward decl to avoid including base/image.h; TODO change the
// render_handler interface to not depend on these structs at all.
namespace image { struct rgb; struct rgba; struct image_base; }

// forward decl
namespace jpeg { struct input; }
class tu_string;
class tu_stringi;

// Forward decl for D3D render handlers, in case they are
// instantiated.  Harmless on non-D3D platforms.
struct IDirect3DDevice9;
struct IDirect3DDevice8;

namespace gameswf
{
	DECLARE_GC_TYPES(tu_gc::singlethreaded_refcount);
	SPECIALIZE_GC_CONTAINER(gc_array, swf_array);

	// Forward declarations.
	struct player;
	struct as_value;
	struct bitmap_info;
	struct character;
	struct execute_tag;
	struct font;
	struct render_handler;
	struct rgba;
	struct sound_handler;
	struct stream;
	struct video_handler;
	struct event_id;
	struct root;
	struct movie_def_impl;
	struct rect;
	struct as_environment;
	struct character_def;
	struct sound_sample;
	struct video_stream_definition;
	struct sprite_definition;
	struct as_s_function;
	struct as_object;
	struct movie_definition;

	//
	// Log & error reporting control.
	//

	// Supply a function pointer to receive log & error messages.
	exported_module void	register_log_callback(void (*callback)(bool error, const char* message));

	// Control verbosity of specific categories.
	exported_module bool get_verbose_parse();
	exported_module bool get_verbose_debug();
	exported_module bool get_verbose_action();
	exported_module void	set_verbose_action(bool verbose);
	exported_module void	set_verbose_parse(bool verbose);

	// Get and set the render handler.  This is one of the first
	// things you should do to initialise the player (assuming you
	// want to display anything).
	exported_module void	set_render_handler(render_handler* s);

	// Pass in a sound handler, so you can handle audio on behalf of
	// gameswf.  This is optional; if you don't set a handler, or set
	// NULL, then sounds won't be played.
	//
	// If you want sound support, you should set this at startup,
	// before loading or playing any movies!
	exported_module void	set_sound_handler(sound_handler* s);

	// You probably don't need this. (@@ make it private?)
	exported_module sound_handler*	get_sound_handler();

	// Register a callback to the host, for providing a file,
	// given a "URL" (i.e. a path name).  This is the only means
	// by which the gameswf library accesses file data, for
	// loading movies, cache files, and so on.
	//
	// gameswf will call this when it needs to open a file.
	//
	// NOTE: the returned tu_file* will be delete'd by gameswf
	// when it is done using it.  Your file_opener_function may
	// return NULL in case the requested file can't be opened.
	typedef tu_file* (*file_opener_callback)(const char* url_or_path);
	exported_module void	register_file_opener_callback(file_opener_callback opener);

	// ActionScripts embedded in a movie can use the built-in
	// fscommand() function to send data back to the host
	// application.	 If you are interested in this data, register
	// a handler, which will be called when the embedded scripts
	// call fscommand().
	//
	// The handler gets the character* that the script is
	// embedded in, and the two string arguments passed by the
	// script to fscommand().
	typedef void (*fscommand_callback)(character* movie, const char* command, const char* arg);
	exported_module void	register_fscommand_callback(fscommand_callback handler);

	// Use this to control how finely curves are subdivided.  1.0
	// is the default; it's a pretty good value.  Larger values
	// result in coarser, more angular curves with fewer vertices.
	exported_module void	set_curve_max_pixel_error(float pixel_error);
	exported_module float	get_curve_max_pixel_error();

	// Some helpers that may or may not be compiled into your
	// version of the library, depending on platform etc.
	exported_module render_handler*	create_render_handler_xbox();
	exported_module render_handler*	create_render_handler_ogl();
	exported_module render_handler*	create_render_handler_ogles();
	exported_module render_handler* create_render_handler_d3d(IDirect3DDevice9* _pDevice);
	exported_module render_handler* create_render_handler_d3d(IDirect3DDevice8* _pDevice);
#ifdef TU_USE_SDL
	exported_module sound_handler*	create_sound_handler_sdl();
#endif
	exported_module sound_handler*	create_sound_handler_openal();


	// For things that should be automatically garbage-collected.
	struct ref_counted : public gc_object, public weak_pointee_mixin
	{
	};

	// Unique id of all gameswf resources
	enum as_classes
	{
		AS_OBJECT,
		AS_CHARACTER,
		AS_SPRITE,
		AS_FUNCTION,
		AS_C_FUNCTION,
		AS_S_FUNCTION,
		AS_3_FUNCTION,	// action script 3 function
		AS_MOVIE_DEF,
		AS_MOVIE_DEF_SUB,
		AS_CHARACTER_DEF,
		AS_SPRITE_DEF,
		AS_VIDEO_DEF,
		AS_SOUND_SAMPLE,
		AS_VIDEO_INST,
		AS_KEY,
		AS_ARRAY,
		AS_COLOR,
		AS_SOUND,
		AS_FONT,
		AS_CANVAS,
		AS_NETSTREAM,
		AS_STRING,
		AS_SELECTION,
		AS_POINT,
		AS_MATRIX,
		AS_TRANSFORM,
		AS_COLOR_TRANSFORM,
		AS_NETCONNECTION,
		AS_LISTENER,
		AS_DATE,
		AS_EDIT_TEXT,
		AS_XML_SOCKET,
		AS_TEXTFORMAT,
		AS_MCLOADER,
		AS_LOADVARS,
		AS_TIMER,
		AS_MOUSE,

		// flash9
		AS_EVENT,
		AS_MOUSE_EVENT,

		// plugins
		AS_PLUGIN_MYDB,
		AS_PLUGIN_MYTABLE,
		AS_PLUGIN_3DS,
		AS_PLUGIN_FILE,
		AS_PLUGIN_SQLITE_DB,
		AS_PLUGIN_SQLITE_TABLE,
		AS_PLUGIN_SQL_DB,
		AS_PLUGIN_SQL_TABLE,

		// user defined plugins
		// should be the last in this enum
		AS_USER_PLUGIN = 1000

	};

	// This is the base class for all ActionScript-able objects
	// ("as_" stands for ActionScript).
	struct as_object_interface : public ref_counted
	{
		virtual bool is(int class_id) const = 0;
	};

	// cast_to<gameswf object>(obj) implementation (from Julien Hamaide)
	template <typename cast_class>
	cast_class* cast_to(as_object_interface* object)
	{
		if (object)
		{
			return object->is(cast_class::m_class_id) ? static_cast<cast_class*>(object) : 0;
		}
		return 0;
	}

	template <typename cast_class>
	const cast_class* cast_to(const as_object_interface* object)
	{
		if (object)
		{
			return object->is(cast_class::m_class_id) ? static_cast<const cast_class*>(object) : 0;
		}
		return 0;
	}

	// For caching precomputed stuff.  Generally of
	// interest to gameswf_processor and programs like it.
	struct cache_options
	{
		bool	m_include_font_bitmaps;

		cache_options()
			:
			m_include_font_bitmaps(true)
		{
		}
	};


	// Keyboard handling
	namespace key
	{
		enum code
		{
			INVALID = 0,
			A = 65,
			B,
			C,
			D,
			E,
			F,
			G,
			H,
			I,
			J,
			K,
			L,
			M,
			N,
			O,
			P,
			Q,
			R,
			S,
			T,
			U,
			V,
			W,
			X,
			Y,
			Z,
			_0 = 48,
			_1,
			_2,
			_3,
			_4,
			_5,
			_6,
			_7,
			_8,
			_9,
			KP_0 = 96,
			KP_1,
			KP_2,
			KP_3,
			KP_4,
			KP_5,
			KP_6,
			KP_7,
			KP_8,
			KP_9,
			KP_MULTIPLY,
			KP_ADD,
			KP_ENTER,
			KP_SUBTRACT,
			KP_DECIMAL,
			KP_DIVIDE,
			F1 = 112,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
			F13,
			F14,
			F15,
			BACKSPACE = 8,
			TAB,
			CLEAR = 12,
			ENTER,
			SHIFT = 16,
			CONTROL,
			ALT,
			CAPSLOCK = 20,
			ESCAPE = 27,
			SPACE = 32,
			PGDN,
			PGUP,
			END = 35,
			HOME,
			LEFT,
			UP,
			RIGHT,
			DOWN,
			INSERT = 45,
			DELETEKEY,
			HELP,
			NUM_LOCK = 144,
			SEMICOLON = 186,
			EQUALS = 187,
			COMMA = 188,
			MINUS = 189,
			PERIOD = 190,
			SLASH = 191,
			BACKTICK = 192,
			LEFT_BRACKET = 219,
			BACKSLASH = 220,
			RIGHT_BRACKET = 221,
			QUOTE = 222,

			KEYCOUNT
		};
	}	// end namespace key

	// Try to grab movie info from the header of the given .swf
	// file.
	//
	// Sets *version to 0 if info can't be extracted.
	//
	// You can pass NULL for any entries you're not interested in.
	void	get_movie_info(
		const char*	filename,
		int*		version,
		int*		width,
		int*		height,
		float*		frames_per_second,
		int*		frame_count,
		int*		tag_count);

	// Enable/disable attempts to read cache files (.gsc) when
	// loading movies.
	exported_module void	set_use_cache_files(bool use_cache);

	//
	// Use DO_NOT_LOAD_BITMAPS if you have pre-processed bitmaps
	// stored externally somewhere, and you plan to install them
	// via get_bitmap_info()->...
	enum create_bitmaps_flag
	{
		DO_LOAD_BITMAPS,
		DO_NOT_LOAD_BITMAPS
	};
	// Use DO_NOT_LOAD_FONT_SHAPES if you know you have
	// precomputed texture glyphs (in cached data) and you know
	// you always want to render text using texture glyphs.
	enum create_font_shapes_flag
	{
		DO_LOAD_FONT_SHAPES,
		DO_NOT_LOAD_FONT_SHAPES
	};

	// unused
//	movie_definition*	create_movie_no_recurse(
//		tu_file*		input_stream,
//		create_bitmaps_flag	cbf,
//		create_font_shapes_flag cfs);

	// Helper to pregenerate cached data (basically, shape
	// tesselations).  Does this by running through each frame of
	// the movie and displaying the shapes with a null renderer.
	// The pregenerated data is stored in the movie_definition
	// object itself, and is included with the cached data written
	// by movie_definition::output_cached_data().
	//
	// Note that this tesselates shapes to the resolution they
	// explicitly appear in the linear frames of the movie.	 Does
	// not try very hard to run your ActionScript to account for
	// dynamic scaling (that's more or less futile anyway due to
	// the halting problem).
	void	precompute_cached_data(movie_definition* movie_def);

	//
	// Library management
	//

	// Release any library movies we've cached.  Do this when you want
	// maximum cleanup.
	void	clear_library();

	//
	// Sound callback handler.
	//

	// You may define a subclass of this, and pass an instance to
	// set_sound_handler().
	struct sound_handler
	{
		// audio for video
		typedef void (*aux_streamer_ptr)(as_object* netstream, unsigned char* stream, int len);

		enum format_type
		{
			FORMAT_RAW = 0,		// unspecified format.	Useful for 8-bit sounds???
			FORMAT_ADPCM = 1,	// gameswf doesn't pass this through; it uncompresses and sends FORMAT_NATIVE16
			FORMAT_MP3 = 2,
			FORMAT_UNCOMPRESSED = 3,	// 16 bits/sample, little-endian
			FORMAT_NELLYMOSER = 6,	// Mystery proprietary format; see nellymoser.com

			// gameswf tries to convert data to this format when possible:
			FORMAT_NATIVE16 = 7	// gameswf extension: 16 bits/sample, native-endian
		};

		// If stereo is true, samples are interleaved w/ left sample first.

		// gameswf calls at load-time with sound data, to be
		// played later.  You should create a sample with the
		// data, and return a handle that can be used to play
		// it later.  If the data is in a format you can't
		// deal with, then you can return 0 (for example), and
		// then ignore 0's in play_sound() and delete_sound().
		//
		// Assign handles however you like.
		virtual int	create_sound(
			void*		data,
			int		data_bytes,
			int		sample_count,
			format_type	format,
			int		sample_rate,	/* one of 5512, 11025, 22050, 44100 */
			bool		stereo) = 0;

		// loads external sound file
		virtual int	load_sound(const char* url) = 0;

		virtual void append_sound(int sound_handle, void* data, int data_bytes) = 0;

		// gameswf calls this when it wants you to play the defined sound.
		//
		// loop_count == 0 means play the sound once (1 means play it twice, etc)
		virtual void	play_sound(as_object* listener_obj, int sound_handle, int loop_count /* , volume, pan, etc? */) = 0;

		virtual void	set_volume(int sound_handle, int volume) = 0;

		// set max volume in percent, [0..100]
		virtual void	set_max_volume(int vol) = 0;

		// Stop the specified sound if it's playing.
		// (Normally a full-featured sound API would take a
		// handle specifying the *instance* of a playing
		// sample, but SWF is not expressive that way.)
		virtual void	stop_sound(int sound_handle) = 0;
		virtual void	stop_all_sounds() = 0;

		// gameswf calls this when it's done with a particular sound.
		virtual void	delete_sound(int sound_handle) = 0;

		// They are called from netstream when it wishes to sound video
		virtual void attach_aux_streamer(aux_streamer_ptr ptr, as_object* netstream) {};
		virtual void detach_aux_streamer(as_object* netstream) {};
		virtual void cvt(short int** adjusted_data, int* adjusted_size, unsigned char* data,
				 int size, int channels, int freq) {};

		virtual ~sound_handler() {};
		virtual bool is_open() { return false; };
		virtual void pause(int sound_handle, bool paused) {};

		// The number of milliseconds a sound has been playing. 
		// If the sound is looped, the position is reset to 0 at the beginning of each loop.
		virtual int get_position(int sound_handle) { return 0; };

		// openAL sound handler uses this function to handle sound events
		virtual void advance(float delta_time) {};
	};

	struct matrix; 
	struct rect; 

	// base class for video handler
	struct video_handler : public ref_counted
	{
		video_handler() :
			m_clear_background(false)
		{
		}

		virtual void display(Uint8* data, int width, int height, 
			const matrix* mat, const rect* bounds, const rgba& color) = 0;

		void clear_background(bool clear) { m_clear_background = clear; }

	protected:

		bool m_clear_background;
	};

	// tu_float is used in matrix & cxform because
	// Flash converts inf to zero when works with matrix & cxform
	struct tu_float
	{
		tu_float()	{ m_float = 0.0f; }
		tu_float(float x)	{ operator=(x); }
		tu_float(double x)	{ operator=((float) x); }

		operator float() const { return m_float; }
		inline void	operator=(float x)
		{
			m_float = x >= -3.402823466e+38F && x <= 3.402823466e+38F ? x : 0.0f;
		}
		void	operator+=(const float x) { operator=(m_float + x); }
		void	operator-=(const float x) { operator=(m_float - x); }
		void	operator*=(const float x) { operator=(m_float * x); }
		void	operator/=(const float x) { operator=(m_float / x); }

	private:
		float m_float;
	};



	//
	// matrix type, used by render handler
	//

	struct point;
	struct matrix
	{
		tu_float	m_[2][3];

		static matrix	identity;

		exported_module matrix();
		exported_module void	set_identity();
		void	concatenate(const matrix& m);
		void	concatenate_translation(float tx, float ty);
		void	concatenate_scale(float s);
		void	set_lerp(const matrix& m1, const matrix& m2, float t);
		void	set_scale_rotation(float x_scale, float y_scale, float rotation);
		void	read(stream* in);
		void	print() const;
		void	transform(point* result, const point& p) const;
		void	transform(rect* bound) const;
		void	transform_vector(point* result, const point& p) const;
		void	transform_by_inverse(point* result, const point& p) const;
		void	transform_by_inverse(rect* bound) const;
		void	set_inverse(const matrix& m);
		bool	does_flip() const;	// return true if we flip handedness
		float	get_determinant() const;	// determinant of the 2x2 rotation/scale part only
		float	get_max_scale() const;	// return the maximum scale factor that this transform applies
		float	get_x_scale() const;	// return the magnitude scale of our x coord output
		float	get_y_scale() const;	// return the magnitude scale of our y coord output
		float	get_rotation() const;	// return our rotation component (in radians)

		bool operator==(const matrix& m) const
		{
			return 
				m_[0][0] == m.m_[0][0] &&
				m_[0][1] == m.m_[0][1] &&
				m_[0][2] == m.m_[0][2] &&
				m_[1][0] == m.m_[1][0] &&
				m_[1][1] == m.m_[1][1] &&
				m_[1][2] == m.m_[1][2];
		}

		bool operator!=(const matrix& m) const
		{
			return ! (*this == m);
		}

	};


	//
	// point: used by rect which is used by render_handler (otherwise would be in internal gameswf_types.h)
	//


	struct point
	{
		float	m_x, m_y;

		point() : m_x(0), m_y(0) {}
		point(float x, float y) : m_x(x), m_y(y) {}

		void	set_lerp(const point& a, const point& b, float t)
		// Set to a + (b - a) * t
		{
			m_x = a.m_x + (b.m_x - a.m_x) * t;
			m_y = a.m_y + (b.m_y - a.m_y) * t;
		}

		bool operator==(const point& p) const { return m_x == p.m_x && m_y == p.m_y; }

		bool	bitwise_equal(const point& p) const;
		
		float get_length() const;

		void twips_to_pixels();
		void pixels_to_twips();
	};


	//
	// rect: rectangle type, used by render handler
	//


	struct rect
	{
		float	m_x_min, m_x_max, m_y_min, m_y_max;

		rect() :
			m_x_min(0.0f),
			m_x_max(0.0f),
			m_y_min(0.0f),
			m_y_max(0.0f)
		{
		}

		void	read(stream* in);
		void	print() const;
		bool	point_test(float x, float y) const;
		bool	bound_test(const rect& bound) const;
		void	set_to_point(float x, float y);
		void	set_to_point(const point& p);
		void	expand_to_point(float x, float y);
		void	expand_to_point(const point& p);
		void	expand_to_rect(const rect& r);
		float	width() const { return m_x_max - m_x_min; }
		float	height() const { return m_y_max - m_y_min; }

		point	get_corner(int i) const;

		void	enclose_transformed_rect(const matrix& m, const rect& r);

		void	set_lerp(const rect& a, const rect& b, float t);

		void twips_to_pixels();
		void pixels_to_twips();
	};


	//
	// cxform: color transform type, used by render handler
	//
	struct cxform
	{
		tu_float	m_[4][2];	// [RGBA][mult, add]

		cxform();
		void	concatenate(const cxform& c);
		rgba	transform(const rgba in) const;
		void	read_rgb(stream* in);
		void	read_rgba(stream* in);
		void	clamp();  // Force component values to be in range.
		void	print() const;

		static cxform	identity;
	};


	//
	// texture and render callback handler.
	//

	// Your render_handler creates bitmap_info's for gameswf.  You
	// need to subclass bitmap_info in order to add the
	// information and functionality your app needs to render
	// using textures.
	struct bitmap_info : public ref_counted
	{
		virtual void layout(unsigned int mProgram) {};
		virtual void activate() {};
		virtual int get_width() const { return 0; }
		virtual int get_height() const { return 0; }
		virtual unsigned char* get_data() const { return 0; }
		virtual int get_bpp() const { return 0; }	// byte per pixel
	};

	// You must define a subclass of render_handler, and pass an
	// instance to set_render_handler().
	struct render_handler
	{
		virtual ~render_handler() {}

		// Your handler should return these with a ref-count of 0.  (@@ is that the right policy?)
		virtual bitmap_info*	create_bitmap_info_empty() = 0;	// used when DO_NOT_LOAD_BITMAPS is set
		virtual bitmap_info*	create_bitmap_info_alpha(int w, int h, unsigned char* data) = 0;
		virtual bitmap_info*	create_bitmap_info_rgb(image::rgb* im) = 0;
		virtual bitmap_info*	create_bitmap_info_rgba(image::rgba* im) = 0;
		virtual video_handler*	create_video_handler() = 0;

		// Bracket the displaying of a frame from a movie.
		// Fill the background color, and set up default
		// transforms, etc.
		virtual void	begin_display(float*mat,
			rgba background_color,
			int viewport_x0, int viewport_y0,
			int viewport_width, int viewport_height,
			float x0, float x1, float y0, float y1) = 0;
		virtual void	end_display() = 0;

		// Geometric and color transforms for mesh and line_strip rendering.
		virtual void	set_matrix(const matrix& m) = 0;
		virtual void	set_cxform(const cxform& cx) = 0;

		// Draw triangles using the current fill-style 0.
		// Clears the style list after rendering.
		//
		// coords is a list of (x,y) coordinate pairs, in
		// triangle-strip order.  The type of the array should
		// be Sint16[vertex_count*2]
		virtual void	draw_mesh_strip(const void* coords, int vertex_count) = 0;
		// As above, but coords is in triangle list order.
		virtual void	draw_triangle_list(const void* coords, int vertex_count) = 0;

		// Draw a line-strip using the current line style.
		// Clear the style list after rendering.
		//
		// Coords is a list of (x,y) coordinate pairs, in
		// sequence.  Each coord is a 16-bit signed integer.
		virtual void	draw_line_strip(const void* coords, int vertex_count) = 0;

		// Set line and fill styles for mesh & line_strip
		// rendering.
		enum bitmap_wrap_mode
		{
			WRAP_REPEAT,
			WRAP_CLAMP
		};

		enum bitmap_blend_mode
		{
			BLEND_NORMAL,
			BLEND_LAYER,
			BLEND_MULTIPLY,
			BLEND_SCREEN,
			BLEND_LIGHTEN,
			BLEND_DARKEN,
			BLEND_DIFFERENCE,
			BLEND_ADD,
			BLEND_SUBTRACT,
			BLEND_INVERT,
			BLEND_ALPHA,
			BLEND_ERASE,
			BLEND_OVERLAY,
			BLEND_HARDLIGHT
		};

		virtual void	fill_style_disable(int fill_side) = 0;
		virtual void	fill_style_color(int fill_side, const rgba& color) = 0;
		virtual void	fill_style_bitmap(int fill_side, bitmap_info* bi, const matrix& m,
							bitmap_wrap_mode wm, bitmap_blend_mode bm) = 0;

		virtual void	line_style_disable() = 0;
		virtual void	line_style_color(rgba color) = 0;
		virtual void	line_style_width(float width) = 0;

		// Special function to draw a rectangular bitmap;
		// intended for textured glyph rendering.  Ignores
		// current transforms.
		virtual void	draw_bitmap(
			const matrix&		m,
			bitmap_info*	bi,
			const rect&		coords,
			const rect&		uv_coords,
			rgba			color) = 0;
		virtual void	set_antialiased(bool enable) = 0;

		virtual bool test_stencil_buffer(const rect& bound, Uint8 pattern) = 0;
		virtual void begin_submit_mask() = 0;
		virtual void end_submit_mask() = 0;
		virtual void disable_mask() = 0;

		// Mouse cursor handling.
		enum cursor_type
		{
			SYSTEM_CURSOR,
			ACTIVE_CURSOR,
			VISIBLE_CURSOR,			// show cursor
			INVISIBLE_CURSOR		// hide cursor
		};
		virtual void set_cursor(cursor_type cursor) {}
		virtual bool is_visible(const rect& bound) = 0;
		virtual void open() = 0;
	};

	// Some optional helpers.
	namespace tools
	{
		struct process_options
		{
			bool	m_zip_whole_file;	// @@ not implemented yet (low priority?)
			bool	m_remove_image_data;	// removes existing image data; leaves minimal placeholder tags
			bool	m_remove_font_glyph_shapes;

			process_options()
				:
				m_zip_whole_file(false),
				m_remove_image_data(false),
				m_remove_font_glyph_shapes(false)
			{}
		};

		// Copy tags from *in to *out, applying the given
		// options.  *in should be a SWF-format stream.	 The
		// output will be a SWF-format stream.
		//
		// Returns 0 on success, or a non-zero error-code on
		// failure.
		int	process_swf(tu_file* swf_out, tu_file* swf_in, const process_options& options);
	}

	// helper
	struct glyph_entity : public ref_counted
	{
		gc_ptr<bitmap_info> m_bi;
		float m_advance;
		rect m_bounds;
	};

	struct glyph_provider : public ref_counted
	{
		glyph_provider() {}
		virtual ~glyph_provider() {}
		
		virtual bitmap_info* get_char_image(character_def* shape_glyph, Uint16 code, 
			const tu_string& fontname, bool is_bold, bool is_italic, int fontsize,
			rect* bounds, float* advance) = 0;

	};

	exported_module glyph_provider*	get_glyph_provider();
	exported_module void	set_glyph_provider(glyph_provider* gp);
	exported_module glyph_provider*	create_glyph_provider_freetype();
	exported_module glyph_provider*	create_glyph_provider_tu();

}	// namespace gameswf


#endif // GAMESWF_H


// Local Variables:
// mode: C++
// c-basic-offset: 8
// tab-width: 8
// indent-tabs-mode: t
// End:
