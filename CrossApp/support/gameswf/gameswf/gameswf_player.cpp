// gameswf.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// here are collected all statics of gameswf library and also
// init_library and clear_library implementation

//  Modify by chenee (chenee543216@gmail.com) on 13-05-20
//  @chenee: 1\change Objective-C to C++ ;2\change opengl es 1.1 to opengl es 2.0
//

//#include "CrossApp.h"
#include "../base/tu_timer.h"
#include "../base/tu_file.h"
#include "../base/tu_random.h"
#include "gameswf_player.h"
#include "gameswf_object.h"
#include "gameswf_action.h"

// action script classes
#include "gameswf_as_sprite.h"
#include "gameswf_text.h"
#include "gameswf_as_classes/as_array.h"
#include "gameswf_as_classes/as_sound.h"
#include "gameswf_as_classes/as_key.h"
#include "gameswf_as_classes/as_math.h"
#include "gameswf_as_classes/as_mcloader.h"
#include "gameswf_as_classes/as_netstream.h"
#include "gameswf_as_classes/as_netconnection.h"
#include "gameswf_as_classes/as_textformat.h"
#include "gameswf_as_classes/as_string.h"
#include "gameswf_as_classes/as_color.h"
#include "gameswf_as_classes/as_date.h"
#include "gameswf_as_classes/as_xmlsocket.h"
#include "gameswf_as_classes/as_loadvars.h"
#include "gameswf_as_classes/as_flash.h"
#include "gameswf_as_classes/as_broadcaster.h"
#include "gameswf_as_classes/as_selection.h"
#include "gameswf_as_classes/as_number.h"
#include "gameswf_as_classes/as_boolean.h"
#include "gameswf_as_classes/as_global.h"
#include "gameswf_as_classes/as_sharedobject.h"
#include "gameswf_as_classes/as_mouse.h"

namespace gameswf
{

	void clears_tag_loaders();
    void set_s_registered(bool);

	void clear_disasm();

	//
	//	gameswf's statics
	//

	static glyph_provider* s_glyph_provider;
	void set_glyph_provider(glyph_provider* gp)
	{
		s_glyph_provider = gp;
	}
	glyph_provider* get_glyph_provider()
	{
		return s_glyph_provider;
	}

	static bool	s_use_cached_movie_def = true;

	// load movies from separate thread
	static bool	s_use_separate_loader = true;

	//
	// file_opener callback stuff
	//

	static file_opener_callback	s_opener_function = NULL;

	void	register_file_opener_callback(file_opener_callback opener)
	// Host calls this to register a function for opening files,
	// for loading movies.
	{
		s_opener_function = opener;
	}

	static bool	s_use_cache_files = false;

	void	set_use_cache_files(bool use_cache)
	// Enable/disable attempts to read cache files when loading
	// movies.
	{
		s_use_cache_files = use_cache;
	}


	int player::s_player_count = 0;

	// standard method map, this stuff should be high optimized

	static stringi_hash<as_value>*	s_standard_method_map[BUILTIN_COUNT];
	void clear_standard_method_map()
	{
		for (int i = 0; i < BUILTIN_COUNT; i++)
		{
			if (s_standard_method_map[i])
			{
				delete s_standard_method_map[i];
                s_standard_method_map[i] = NULL;
			}
		}
	}

	bool get_builtin(builtin_object id, const tu_stringi& name, as_value* val)
	{
		if (s_standard_method_map[id])
		{
			return s_standard_method_map[id]->get(name, val);
		}
		return false;
	}

	stringi_hash<as_value>* new_standard_method_map(builtin_object id)
	{
		if (s_standard_method_map[id] == NULL)
		{
			s_standard_method_map[id] = new stringi_hash<as_value>;
		}
		return s_standard_method_map[id];
	}

