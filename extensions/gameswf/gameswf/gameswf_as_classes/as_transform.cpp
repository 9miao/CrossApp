// as_transform.cpp	-- Julien Hamaide <julien.hamaide@gmail.com>	2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.


#include "as_transform.h"

namespace gameswf
{

	enum as_transform_member
	{
		colorTransform = 1, 
		concatenatedColorTransform, 
		matrix,
		concatenatedMatrix,
		pixelBounds,
		invalidMember,
		AS_TRANSFORM_MEMBER_COUNT,
	};

	static stringi_hash<as_transform_member>	s_transform_member_map;
	static as_transform_member   get_transform_member(const tu_stringi& name);

	// Transform( mc:MovieClip )
	void	as_global_transform_ctor(const fn_call& fn)
	{
		if (fn.nargs == 1)
		{
			gc_ptr<as_transform>	obj;
			if (character* movie = cast_to<character>(fn.arg(0).to_object()))
			{
				obj = new as_transform(fn.get_player(), movie);
			}
			fn.result->set_as_object(obj.get_ptr());
		}
	}

	as_transform::as_transform(player* player, character* movie_clip) :
		as_object(player),
		m_movie(movie_clip)
	{
	}

	bool	as_transform::set_member(const tu_stringi& name, const as_value& val)
	{
		as_transform_member	member = get_transform_member( name );

		switch (member)
		{
			case colorTransform:
			{
				m_color_transform = cast_to<as_color_transform>( val.to_object() );
				m_movie->set_cxform( m_color_transform->m_color_transform );
				return true;
			}
			case concatenatedColorTransform:
			{
				//read-only
				return true;
			}
			case matrix:
			{
				assert( false && "todo" );
				return true;
			}
			case concatenatedMatrix:
			{
				//read-only
				return true;
			}
			case pixelBounds:
			{
				assert( false && "todo" );
				return true;
			}
			default:
                                break;
		};

		return as_object::set_member( name, val );
	}

	bool	as_transform::get_member(const tu_stringi& name, as_value* val)
	{
		as_transform_member	member = get_transform_member( name );

		switch( member )
		{
			case colorTransform:
			{
				val->set_as_object( m_color_transform.get_ptr() );
				return true;
			}
			case concatenatedColorTransform:
			{
				assert( false && "todo" );
				return true;
			}
			case matrix:
			{
				assert( false && "todo" );
				return true;
			}
			case concatenatedMatrix:
			{
				assert( false && "todo" );
				return true;
			}
			case pixelBounds:
			{
				assert( false && "todo" );
				return true;
			}
			default:
                                break;
		};

		return as_object::get_member( name, val );
	}

	as_transform_member	get_transform_member(const tu_stringi& name)
	{
		if (s_transform_member_map.size() == 0)
		{
			s_transform_member_map.set_capacity(int(AS_TRANSFORM_MEMBER_COUNT));
			s_transform_member_map.add("colorTransform", colorTransform);
			s_transform_member_map.add("concatenatedColorTransform", concatenatedColorTransform);
			s_transform_member_map.add("matrix", matrix);
			s_transform_member_map.add("concatenatedMatrix", concatenatedMatrix);
			s_transform_member_map.add("pixelBounds", pixelBounds);
		}

		as_transform_member	result = invalidMember;
		s_transform_member_map.get(name, &result);

		return result;
	}


}
