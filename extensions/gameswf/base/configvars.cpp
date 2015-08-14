// configvars.cpp	-- by Thatcher Ulrich <tu@tulrich.com> 22 July 2001

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Configuration glue.  C++ interface to Lua scripting library.

// @@ move/finish this later...
#if 0


#include "configvars.h"
extern "C" {
#include <lualib.h>
}
#include "utility.h"


namespace config {


static const int	LUA_STACK_SIZE = 2048;


static bool	s_open = false;

lua_State*	L = NULL;
int	g_cfloat_tag = 0;


static int	cfloat_get( lua_State* L )
// Lua CFunction which returns the value of the cfloat passed in on
// the top of the Lua stack.  Attach this to the "getglobal" event for
// cfloats.
{
	// arg 1: varname
	// arg 2: raw var value (in this case the cfloat*)

	cfloat*	cf = static_cast<cfloat*>( lua_touserdata( L, 2 ) );
	lua_pushnumber( L, float( *cf ) );

	return 1;
}


static int	cfloat_set( lua_State* L )
// Lua CFunction which sets the value of the given cfloat to the given lua
// number.
{
	// arg 1: varname
	// arg 2: previous var value (in this case the cfloat*)
	// arg 3: new var value

	cfloat*	cf = static_cast<cfloat*>( lua_touserdata( L, 2 ) );
	*cf = float(lua_tonumber( L, 3 ));

	return 0;
}


void	open()
// Initialize the config subsystem.  ...
{
	if ( s_open == true ) return;

	//
	// Initialize Lua.
	//
	L = lua_open( LUA_STACK_SIZE );

	// Init the standard Lua libs.
	lua_baselibopen( L );
	lua_iolibopen( L );
	lua_strlibopen( L );
	lua_mathlibopen( L );

	//
	// Attach the cfloat hooks
	//

	g_cfloat_tag = lua_newtag( config::L );

	lua_pushcfunction( config::L, cfloat_get );
	lua_settagmethod( config::L, config::g_cfloat_tag, "getglobal" );	// xxx is "getglobal" right?

	lua_pushcfunction( config::L, cfloat_set );
	lua_settagmethod( config::L, config::g_cfloat_tag, "setglobal" );	// xxx is "setglobal" right?

	// set tag methods for add, sub, mul, div, pow, unm, lt

	//	gettable{ min, max, default, comment }
	//	settable{ min, max, default, comment }

	s_open = true;
}


void	close()
// Close the config subsystem.
{
	// Nothing really to do here.
}


}; // end namespace config



//
// cvar
//


void	cvar::init( const char* name )
// Initializes a chain of references to Lua strings, for looking up
// the value of the specified name.
//
// The name can contain embedded '.' separators, to refer to values
// within nested tables.  For example, a name of
// "player.spaceship.health" would refer to the "health" member of the
// "player.spaceship" table (where the "player.spaceship" table is
// found by looking up the "spaceship" member of the "player" table).
{
	config::open();

	// Count the number of keys in the name (keys are separated by '.'
	// chars).
	m_lua_key_count = 1;
	const char*	p = name;
	while (*p) {
		if (*p == '.') {
			m_lua_key_count++;
		}
		p++;
	}

	// Allocate array for references.
	m_lua_key_reference = new int[m_lua_key_count];

	// Now initialize the keys.
	const char*	varname = name;

	int	key_index = 0;
	p = name;
	while (*p) {
		if (*p == '.') {
			if (varname == p) {
				// null string for a key.
				// warning("something or other");
				assert(0);	// TODO: recover somehow.
			}

			// Reference the name of the table.
			lua_pushlstring(config::L, varname, p - varname);
			m_lua_key_reference[key_index] = lua_ref(config::L, 1);

			key_index++;
			varname = p + 1;
		}
		p++;
	}

	// Get a reference to the last key name.
	assert(varname != p);	// else...
	lua_pushlstring(config::L, varname, p - varname);
	m_lua_key_reference[key_index] = lua_ref(config::L, 1);
}


void	cvar::push_table_and_key() const
// Traverse our key names, and push the table and keyname of our value
// onto the Lua stack.
//
// Creates empty tables if necessary to fill in missing links in the
// chain.
{
	lua_getglobals(config::L);	// Start with the global table.

	// Chain through additional tables.
	int	i;
	for (i = 0; i < m_lua_key_count - 1; i++) {
		lua_getref(config::L, m_lua_key_reference[i]);
		lua_gettable(config::L, -2);

		if (lua_isnil(config::L, -1)) {
			// Tablename is undefined, so create a new empty table for it.
			lua_pop(config::L, 1);	// pop the nil.
			lua_getref(config::L, m_lua_key_reference[i]);
			lua_newtable(config::L);
			lua_settable(config::L, -3);
			
			// Get the newly created table and put it on the top
			// of stack.
			lua_getref(config::L, m_lua_key_reference[i]);
			lua_gettable(config::L, -2);
		}

		lua_remove(config::L, -2);	// previous table that we just chained from
	}

	// push the final key, on top of the table we just pushed.
	lua_getref(config::L, m_lua_key_reference[i]);
}


cvar::cvar( const char* name )
// Constructor; leaves existing value, if any (otherwise it's 'nil').
{
	init(name);
}


cvar::cvar( const char* name, const cvalue& val )
// Constructor; initializes to given Lua value.
{
	init(name);
	*this = val;	// invoke operator=(const cvalue& val)
}


cvar::cvar( const char* name, const char* val )
// Constructor; initializes to given string value.
{
	init(name);
	*this = val;	// invoke operator=(const char*)
}