	void standard_method_map_init()
	{
		// setup builtin methods
		stringi_hash<as_value>* map;

		// as_object builtins
		map = new_standard_method_map(BUILTIN_OBJECT_METHOD);
		map->add("addProperty", as_object_addproperty);
		map->add("registerClass", as_object_registerclass);
		map->add("hasOwnProperty", as_object_hasownproperty);
		map->add("watch", as_object_watch);
		map->add("unwatch", as_object_unwatch);

		// flash9
		map->add("addEventListener", as_object_add_event_listener);

		// for debugging
#ifdef _DEBUG
		map->add("dump", as_object_dump);
#endif

		// as_number builtins
		map = new_standard_method_map(BUILTIN_NUMBER_METHOD);
		map->add("valueOf", as_number_valueof);
		map->add("toString", as_number_to_string);

		// as_boolean builtins
		map = new_standard_method_map(BUILTIN_BOOLEAN_METHOD);
		map->add("toString", as_boolean_to_string);
		map->add("valueOf", as_boolean_valueof);

		// as_string builtins
		map = new_standard_method_map(BUILTIN_STRING_METHOD);
		map->add("toString", string_to_string);
		map->add("fromCharCode", string_from_char_code);
		map->add("charCodeAt", string_char_code_at);
		map->add("concat", string_concat);
		map->add("indexOf", string_index_of);
		map->add("lastIndexOf", string_last_index_of);
		map->add("slice", string_slice);
		map->add("split", string_split);
		map->add("substring", string_substring);
		map->add("substr", string_substr);
		map->add("toLowerCase", string_to_lowercase);
		map->add("toUpperCase", string_to_uppercase);
		map->add("charAt", string_char_at);
		map->add("length", as_value(string_length, as_value()));

		// sprite_instance builtins
		map = new_standard_method_map(BUILTIN_SPRITE_METHOD);
		map->add("play", sprite_play);
		map->add("stop", sprite_stop);
		map->add("gotoAndStop", sprite_goto_and_stop);
		map->add("gotoAndPlay", sprite_goto_and_play);
		map->add("nextFrame", sprite_next_frame);
		map->add("prevFrame", sprite_prev_frame);
		map->add("getBytesLoaded", sprite_get_bytes_loaded);
		map->add("getBytesTotal", sprite_get_bytes_total);
		map->add("swapDepths", sprite_swap_depths);
		map->add("duplicateMovieClip", sprite_duplicate_movieclip);
		map->add("getDepth", sprite_get_depth);
		map->add("createEmptyMovieClip", sprite_create_empty_movieclip);
		map->add("removeMovieClip", sprite_remove_movieclip);
		map->add("hitTest", sprite_hit_test);
		map->add("startDrag", sprite_start_drag);
		map->add("stopDrag", sprite_stop_drag);
		map->add("loadMovie", sprite_loadmovie);
		map->add("unloadMovie", sprite_unloadmovie);
		map->add("getNextHighestDepth", sprite_getnexthighestdepth);
		map->add("createTextField", sprite_create_text_field);
		map->add("attachMovie", sprite_attach_movie);
		map->add("localToGlobal", sprite_local_global);
		map->add("globalToLocal", sprite_global_local);
		map->add("getRect", sprite_get_rect);
		map->add("getBounds", sprite_get_bounds);
		map->add("setMask", sprite_set_mask);

		// drawing API
		map->add("beginFill", sprite_begin_fill);
		map->add("endFill", sprite_end_fill);
		map->add("lineTo", sprite_line_to);
		map->add("moveTo", sprite_move_to);
		map->add("curveTo", sprite_curve_to);
		map->add("clear", sprite_clear);
		map->add("lineStyle", sprite_line_style);

		// gameSWF extension
		// reset root FPS
		map->add("setFPS", sprite_set_fps);

		// gameSWF extension
		// return true if movieclip is in PLAY state
 		map->add("getPlayState", sprite_get_play_state);

		// Flash9
		map->add("addFrameScript", sprite_add_script);

	}

	// Standard property lookup.

	static stringi_hash<as_standard_member>	s_standard_property_map;
	void clear_standard_property_map()
	{
		s_standard_property_map.clear();
	}

