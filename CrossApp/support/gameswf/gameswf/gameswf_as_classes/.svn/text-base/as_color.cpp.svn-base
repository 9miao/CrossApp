// as_color.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// The Color class lets you set the RGB color value and color transform
// of movie clips and retrieve those values once they have been set. 

#include "gameswf/gameswf_as_classes/as_color.h"

namespace gameswf
{

	// Color(target:Object)
	void	as_global_color_ctor(const fn_call& fn)
	{
		if (fn.nargs == 1)
		{
			character* target = cast_to<character>(fn.arg(0).to_object());
			if (target)
			{
				fn.result->set_as_object(new as_color(fn.get_player(), target));
			}
		}
	}


	void	as_color_getRGB(const fn_call& fn)
	{
		as_color* obj = cast_to<as_color>(fn.this_ptr);
		if (obj == NULL)
		{
			return;
		}
		
		if (obj->m_target == NULL)
		{
			return;
		}

		cxform	cx = obj->m_target->get_cxform();
		Uint8 r = (Uint8) ceil(cx.m_[0][0] * 255.0f);
		Uint8 g = (Uint8) ceil(cx.m_[1][0] * 255.0f);
		Uint8 b = (Uint8) ceil(cx.m_[2][0] * 255.0f);

		fn.result->set_int(r << 16 | g << 8 | b);
	}

	void	as_color_setRGB(const fn_call& fn)
	{
		if (fn.nargs < 1)
		{
			return;
		}

		as_color* obj = cast_to<as_color>(fn.this_ptr);
		if (obj == NULL)
		{
			return;
		}
			
		if (obj->m_target == NULL)
		{
			return;
		}

		cxform	cx = obj->m_target->get_cxform();
		rgba color(fn.arg(0).to_number());
		cx.m_[0][0] = float(color.m_r) / 255.0f;
		cx.m_[1][0] = float(color.m_g) / 255.0f;
		cx.m_[2][0] = float(color.m_b) / 255.0f;
		obj->m_target->set_cxform(cx);
	}

	// TODO: Fix gettransform()
	void	as_color_gettransform(const fn_call& fn)
	{
		as_color* obj = cast_to<as_color>(fn.this_ptr);
		if (obj == NULL)
		{
			return;
		}
			
		if (obj->m_target == NULL)
		{
			return;
		}

		cxform	cx = obj->m_target->get_cxform();
		Uint8 r = (Uint8) ceil(cx.m_[0][0] * 255.0f);
		Uint8 g = (Uint8) ceil(cx.m_[1][0] * 255.0f);
		Uint8 b = (Uint8) ceil(cx.m_[2][0] * 255.0f);
		Uint8 a = (Uint8) ceil(cx.m_[3][0] * 255.0f);

		as_object* tobj = new as_object(fn.get_player());
		tobj->set_member("ra", r / 255.0f * 100.0f);	// percent (-100..100)
		tobj->set_member("rb", r);	// value	(-255..255)
		tobj->set_member("ga", g / 255.0f * 100.0f);	// percent (-100..100)
		tobj->set_member("gb", g);	// value	(-255..255)
		tobj->set_member("ba", b / 255.0f * 100.0f);	// percent (-100..100)
		tobj->set_member("bb", b);	// value	(-255..255)
		tobj->set_member("aa", a / 255.0f * 100.0f);	// percent (-100..100)
		tobj->set_member("ab", a);	// value	(-255..255)

		fn.result->set_as_object(tobj);
	}

	void	as_color_settransform(const fn_call& fn)
	{
		if (fn.nargs < 1)
		{
			return;
		}

		as_color* obj = cast_to<as_color>(fn.this_ptr);
		if (obj == NULL || obj->m_target == NULL)
		{
			return;
		}
			
		as_object* tobj = fn.arg(0).to_object();
		if (tobj)
		{
			cxform	cx = obj->m_cxform;
			as_value v;

			if (tobj->get_member("ra", &v))
			{
				cx.m_[0][0] *= v.to_float() / 100.0f;
			}
			else
			if (tobj->get_member("rb", &v))
			{
				cx.m_[0][0] = v.to_float() / 255.0f;
			}

			if (tobj->get_member("ga", &v))
			{
				cx.m_[1][0] *= v.to_float() / 100.0f;
			}
			else
			if (tobj->get_member("gb", &v))
			{
				cx.m_[1][0] = v.to_float() / 255.0f;
			}

			if (tobj->get_member("ba", &v))
			{
				cx.m_[2][0] *= v.to_float() / 100.0f;
			}
			else
			if (tobj->get_member("bb", &v))
			{
				cx.m_[2][0] = v.to_float() / 255.0f;
			}

			if (tobj->get_member("aa", &v))
			{
				cx.m_[3][0] *= v.to_float() / 100.0f;
			}
			else
			if (tobj->get_member("ab", &v))
			{
				cx.m_[3][0] = v.to_float() / 255.0f;
			}

			obj->m_target->set_cxform(cx);
		}		
	}

	as_color::as_color(player* player, character* target) :
		as_object(player),
		m_target(target)
	{
		assert(target);
		m_cxform = target->get_cxform();

		builtin_member("getRGB", as_color_getRGB);
		builtin_member("setRGB", as_color_setRGB);
		builtin_member("getTransform", as_color_gettransform);
		builtin_member("setTransform", as_color_settransform);
		set_ctor(as_global_color_ctor);
	}

};
