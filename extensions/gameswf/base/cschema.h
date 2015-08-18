// Notes for contemplated "Pretty good C++ schema system"

/*
These are some recurring needs in C++ game development:

* to initialize hard-coded C++ structures (structs & classes) from a
  softer data source such as XML, Lua, s-lists, .INI or whatever
  custom data format you've cooked up to describe game levels.

* to serialize the state of the C++ program in order to implement
  game-saving.

* to dynamically tweak live values in the program in order to tune
  gameplay.

There are many ways to service these needs.  The needs are not
identical, and need not be implemented through a common system.
Nevertheless, at the core, they do all have something in common: in
some way, they all require ways to annotate the content of C++
datatypes.  In other words, they provide a "schema" that describes
what's inside live C++ data structures.

It seems to me that a "pretty good" universal schema system for C++,
tailored for the above game-development needs, would be extremely
useful to have.  "Pretty good" is not a modest target; I've
implemented variations on the above tasks dozens of times in my career
and IMO none of my implementations have risen to the status of "pretty
good".

These are some requirements I would expect from a "pretty good C++
schema system":

* Fast path.  Data assets are compilable into a compact format, with a
  "fast path" for loading.  Ideally the compact format would just be a
  memory dump of live data structures, so loading it is essentially
  just fread(), plus minimal fixup.  Notes:

  * in order to be safe, you need to check that the schema of the
    saved data matches the schema of the objects you're about to
    fread() into.  This can be done by storing a digest (hash) of the
    schema description with the saved data, and comparing the digest
    on load.  If the digests match, then fread() is safe.  If they
    don't match, then you want to fall back on a slower path, which
    uses a stored digest in the saved data to interpret the dumped
    binary data, and push it as well as possible into the matching
    elements of the run-time data structure.

* Human-readable capability.  It should be possible to initialize data
  structures from a reasonably sane ASCII format.  This doesn't have
  to be the format of the final shipped assets (see "fast path"
  above), but this is so useful during development that you want to be
  able to support it.

* Minimal impact on C++ code.  There is no way to implement a C++
  schema system without either adding some code to your C++ program,
  or implementing some kind of processor that reads your C++ source or
  object files in order to understand your data structures.  There are
  different schools of thought on this:

  1 Add code and macros to describe/iterate the C++ structures.  This
    is what you're doing when you write "save/load" code that reads &
    writes each of the members in your data structures.  One
    (arguable) improvement is a set of macros & templates where you
    declare the members of your structs, which initializes some info
    that the run-time system can use to iterate over your structs.

  2 Use a tool to read your C++ source and extract the schemas.
    E.g. SWIG, tolua, etc.  This cuts down on busy-work for the
    programmer, but on the other hand, creates some extra complexity
    in the building process.

  3 Parse some side-data emitted by the compiler.  See Bilas' FUBI for
    example.  http://www.drizzle.com/~scottb/publish/gpgems1_fubi.htm

  For a generic "pretty good" system, the only acceptable approach is
  1.  2 can be nice, but it's a tough sell for cross-platform
  development, especially if one of the platforms involves MSVC.  3 is
  necessarily compiler-specific.

* Acceptable level of complexity.  Remains to be seen how bad
  "acceptable" is.

* Needs to deal with virtual inheritance in C++ (not to mention
  regular inheritance), because IMO it's useful.

Random thoughts on implementation:

* use a function per-class to iterate over internals.  Use some kind
  of visitor pattern to accomodate various needs.  The schema
  iteration could compile a schema description for later use
  (generating digests), or serialize the state of an object directly.
  E.g.

  class some_class : virtual public parent_class1, virtual public parent_class2
  {
    ...
    virtual void    do_schema(cschema_visitor* v);

  private:
    int m_member1;
    some_other_class*   m_member2;
    ...
  };


  void some_class::do_schema(cschema_visitor* v)
  {
    // these might be overloaded macro/template magic
    schema_start(v, some_class);
    do_schema(v, (parent_class1*) this);
    do_schema(v, (parent_class2*) this);
    do_schema(v, m_member1);
    do_schema(v, m_member2);
    ...
  }

  Basically, when it gets down to basic datatypes, do_schema() calls
  cschema_visitor::notify_member(member_type, member_name, member_address)

  Note to self: overloading a built-in operator (e.g. like C++ streams
  do w/ operator<<) has advantages, since you can provide an overload
  that matches a class, *without* making it a member function.

* It's good to have a "update_cached()" method that gets called after
  loading from a schema.

* For tweaking in particular, it's helpful to have a way to add extra
  annotation.  E.g.:

  do_schema(v, m_some_member, "min=0 max=100 desc='number of widgets emitted per second'");

  Where the extra info gets exposed to a tweaking tool.

* Pointers are important.  Basically, in order to serialize pointers,
  what we need is:

  * Given a pointer to a live object, be able to get a descriptor of
    its concrete type (e.g. a type name string; could be
    typeid(*ptr).name(), but that tends to make an ugly string, and
    would require users to enable RTTI).

  * table of factory functions that take a stream for init, indexed by
    type descriptor.

  The rest we can implement with a pointer map, by just tracing all
  pointers and serializing objects as we go.

The above discussion is a lot of "how", i.e. how to build schemas, but
not enough "what", i.e. what exactly are these schemas?

Schemas
=======

A cschema is a description of a block of memory, corresponding to a
compiled C++ datatype.

...


A Totally Different Approach
============================

Another approach to this problem is to "fix" C++'s data model.  What I
have in mind is replacing C++'s heap and static variables with
something that is SQL-like, and then bind the SQL data structures to
some fancy DB back-end.

Half-baked sketch:

table entities {
        // table layout; roughly equivalent to 'struct' or 'class'
        int id = 0 (primary key autoincrement);
        string name = "";
        bool updatable = true;
        vec3 position = vec3::zero;
        float health = 100;
        ...

        // allow member functions in here too?  I guess so.
} = {
        // some static initialization
        { , "world", false, 0, ... },
        { , "player0", true, vec3(100, 0, 100), 100, ... },
        ...
};

void    update_entities()
{
        // SQL-like iteration through a subset of the rows; maybe there is a better
        // way to express this using "views" or something.
        for (entities.iterator e = entities.where("updatable = true"); e != entities.end(); ++e)
        {
                e.row.update(); // or update_entity(e) or entities.update_row(e.id) or ???
        }
}

So the idea is that persistence can happen easily via something like
"database.dump(filename)", and the SQL key crud automatically deals
with pointer fixup and GC, and under the hood the back-end can do lots
of intelligent stuff to optimize your queries, with minimal hinting
from the programmer.

To implement this, I guess the reasonable thing would be to preprocess
the augmented language and output C++ code.  Hm, unfortunate.  Or,
perhaps there is some insane template trickery that could do it
(although not necessarily with all the syntax amenities)?

*/