	const char* get_gameswf_version()
	{
#ifdef WIN32
	static tu_string s_gameswf_version("WIN ");
#else
	static tu_string s_gameswf_version("LINUX ");
#endif
		
		return s_gameswf_version.c_str();
	}

	// dynamic library stuff, for sharing DLL/shared library among different movies.

	static string_hash<tu_loadlib*> s_shared_libs;
	string_hash<tu_loadlib*>* get_shared_libs()
	{
		return &s_shared_libs;
	}

	void clear_shared_libs()
	{
		for (string_hash<tu_loadlib*>::iterator it = s_shared_libs.begin();
			it != s_shared_libs.end(); ++it)
		{
			delete it->second;
		}
		s_shared_libs.clear();
	}

	struct registered_type_node
	{
		registered_type_node(const tu_string& classname, gameswf_module_init type_init_func) :
			m_next(NULL),
			m_classname(classname),
			m_type_init(type_init_func)
		{
		}

		registered_type_node *m_next;
		tu_string             m_classname;
		gameswf_module_init   m_type_init;
	};

	static registered_type_node* s_registered_types = NULL;

	void register_type_handler(const tu_string& type_name, gameswf_module_init type_init_func )
	{
		registered_type_node** node = &s_registered_types;
		while(*node)
		{
			node = &((*node)->m_next);
		}
		*node = new registered_type_node(type_name, type_init_func);
	}

	void clear_registered_type_handlers()
	{
		registered_type_node *curr = s_registered_types;
		s_registered_types = NULL;
		while(curr)
		{
			registered_type_node *next = curr->m_next;
			delete curr;
			curr = next;
		}
	}

	gameswf_module_init find_type_handler(const tu_string& type_name)
	{
		registered_type_node *node = s_registered_types;
		while(node)
		{
			if (node->m_classname == type_name)
			{
				return node->m_type_init;
			}
			node = node->m_next;
		}
		return NULL;
	}

	// External interface.

	static fscommand_callback	s_fscommand_handler = NULL;

	fscommand_callback	get_fscommand_callback()
	{
		return s_fscommand_handler;
	}

	void	register_fscommand_callback(fscommand_callback handler)
	{
		s_fscommand_handler = handler;
	}

	as_standard_member	get_standard_member(const tu_stringi& name)
	{
		if (s_standard_property_map.size() == 0)
		{
			s_standard_property_map.set_capacity(int(AS_STANDARD_MEMBER_COUNT));

			s_standard_property_map.add("_x", M_X);
			s_standard_property_map.add("_y", M_Y);
			s_standard_property_map.add("_xscale", M_XSCALE);
			s_standard_property_map.add("_yscale", M_YSCALE);
			s_standard_property_map.add("_currentframe", M_CURRENTFRAME);
			s_standard_property_map.add("_totalframes", M_TOTALFRAMES);
			s_standard_property_map.add("_alpha", M_ALPHA);
			s_standard_property_map.add("_visible", M_VISIBLE);
			s_standard_property_map.add("_width", M_WIDTH);
			s_standard_property_map.add("_height", M_HEIGHT);
			s_standard_property_map.add("_rotation", M_ROTATION);
			s_standard_property_map.add("_target", M_TARGET);
			s_standard_property_map.add("_framesloaded", M_FRAMESLOADED);
			s_standard_property_map.add("_name", M_NAME);
			s_standard_property_map.add("_droptarget", M_DROPTARGET);
			s_standard_property_map.add("_url", M_URL);
			s_standard_property_map.add("_highquality", M_HIGHQUALITY);
			s_standard_property_map.add("_focusrect", M_FOCUSRECT);
			s_standard_property_map.add("_soundbuftime", M_SOUNDBUFTIME);
			s_standard_property_map.add("_xmouse", M_XMOUSE);
			s_standard_property_map.add("_ymouse", M_YMOUSE);
			s_standard_property_map.add("_parent", M_PARENT);
			s_standard_property_map.add("text", M_TEXT);
			s_standard_property_map.add("textWidth", M_TEXTWIDTH);
			s_standard_property_map.add("textColor", M_TEXTCOLOR);
			s_standard_property_map.add("border", M_BORDER);
			s_standard_property_map.add("multiline", M_MULTILINE);
			s_standard_property_map.add("wordWrap", M_WORDWRAP);
			s_standard_property_map.add("type", M_TYPE);
			s_standard_property_map.add("backgroundColor", M_BACKGROUNDCOLOR);
			s_standard_property_map.add("_this", M_THIS);
			s_standard_property_map.add("this", MTHIS);
			s_standard_property_map.add("_root", M_ROOT);
			s_standard_property_map.add(".", MDOT);
			s_standard_property_map.add("..", MDOT2);
			s_standard_property_map.add("_level0", M_LEVEL0);
			s_standard_property_map.add("_global", M_GLOBAL);
			s_standard_property_map.add("enabled", M_ENABLED);
			s_standard_property_map.add("password", M_PASSWORD);
			s_standard_property_map.add("onMouseMove", M_MOUSE_MOVE);
		}

		as_standard_member	result = M_INVALID_MEMBER;
		s_standard_property_map.get(name, &result);

		return result;
	}

