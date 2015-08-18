// gameswf_character.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Some implementation code for the gameswf SWF player library.


#include "gameswf_character.h"
#include "gameswf_render.h"

namespace gameswf
{

	character::character(player* player, character* parent, int id)	:
		as_object(player),
		m_id(id),
		m_parent(parent),
		m_depth(0),
		m_ratio(0.0f),
		m_clip_depth(0),
		m_blend_mode(0),
		m_visible(true),
		m_display_callback(NULL),
		m_display_callback_user_ptr(NULL)
	{
		// loadMovieClip() requires that the following will be commented out
		// assert((parent == NULL && m_id == -1)	|| (parent != NULL && m_id >= 0));
	}

	bool	character::get_member(const tu_stringi& name, as_value* val)
	// Set *val to the value of the named member and
	// return true, if we have the named member.
	// Otherwise leave *val alone and return false.
	{
		// first try character members
		as_standard_member	std_member = get_standard_member(name);
		switch (std_member)
		{
			default:
				break;

			case M_GLOBAL:
				val->set_as_object(get_global());
				return true;

			case M_ROOT:
			case M_LEVEL0:
				val->set_as_object(get_root_movie());
				return true;

			case M_THIS:
			case MDOT:
				val->set_as_object(this);
				return true;

			case M_X:
			{
				const matrix&	m = get_matrix();
				val->set_double(TWIPS_TO_PIXELS(m.m_[0][2]));
				return true;
			}
			case M_Y:
			{
				const matrix&	m = get_matrix();
				val->set_double(TWIPS_TO_PIXELS(m.m_[1][2]));
				return true;
			}
			case M_XSCALE:
			{
				const matrix& m = get_matrix();	// @@ or get_world_matrix()?  Test this.
				float xscale = m.get_x_scale();
				val->set_double(xscale * 100);		// result in percent
				return true;
			}
			case M_YSCALE:
			{
				const matrix& m = get_matrix();	// @@ or get_world_matrix()?  Test this.
				float yscale = m.get_y_scale();
				val->set_double(yscale * 100);		// result in percent
				return true;
			}
			case M_ALPHA:
			{
				// Alpha units are in percent.
				val->set_double(get_cxform().m_[3][0] * 100.f);
				return true;
			}
			case M_VISIBLE:
			{
				val->set_bool(get_visible());
				return true;
			}
			case M_WIDTH:
			{
				val->set_double((int) TWIPS_TO_PIXELS(get_width()));
				return true;
			}
			case M_HEIGHT:
			{
				val->set_double((int) TWIPS_TO_PIXELS(get_height()));
				return true;
			}
			case M_ROTATION:
			{
				// Verified against Macromedia player using samples/test_rotation.swf
				float	angle = get_matrix().get_rotation();

				// Result is CLOCKWISE DEGREES, [-180,180]
				angle *= 180.0f / float(M_PI);

				val->set_double(angle);
				return true;
			}
			case M_TARGET:
			{
				// Full path to this object; e.g. "/_level0/sprite1/sprite2/ourSprite"
				character* parent = get_parent();
				if (parent == NULL)	// root
				{
					val->set_tu_string("/");
					return true;
				}

				as_value target;
				parent->get_member("_target", &target);

					// if s != "/"(root) add "/"
				tu_string s = target.to_tu_string();
				s += s == "/" ? "" : "/";

				// add own name
				if (get_name().length() == 0)
				{
					s += "noname";
				}
				else
				{
					s += get_name();
				}

				val->set_tu_string(s);
				return true;
			}
			case M_NAME:
			{
				val->set_tu_string(get_name());
				return true;
			}
			case M_DROPTARGET:
			{
				// Absolute path in slash syntax where we were last dropped (?)
				// @@ TODO
				val->set_string("/_root");
				return true;
			}
			case M_URL:
			{
				// our URL.
				val->set_string("gameswf");
				return true;
			}
			case M_HIGHQUALITY:
			{
				// Whether we're in high quality mode or not.
				val->set_bool(true);
				return true;
			}
			case M_FOCUSRECT:
			{
				// Is a yellow rectangle visible around a focused movie clip (?)
				val->set_bool(false);
				return true;
			}
			case M_SOUNDBUFTIME:
			{
				// Number of seconds before sound starts to stream.
				val->set_double(0.0);
				return true;
			}
			case M_XMOUSE:
			{
				// Local coord of mouse IN PIXELS.
				int	x, y, buttons;
				get_mouse_state(&x, &y, &buttons);

				matrix	m = get_world_matrix();

				point	a(PIXELS_TO_TWIPS(x), PIXELS_TO_TWIPS(y));
				point	b;

				m.transform_by_inverse(&b, a);

				val->set_double(TWIPS_TO_PIXELS(b.m_x));
				return true;
			}
			case M_YMOUSE:
			{
				// Local coord of mouse IN PIXELS.
				int	x, y, buttons;
				get_mouse_state(&x, &y, &buttons);

				matrix	m = get_world_matrix();

				point	a(PIXELS_TO_TWIPS(x), PIXELS_TO_TWIPS(y));
				point	b;

				m.transform_by_inverse(&b, a);

				val->set_double(TWIPS_TO_PIXELS(b.m_y));
				return true;
			}
			case M_PARENT:
			case MDOT2:
			{
				val->set_as_object(cast_to<as_object>(m_parent.get_ptr()));
				return true;
			}
		}	// end switch

		return as_object::get_member(name, val);
	}

