// as_color_transform.cpp	-- Julien Hamaide <julien.hamaide@gmail.com>	2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.


#include "as_transform.h"

namespace gameswf
{
	enum as_color_transform_member
	{
		redMultiplier = 1, 
		greenMultiplier, 
		blueMultiplier,
		alphaMultiplier,
		redOffset, 
		greenOffset,
		blueOffset, 
		alphaOffset,
		rgb,
		invalidMember,
		AS_COLOR_TRANSFORM_MEMBER_COUNT,
	};

	static stringi_hash<as_color_transform_member>	s_color_transform_member_map;
	static as_color_transform_member   get_color_transform_member(const tu_stringi& name);


	// ColorTranform([redMultiplier:Number], [greenMultiplier:Number], [blueMultiplier:Number], [alphaMultiplier:Number], [redOffset:Number], [greenOffset:Number], [blueOffset:Number], [alphaOffset:Number])
	void	as_global_color_transform_ctor(const fn_call& fn)
	{
		gc_ptr<as_color_transform>	obj;
		obj = new as_color_transform(fn.get_player());

		switch ( fn.nargs )
		{
			default:
				break;
			case alphaOffset:
				obj->m_color_transform.m_[3][1] = fn.arg(7).to_float();
			case blueOffset:
				obj->m_color_transform.m_[2][1] = fn.arg(6).to_float();
			case greenOffset:
				obj->m_color_transform.m_[1][1] = fn.arg(5).to_float();
			case redOffset:
				obj->m_color_transform.m_[0][1] = fn.arg(4).to_float();
			case alphaMultiplier:
				obj->m_color_transform.m_[3][0] = fn.arg(3).to_float();
			case blueMultiplier:
				obj->m_color_transform.m_[2][0] = fn.arg(2).to_float();
			case greenMultiplier:
				obj->m_color_transform.m_[1][0] = fn.arg(1).to_float();
			case redMultiplier:
				obj->m_color_transform.m_[0][0] = fn.arg(0).to_float();
		}

		 fn.result->set_as_object(obj.get_ptr());
	}

	as_color_transform::as_color_transform(player* player) :
		as_object(player)
	{
	}

	bool	as_color_transform::set_member(const tu_stringi& name, const as_value& val)
	{
		as_color_transform_member member = get_color_transform_member( name );

		switch( member )
		{
			case alphaOffset:
			{
				m_color_transform.m_[ 3 ][ 1 ] = val.to_float();
				return true;
			}

			case blueOffset:
			{
				m_color_transform.m_[ 2 ][ 1 ] = val.to_float();
				return true;
			}

			case greenOffset:
			{
				m_color_transform.m_[ 1 ][ 1 ] = val.to_float();
				return true;
			}

			case redOffset:
			{
				m_color_transform.m_[ 0 ][ 1 ] = val.to_float();
				return true;
			}

			case alphaMultiplier:
			{
				m_color_transform.m_[ 3 ][ 0 ] = val.to_float();
				return true;
			}

			case blueMultiplier:
			{
				m_color_transform.m_[ 2 ][ 0 ] = val.to_float();
				return true;
			}

			case greenMultiplier:
			{
				m_color_transform.m_[ 1 ][ 0 ] = val.to_float();
				return true;
			}

			case redMultiplier:
			{
				m_color_transform.m_[ 0 ][ 0 ] = val.to_float();
				return true;
			}

			case rgb:
			{
				int rgb;

				rgb = val.to_int();
				m_color_transform.m_[ 0 ][ 0 ] = 0.0f;
				m_color_transform.m_[ 1 ][ 0 ] = 0.0f;
				m_color_transform.m_[ 2 ][ 0 ] = 0.0f;
				m_color_transform.m_[ 3 ][ 0 ] = 0.0f;
				m_color_transform.m_[ 0 ][ 1 ] = static_cast<float>( ( rgb >> 16 ) & 0xFF );
				m_color_transform.m_[ 1 ][ 1 ] = static_cast<float>( ( rgb >> 8 ) & 0xFF );
				m_color_transform.m_[ 2 ][ 1 ] = static_cast<float>( rgb & 0xFF );
				m_color_transform.m_[ 3 ][ 1 ] = 255.0f;
				return true;
			}

			default:
				break;
		}

		return as_object::set_member( name, val );
	}

	bool	as_color_transform::get_member(const tu_stringi& name, as_value* val)
	{
		as_color_transform_member member = get_color_transform_member( name );

		switch( member )
		{
			case alphaOffset:
				val->set_double( m_color_transform.m_[ 3 ][ 1 ] );
				return true;

			case blueOffset:
				val->set_double( m_color_transform.m_[ 2 ][ 1 ] );
				return true;
		
			case greenOffset:
				val->set_double( m_color_transform.m_[ 1 ][ 1 ] );
				return true;

			case redOffset:
				val->set_double( m_color_transform.m_[ 0 ][ 1 ] );
				return true;
		
			case alphaMultiplier:
				val->set_double( m_color_transform.m_[ 3 ][ 0 ] );
				return true;
		
			case blueMultiplier:
				val->set_double( m_color_transform.m_[ 2 ][ 0 ] );
				return true;

			case greenMultiplier:
				val->set_double( m_color_transform.m_[ 1 ][ 0 ] );
				return true;
			
			case redMultiplier:
				val->set_double( m_color_transform.m_[ 0 ][ 0 ] );
				return true;

			case rgb:
				// TODO: Check behavior
				val->set_undefined();
				return true;

			default:
				break;
		}

		return as_object::get_member( name, val );
	}

	as_color_transform_member	get_color_transform_member(const tu_stringi& name)
	{
		if (s_color_transform_member_map.size() == 0)
		{
			s_color_transform_member_map.set_capacity(int(AS_COLOR_TRANSFORM_MEMBER_COUNT));
			s_color_transform_member_map.add("redMultiplier", redMultiplier);
			s_color_transform_member_map.add("greenMultiplier", greenMultiplier);
			s_color_transform_member_map.add("blueMultiplier", blueMultiplier);
			s_color_transform_member_map.add("alphaMultiplier", alphaMultiplier);
			s_color_transform_member_map.add("redOffset", redOffset);
			s_color_transform_member_map.add("greenOffset", greenOffset);
			s_color_transform_member_map.add("blueOffset", blueOffset);
			s_color_transform_member_map.add("alphaOffset", alphaOffset);
			s_color_transform_member_map.add("rgb", rgb);
		}

		as_color_transform_member	result = invalidMember;
		s_color_transform_member_map.get(name, &result);

		return result;
	}

}