	//
	// properties by number
	//

	static const tu_string	s_property_names[] =
	{
		tu_string("_x"),
		tu_string("_y"),
		tu_string("_xscale"),
		tu_string("_yscale"),
		tu_string("_currentframe"),
		tu_string("_totalframes"),
		tu_string("_alpha"),
		tu_string("_visible"),
		tu_string("_width"),
		tu_string("_height"),
		tu_string("_rotation"),
		tu_string("_target"),
		tu_string("_framesloaded"),
		tu_string("_name"),
		tu_string("_droptarget"),
		tu_string("_url"),
		tu_string("_highquality"),
		tu_string("_focusrect"),
		tu_string("_soundbuftime"),
		tu_string("mysteryquality"), //tu_string("@@ mystery quality member"),  //this seems like a stupid bug to me . . . but I don't want it accessing the heap yet.
		tu_string("_xmouse"),
		tu_string("_ymouse"),
	};

	as_value	get_property(as_object* obj, int prop_number)
	{
		as_value	val;
		if (prop_number >= 0 && prop_number < int(sizeof(s_property_names)/sizeof(s_property_names[0])))
		{
			obj->get_member(s_property_names[prop_number], &val);
		}
		else
		{
			log_error("error: invalid property query, property number %d\n", prop_number);
		}
		return val;
	}

	void	set_property(as_object* obj, int prop_number, const as_value& val)
	{
		if (prop_number >= 0 && prop_number < int(sizeof(s_property_names)/sizeof(s_property_names[0])))
		{
			obj->set_member(s_property_names[prop_number], val);
		}
		else
		{
			log_error("error: invalid set_property, property number %d\n", prop_number);
		}
	}

	//
	//	player
	//

	player::player() :
		m_force_realtime_framerate(false),
		m_log_bitmap_info(false)
	{
		m_global = new as_object(this);

		action_init();

		if (s_player_count == 0)
		{
			// timer should be inited only once
			tu_timer::init_timer();

			standard_method_map_init();
		}

		++s_player_count;
		
		// set startup random position
		Uint64 t = tu_timer::get_systime();
		t &= 0xFF;	// truncate
		for (unsigned int i = 0; i < t; i++)
		{
			tu_random::next_random();
		}

	}

	player::~player()
	{
		// Clean up gameswf as much as possible, so valgrind will help find actual leaks.
		// Maximum release of resources.  Calls clear_library() and
		// fontlib::clear(), and also clears some extra internal stuff
		// that may have been allocated (e.g. global ActionScript
		// objects).  This should get all gameswf structures off the
		// heap, with the exception of any objects that are still
		// referenced by the host program and haven't had drop_ref()
		// called on them.

		m_current_root = NULL;
		m_global = NULL;
		--s_player_count;
		
		clear_heap();

		gameswf_engine_mutex().lock();

		clear_library();

		// Clear shared stuff only when all players are deleted
		if (s_player_count == 0)
		{
			clears_tag_loaders();
            set_s_registered(false);

			clear_shared_libs();
			clear_registered_type_handlers();
			clear_standard_method_map();
			clear_disasm();
			delete s_glyph_provider;
			s_glyph_provider = NULL;
		}

		gameswf_engine_mutex().unlock();

		action_clear();
	}