cvar::cvar( const char* name, float val )
// Constructor; initializes to given float value.
{
	init(name);
	*this = val;	// invoke operator=(float f)
}


cvar::~cvar()
// Destructor; make sure our references are released.
{
	// drop lua references, so table & name can be gc'd if not
	// referenced elsewhere.

	for (int i = 0; i < m_lua_key_count; i++) {
		lua_unref(config::L, m_lua_key_reference[i]);
	}

	m_lua_key_count = 0;
	m_lua_key_reference = NULL;
}


#if 0
const char*	cvar::get_name() const
// Return our name.  The char[] storage is valid at least as long
// as this variable is alive.
{
	lua_getref( config::L, m_lua_name_reference );
	return lua_tostring( config::L, -1 );
}
#endif // 0


cvar::operator float() const
// Convert the variable to a float and return it.
{
	push_table_and_key();
	lua_gettable( config::L, -2 );	// get the value of our variable from the table.
	float	f = float(lua_tonumber( config::L, -1 ));
	lua_pop( config::L, 2 );	// pop table & the number result.

	return f;
}


void	cvar::operator=( float f )
// Assign a float to this lua variable.
{
	push_table_and_key();
	lua_pushnumber( config::L, f );
	lua_settable( config::L, -3 );
	lua_pop( config::L, 1 );	// pop the table.
}


cvar::operator const char*() const
// Convert to a string.
//
// xxx there are garbage-collection issues here!  Returned string
// has no valid reference once stack is cleared!
// Possible fixes:
// - return some kind of proxy object that holds a locked Lua ref
// - return a C++ "string" value; i.e. make a copy
// - hold a locked reference in this instance; drop it on next call to this conversion operator (blech).
{
	push_table_and_key();
	lua_gettable( config::L, -2 );	// get the value of our variable from the table.
	const char*	c = lua_tostring( config::L, -1 );
	// TODO: grab a locked reference to the string!  Or copy it!
	lua_pop( config::L, 2 );	// discard table & the string result.

	return c;
}


void	cvar::operator=( const cvalue& val )
// Assign a Lua value to this lua global variable.
{
	push_table_and_key();
	val.lua_push();
	lua_settable( config::L, -3 );
	lua_pop( config::L, 1 );	// pop the table.
}


void	cvar::operator=( const char* s )
// Assign a string to this lua variable.
{
	push_table_and_key();
	lua_pushstring( config::L, s );
	lua_settable( config::L, -3 );
	lua_pop( config::L, 1 );	// pop the table.
}


cvar::operator cvalue() const
// Return a reference to our value.
{
	push_table_and_key();
	cvalue	c = cvalue::lua_stacktop_reference();
	lua_pop( config::L, 1 );	// pop the table.

	return c;
}


//	void	operator=( const cvar c );



//
// cvalue
//


cvalue::cvalue( const char* lua_constructor )
// Evaluates the given code and takes a reference to the result.
{
	config::open();

	lua_dostring( config::L, lua_constructor );	// @@ could check for error return codes, presence of return value, etc.
	m_lua_ref = lua_ref( config::L, 1 );
}


cvalue::cvalue( const cvalue& c )
{
	lua_getref( config::L, c.m_lua_ref );
	m_lua_ref = lua_ref( config::L, 1 );
}


cvalue::cvalue()
// Creates an reference to nothing.  Use this only in special
// circumstances; i.e. when you're about to set m_lua_ref manually.
{
	config::open();
	m_lua_ref = LUA_NOREF;
}


cvalue	cvalue::lua_stacktop_reference()
// Factory function; pops the value off the top of the Lua stack, and
// return a cvalue that references the popped value.
{
	cvalue	c;
	c.m_lua_ref = lua_ref( config::L, 1 );
	return c;
}


cvalue::~cvalue()
// Drop our Lua reference, to allow our value to be gc'd.
{
	lua_unref( config::L, m_lua_ref );
	m_lua_ref = LUA_NOREF;
}


void	cvalue::lua_push() const
// Push our value onto the top of the Lua stack.
{
	assert( m_lua_ref != LUA_NOREF );
	lua_getref( config::L, m_lua_ref );
}


void	cvalue::operator=( const char* str )
// Transfer our reference to the given string.
{
	lua_unref( config::L, m_lua_ref );
	lua_pushstring( config::L, str );
	m_lua_ref = lua_ref( config::L, 1 );
}


void	cvalue::operator=( const cvalue& c )
// Reference the thing that c references.
{
	lua_unref( config::L, m_lua_ref );
	lua_getref( config::L, c.m_lua_ref );
	m_lua_ref = lua_ref( config::L, 1 );
}


cvalue::operator float() const
// Converts this Lua value to a number, and returns it.
{
	lua_getref( config::L, m_lua_ref );
	float	f = float(lua_tonumber( config::L, -1 ));
	lua_pop( config::L, 1 );
	return f;
}


cvalue::operator const char*() const
// Converts this Lua value to a string, and returns it.
{
	lua_getref( config::L, m_lua_ref );
	const char*	str = lua_tostring( config::L, -1 );
	lua_pop( config::L, 1 );	// @@ I'm pretty sure this imperils the string, if we just had to do a tostring() conversion!  Look into this, and/or make a copy of the string.
	return str;
}


cvalue	cvalue::get( const char* index )
// Does a table lookup.  *this should be a Lua table, and index is its
// key.
{
	lua_getref( config::L, m_lua_ref );
	lua_pushstring( config::L, index );
	lua_gettable( config::L, -2 );
	cvalue	c = cvalue::lua_stacktop_reference();	// references the value on the top of the Lua stack.
	lua_pop( config::L, 1 );

	return c;
}


#endif // 0