	// TODO: call_watcher
	bool	character::set_member(const tu_stringi& name, const as_value& val)
	{
		// first try character members
		as_standard_member	std_member = get_standard_member(name);
		switch (std_member)
		{
			default:
				break;

			case M_X:
			{
				if( !isnan( val.to_number() ) )
				{
					matrix	m = get_matrix();
					m.m_[0][2] = (float) PIXELS_TO_TWIPS(val.to_number());
					set_matrix(m);
				}
				return true;
			}
			case M_Y:
			{
				if( !isnan( val.to_number() ) )
				{
					matrix	m = get_matrix();
					m.m_[1][2] = (float) PIXELS_TO_TWIPS(val.to_number());
					set_matrix(m);
				}
				return true;
			}
			case M_XSCALE:
			{
				matrix	m = get_matrix();

				// Decompose matrix and insert the desired value.
				float	x_scale = val.to_float() / 100.f;	// input is in percent
				float	y_scale = m.get_y_scale();
				float	rotation = m.get_rotation();
				m.set_scale_rotation(x_scale, y_scale, rotation);

				set_matrix(m);
				return true;
			}
			case M_YSCALE:
			{
				matrix	m = get_matrix();

				// Decompose matrix and insert the desired value.
				float	x_scale = m.get_x_scale();
				float	y_scale = val.to_float() / 100.f;	// input is in percent
				float	rotation = m.get_rotation();
				m.set_scale_rotation(x_scale, y_scale, rotation);

				set_matrix(m);
				return true;
			}
			case M_ALPHA:
			{
				// Set alpha modulate, in percent.
				cxform	cx = get_cxform();
				cx.m_[3][0] = val.to_float() / 100.f;
				set_cxform(cx);
				return true;
			}
			case M_VISIBLE:
			{
				set_visible(val.to_bool());
				return true;
			}
			case M_WIDTH:
			{
				if (val.to_float() > 0)
				{
					matrix	m = get_matrix();

					// Decompose matrix and insert the desired value.
					float	x_scale = m.get_x_scale();
					float	y_scale = m.get_y_scale();
					float	rotation = m.get_rotation();

					// get current width
					float current_width = TWIPS_TO_PIXELS(get_width());

					// set new scale
					x_scale /= current_width / val.to_float();

					m.set_scale_rotation(x_scale, y_scale, rotation);
					set_matrix(m);
				}
				return true;
			}
			case M_HEIGHT:
			{
				if (val.to_float() > 0)
				{
					matrix	m = get_matrix();

					// Decompose matrix and insert the desired value.
					float	x_scale = m.get_x_scale();
					float	y_scale = m.get_y_scale();
					float	rotation = m.get_rotation();

					// get current height
					float current_height = TWIPS_TO_PIXELS(get_height());

					// set new scale
					y_scale /= current_height / val.to_float();

					m.set_scale_rotation(x_scale, y_scale, rotation);
					set_matrix(m);
				}
				return true;
			}
			case M_ROTATION:
			{
				matrix	m = get_matrix();

				// Decompose matrix and insert the desired value.
				float	x_scale = m.get_x_scale();
				float	y_scale = m.get_y_scale();
				float	rotation = val.to_float() * float(M_PI) / 180.f;	// input is in degrees
				m.set_scale_rotation(x_scale, y_scale, rotation);

				set_matrix(m);
				return true;
			}
			case M_NAME:
			{
				set_name(val.to_tu_string());
				return true;
			}
			case M_HIGHQUALITY:
			{
				// @@ global { 0, 1, 2 }
				//				// Whether we're in high quality mode or not.
				//				val->set(true);
				return true;
			}
			case M_FOCUSRECT:
			{
				//				// Is a yellow rectangle visible around a focused movie clip (?)
				//				val->set(false);
				return true;
			}
			case M_SOUNDBUFTIME:
			{
				// @@ global
				//				// Number of seconds before sound starts to stream.
				//				val->set(0.0);
				return true;
			}
		}	// end switch

		return as_object::set_member(name, val);
	}

	float	character::get_width()
	{
		rect bound;
		get_bound(&bound);
		float w = bound.m_x_max - bound.m_x_min;
		return w >= FLT_MIN ? w : 0;
	}

	float	character::get_height()
	{
		rect bound;
		get_bound(&bound);
		float h = bound.m_y_max - bound.m_y_min;
		return h >= FLT_MIN ? h : 0;
	}

	void character::get_bound(rect* bound)
	{
		character_def* def = get_character_def();
		assert(def);
		def->get_bound(bound);
		get_matrix().transform(bound);
	}


//	bool	character::is_visible()
//	{
		// The increase of performance that gives the skipping of invisible characters
		// less than expenses for performance of test of visibility
		// get_bound() & get_world_matrix() are heavy recursive operations
//		return true;

//		rect bound;
//		get_bound(&bound);
		
//		matrix m;
//		m.set_inverse(get_matrix());
//		m.transform(&bound);

//		m = get_world_matrix();
//		m.transform(&bound);

//		return render::is_visible(bound);
//	}

	void character::enumerate(as_environment* env)
	{
		as_object::enumerate(env);
	}

}