	void player::set_flash_vars(const tu_string& param)
	// Allow pass user variables to Flash
	{
		m_flash_vars = param;
	}

	void player::verbose_action(bool val)
	{
		set_verbose_action(val);
	}

	void player::verbose_parse(bool val)
	{
		set_verbose_parse(val);
	}

	void	as_global_trace(const fn_call& fn);
	void	player::action_init()
	// Create/hook built-ins.
	{
		//
		// global init
		//

		m_heap.set(m_global.get_ptr(), false);
		m_global->builtin_member("trace", as_global_trace);
		m_global->builtin_member("Object", as_global_object_ctor);
		m_global->builtin_member("Sound", as_global_sound_ctor);

		m_global->builtin_member("Array", new as_global_array(this));
		m_global->builtin_member("MovieClip", as_global_movieclip_ctor);
		m_global->builtin_member("TextField", as_global_textfield_ctor);
		m_global->builtin_member("TextFormat", as_global_textformat_ctor);
		m_global->builtin_member("SharedObject", new as_sharedobject(this));
		m_global->builtin_member("Mouse", new as_mouse(this));

		//			m_global->set_member("XML", as_value(xml_new));
		m_global->builtin_member("MovieClipLoader", as_global_mcloader_ctor);
		m_global->builtin_member("String", get_global_string_ctor(this));
		m_global->builtin_member("Number", as_global_number_ctor);
		m_global->builtin_member("Boolean", as_global_boolean_ctor);
		m_global->builtin_member("Color", as_global_color_ctor);
		m_global->builtin_member("Date", as_global_date_ctor);
		m_global->builtin_member("Selection", selection_init(this));

		as_object * capabilities = new as_object(this);
		capabilities->set_member( "version", "WIN 9,0,45,0" );
		m_global->builtin_member("Capabilities", capabilities);

#if TU_ENABLE_NETWORK == 1
		m_global->builtin_member("XMLSocket", as_global_xmlsock_ctor);
		m_global->builtin_member("LoadVars", as_global_loadvars_ctor);
#endif

		// ASSetPropFlags
		m_global->builtin_member("ASSetPropFlags", as_global_assetpropflags);

		// for video
		m_global->builtin_member("NetStream", as_global_netstream_ctor);
		m_global->builtin_member("NetConnection", as_global_netconnection_ctor);

		m_global->builtin_member("math", math_init(this));
		m_global->builtin_member("Key", key_init(this));
		m_global->builtin_member("AsBroadcaster", broadcaster_init(this));
		m_global->builtin_member("flash", flash_init(this));

		// global builtins functions
		m_global->builtin_member("setInterval",  as_global_setinterval);
		m_global->builtin_member("clearInterval",  as_global_clearinterval);
		m_global->builtin_member("setTimeout",  as_global_settimeout);
		m_global->builtin_member("clearTimeout",  as_global_clearinterval);
		m_global->builtin_member("getVersion",  as_global_get_version);
		m_global->builtin_member("parseFloat",  as_global_parse_float);
		m_global->builtin_member("parseInt",  as_global_parse_int);
		m_global->builtin_member("isNaN",  as_global_isnan);
		m_global->builtin_member("$version", as_value(as_global_get_version, as_value()));
		m_global->builtin_member("updateAfterEvent", as_global_update_after_event);
	}


	void	player::action_clear()
	{
		if ( 0 == s_player_count )
		{
			clear_standard_property_map();
		}
	}

	as_object* player::get_global() const
	{
		return m_global.get_ptr();
	}

