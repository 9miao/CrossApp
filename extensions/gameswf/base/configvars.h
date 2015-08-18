// configvars.h	-- by Thatcher Ulrich <tu@tulrich.com> 22 July 2001

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Configuration glue.  C++ interface to Lua scripting library, plus utilities.


#ifndef CONFIGVARS_H
#define CONFIGVARS_H


// @@ this probably doesn't belong in "base"... move it somewhere and
// finish later...
#if 0


extern "C" {
#include <lua.h>
}


namespace config {
	void	open();
	void	close();
	// void	clear();

	//
	// Globals to help interface w/ Lua.  User code usually won't be
	// interested in these.
	//
	extern lua_State*	L;	// Lua state.  Passed as a param to most lua_ calls.
	extern int	g_cfloat_tag;	// TODO: no need to expose this, so hide it (need to hide cfloat::lua_push()).
};


class cfloat
// A class that holds a floating point value.  Accessible from Lua via
// a global variable.  Has the time efficiency of an ordinary C++
// float when accessed from C++, so it's OK to use these in inner
// loops.
{
public:
	cfloat( float val )
	// Constructs; does not assign to a Lua global variable.
	{
		m_value = val;
	}
	
	cfloat( const char* name, float val )
	// Constructs, and assigns to the Lua global variable whose name is given.
	{
		m_value = val;

		lua_push();
		lua_setglobal( config::L, name );
	}

	void	lua_push()
	{
		// Identify us to Lua as a cfloat.
		config::open();	// make sure Lua is set up.
		lua_pushuserdata( config::L, this );
		lua_settag( config::L, config::g_cfloat_tag );
	}

	~cfloat()
	{
		// do nothing?
		// set our global var to nil ???  To a Lua number equal to our value???
	}

	operator float()
	// Return our value.
	{
		return m_value;
	}

	void operator=( float f )
	// Set our value to f.
	{
		m_value = f;
	}

	// +=, -=, *=, /= etc

	// config::open() will hook in some handlers so Lua can have access to our value.
//private:
	float	m_value;
};


#define declare_cfloat( name, value ) cfloat name( #name, ( value ) )

// Do we even need a C++ cfloat class?  E.g. just use a pointer to
// an ordinary float:
//	#define declare_cfloat(name, value) \x
//		extern float name = value; config::export_float(#name, &f);
//
// Probably still need a class constructor tho, to do that export automatically.

// usage, very much like a C++ global float:
//
//	extern cfloat max_pixel_error;
//	declare_cfloat( max_pixel_error, 5.0f );	would this work?: cfloat name( #name ) = value;
//  declare_cfloat( max_pixel_error );
//
//	// Use it like an ordinary float.
//	max_pixel_error = 10.f;
//	printf( "%f\n", max_pixel_error );
//
//	// But, it can be accessed from Lua at nominal cost...
//
//	config.lua:
//		max_pixel_error = 5.0f;
//
//	Lua scripts, debugger or console can find and manipulate.


class cvalue {
// Hook to anonymous, dynamically typed Lua value.  Holds a Lua
// reference to its value for proper gc.
//
// Can use this for handy evaluation of Lua snippets, by passing some
// Lua code to the constructor.
public:
	cvalue( const cvalue& c );
	cvalue( const char* lua_constructor );
	~cvalue();
	static cvalue	lua_stacktop_reference();	// factory; creates a reference to the Lua stack top, and pops.

private:
	cvalue();	// Used only in cvalue::lua_stacktop_reference()
public:

	void	operator=( const char* str );
	void	operator=( float num );
	void	operator=( const cvalue& cval );

	// other basic stuff, such as arithmetic ops, and string concatenation...

	operator	float() const;	// tonumber
	operator	const char*() const;	// tostring

	// Lua tables can be indexed by both strings and numbers.
	void	set( const char* index, cvalue val );
	void	set( float index, cvalue val );
	void	set( cvalue index, cvalue val );

	void	set( const char* index, float f );
	void	set( float index, float f );