	void player::notify_key_object(key::code k, bool down)
	{
		as_value	kval;
		as_object* global = get_global();
		global->get_member("Key", &kval);
		as_key*	ko = cast_to<as_key>(kval.to_object());
		if (ko)
		{
			if (down) ko->set_key_down(k);
			else ko->set_key_up(k);
		}
		else
		{
			log_error("gameswf::notify_key_event(): no Key built-in\n");
		}
	}

	root* player::get_root()
	{
		// on exit m_current_root may be NULL
//		assert(m_current_root.get_ptr() != NULL);
		return m_current_root.get_ptr();
	}

	character*	player::get_root_movie() const
	{
		if (m_current_root != NULL)
		{
			return m_current_root->get_root_movie();
		}
		return NULL;
	}

	void player::notify_key_event(key::code k, bool down)
	{
		m_current_root->notify_key_event(this, k, down);
	}

	void player::set_root(root* m)
	{
		assert(m != NULL);
		m_current_root = m;
	}

	const char* player::get_workdir() const
	{
		return m_workdir.c_str();
	}

	void player::set_workdir(const char* dir)
	{
		assert(dir != NULL);
		m_workdir = dir;
	}

	// library stuff, for sharing resources among different movies.
	string_hash<gc_ptr<character_def> >* player::get_chardef_library()
	{
		return &m_chardef_library;
	}

	const char* player::get_root_filename(const character_def* rdef)
	// get filename by root movie definition
	{
		for (string_hash<gc_ptr<character_def> >::iterator it = m_chardef_library.begin();
			it != m_chardef_library.end(); ++it)
		{
			if (it->second == rdef)
			{
				return it->first.c_str();
			}
		}
		return NULL;
	}

	void player::clear_library()
	// Drop all library references to movie_definitions, so they
	// can be cleaned up.
	{
		for (string_hash<gc_ptr<character_def> >::iterator it = 
			m_chardef_library.begin(); it != m_chardef_library.end(); ++it)
		{
			if (gc_collector::debug_get_ref_count(it->second) > 1)
			{
				printf("memory leaks is found out: on exit movie_definition_sub ref_count > 1\n");
				printf("this = 0x%p, ref_count = %d\n", it->second.get_ptr(),
				       gc_collector::debug_get_ref_count(it->second));

				// to detect memory leaks
				while (gc_collector::debug_get_ref_count(it->second) > 1) {
					gc_collector::debug_decrement_ref_count(it->second);
				}
			}
		}
		m_chardef_library.clear();
	}

	void	ensure_loaders_registered();
	movie_definition*	player::create_movie(const char* filename)
	{
		assert(filename);

		// Is the movie already in the library?
		if (s_use_cached_movie_def)
		{
			gc_ptr<character_def>	m;
			get_chardef_library()->get(filename, &m);
			if (m != NULL)
			{
             //   CCLog("qiaoxin   create_movie  is  null ..... ");
				// Return cached movie.
				return cast_to<movie_definition>(m.get_ptr());
			}
		}

		if (s_opener_function == NULL)
        {
           // CCLog("qiaoxin   create_movie  s_opener_function is  null ..... ");
			// Don't even have a way to open the file.
			log_error("error: no file opener function; can't create movie.	"
				"See gameswf::register_file_opener_callback\n");
			return NULL;
		}

		tu_file* in = s_opener_function(filename);
		if (in == NULL)
		{
           // CCLog("qiaoxin   create_movie  in is  null ..... ");
			log_error("failed to open '%s'; can't create movie.\n", filename);
			return NULL;
		}
		else if (in->get_error())
		{
            //CCLog("qiaoxin   create_movie  in->get_error() is  null .....:%d ", in->get_error());
			log_error("error: file opener can't open '%s'\n", filename);
			delete in;
			return NULL;
		}

		ensure_loaders_registered();

		movie_def_impl*	m = new movie_def_impl(this, DO_LOAD_BITMAPS, DO_LOAD_FONT_SHAPES);

		if (s_use_cached_movie_def)
		{
			get_chardef_library()->add(filename, m);
		}

		m->read(in);

		// "in" will be deleted after termination of the loader thread
		//	delete in;

		if (m && s_use_cache_files)
		{
			// Try to load a .gsc file.
			tu_string	cache_filename(filename);
			cache_filename += ".gsc";
			tu_file*	cache_in = s_opener_function(cache_filename.c_str());
			if (cache_in == NULL
				|| cache_in->get_error() != TU_FILE_NO_ERROR)
			{
				// Can't open cache file; don't sweat it.
				IF_VERBOSE_PARSE(log_msg("note: couldn't open cache file '%s'\n", cache_filename.c_str()));
			}
			else
			{
				// Load the cached data.
				m->input_cached_data(cache_in);
			}

			delete cache_in;
		}

		return m;
	}