	void	set( const char* index, const char* str );
	void	set( float index, const char* str );

	cvalue	get( const char* index );
	cvalue	get( float index );

	// const char*	type();

	void	lua_push() const;	// push our value onto the Lua stack.

private:
	int	m_lua_ref;
};


// cvalue cv("return point(10, 10)");
// cv.set("x", 20);
// printf("y = %f\n", (float) cv.get("y"));


class cvar {
// Hook to Lua variable.  cvars are dynamically typed and have to do
// hash lookups & type coercion to access from the C++ side.  Some
// gc issues with conversion to C strings.
public:
	cvar( const char* name );
	cvar( const char* name, const cvalue& val );
	cvar( const char* name, const char* str );
	cvar( const char* name, float val );
	~cvar();

//	const char*	get_name() const;	// is this useful, maybe for debugging?  Build up the name, etc.  Could maybe return a cvalue() and use Lua string-handling & gc.

	operator float() const;
	operator const char*() const;

	void	operator=( float f );
	void	operator=( const char* s );
	void	operator=( cfloat c );

	// cvalue access
	operator cvalue() const;
	void	operator=( const cvalue& c );

	// TODO: vec3 access
	// TODO: orientation access
	// TODO: actor, component access

	// cvar assignment
	void	operator=( cvar c );

private:
	void	init(const char* name);
	void	push_table_and_key() const;

	int	m_lua_key_count;
	int*	m_lua_key_reference;	// Array of lua key references, for quicker lookup.
};


#define declare_cvar( name, value ) cvar name( #name, value )


// e.g.
//
// declare_cvar( "varname", "string" );
// float f = cvar( "varname" );



// e.g.
//
//	declare_cvar( snowflakes, 5.0f );
//	declare_cvar( snowflake_image, "snowflake.jpg" );
//
//	snowflakes++;
//	snowflakes = 10;
//	for ( i = 0; i < snowflakes; i++ ) {	// hash lookup here
//		make_snowflake( snowflake_image );	// hash lookup here
//	}


//// how to do "varname = val" from c?  config::set(var, float/string/cvalue); cset(var, val) == shorthand?
//declare_cvalue("varname", "'string'");
//declare_cstring(varname, "string");	// Will C preprocessor quote the embedded quotes???
//declare_cfloat(varname, 10.f);


// e.g.
//
//	namespace playerprefs {
//		cvar	jumpheight("playerprefs.jumpheight");
//		cvar	runspeed("playerprefs.runspeed");
//	};
//
// The problem with this idiom is that it involves a lot of
// error-prone duplicate typing of names.  Really it would be nice to
// have some C++ level construct to do constructors that look more
// like Lua.  Maybe some wacky macros do the trick:

#if 0

#define PREFS(tablename) \
	namespace tablename { const char* _tablename = #tablename;

#define PREF(varname) \
	cvar varname(_tablename, #varname)

#define END_PREFS	}


PREFS(playerprefs)
	PREF(jumpheight);
	PREF(runspeed);
END_PREFS

#endif // 0


// Access the cvars using namespace syntax.  So,
//		float vspeed = compute_vspeed(playerprefs::jumpheight, gravity);

// Can't do nested stuff like:
//
// PREFS(actorprefs.player.movement)
//	etc...;


// The best-looking and least-error-prone way would be to use some
// kind of preprocessor that scans C++ and replaces Lua prefs
// declarations.  But that's somewhat radical.  It might look like
// this:
//
//	PREFS actorprefs::player::movement {
//		cfloat	runspeed = 10;
//		cvar	jumpheight = 27;

//		cproperty	....;
//	};
//
// which would emit C++ code:
//
//	namespace actorprefs { namespace player { namespace movement {
//		cvar runspeed("runspeed", 10);
//		cvar jumpheight("jumpheight", 27);
//	} } }
//
// And then access via:
//
//	float vspeed = compute_vspeed(actorprefs::player::movement, gravity);
//
// could even use "using" declarations...


#endif // 0

#endif // CONFIGVARS_H