	gc_ptr<root> player::load_file(const char* infile)
	// Load the actual movie.
	{
		gc_ptr<gameswf::movie_definition>	md = create_movie(infile);
		if (md == NULL)
		{
           // CCLog("qiaoxin   md  is  null ..... %s", infile);
			fprintf(stderr, "error: can't create a movie from '%s'\n", infile);
			return NULL;
		}

		gc_ptr<gameswf::root>	m = md->create_instance();
		if (m == NULL)
		{
           // CCLog("qiaoxin   m  is  null .....");
			fprintf(stderr, "error: can't create movie instance\n");
			return NULL;
		}

		int	movie_version = m->get_movie_version();
       // CCLog("qiaoxin   movie_version:%d", movie_version);

	#ifdef _DEBUG
		log_msg("Playing %s, swf version %d\n", infile, movie_version);
	#else
		IF_VERBOSE_PARSE(log_msg("Playing %s, swf version %d\n", infile, movie_version));
	#endif

		return m;
	}

	const bool player::get_force_realtime_framerate() const
	{
		return m_force_realtime_framerate;
	}

	void player::set_force_realtime_framerate(const bool force_realtime_framerate)
	{
		m_force_realtime_framerate = force_realtime_framerate;
	}

	// garbage collector

	void player::set_alive(as_object* obj)
	{
		m_heap.set(obj, false);
	}

	bool player::is_garbage(as_object* obj)
	{
		bool is_garbage = false;
		m_heap.get(obj, &is_garbage);
		return is_garbage;
	}

	void player::clear_heap()
	{
		for (gameswf::hash<gc_ptr<as_object>, bool>::iterator it = m_heap.begin();
			it != m_heap.end(); ++it)
		{
			as_object* obj = it->first.get_ptr();
			if (obj)
			{
				if (gc_collector::debug_get_ref_count(obj) > 1)
				{
					gameswf::hash<as_object*, bool> visited_objects;
					obj->clear_refs(&visited_objects, obj);
				}
			}
		}
		m_heap.clear();
	}

	void player::set_as_garbage()
	{
		for (gameswf::hash<gc_ptr<as_object>, bool>::iterator it = m_heap.begin();
			it != m_heap.end(); ++it)
		{
			as_object* obj = it->first.get_ptr();
			if (obj)
			{
				m_heap.set(obj, true);
			}
		}
	}

	void player::clear_garbage()
	{
		m_global->this_alive();
		for (gameswf::hash<gc_ptr<as_object>, bool>::iterator it = m_heap.begin();
			it != m_heap.end(); ++it)
		{
			as_object* obj = it->first.get_ptr();
			if (obj)
			{
				if (it->second)	// is garbage ?
				{
					if (gc_collector::debug_get_ref_count(obj) > 1)	// is in heap only ?
					{
						gameswf::hash<as_object*, bool> visited_objects;
						obj->clear_refs(&visited_objects, obj);
					}
					m_heap.erase(obj);
				}
			}
		}
	}

	bool player::use_separate_thread()
	{
		return s_use_separate_loader;
	}

	void player::set_separate_thread(bool flag)
	{
		s_use_separate_loader = flag;
	}

}



// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
