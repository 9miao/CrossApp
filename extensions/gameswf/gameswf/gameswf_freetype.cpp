// gameswf_freetype.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// TrueType font rasterizer based on freetype library
// Also serve as fontlib

#include "../base/tu_config.h"

#include "gameswf_render.h"
#include "gameswf_freetype.h"
#include "gameswf_log.h"
#include "gameswf_canvas.h"
#include "../base/utility.h"
#include "../base/container.h"

#ifdef _WIN32
	#include <Windows.h>
#else
#endif

namespace gameswf
{

#if TU_CONFIG_LINK_TO_FREETYPE == 1

	static FT_Library	m_lib;

	bool get_fontfile(const char* font_name, tu_string& file_name, bool is_bold, bool is_italic)
	// gets font file name by font name
	{

		if (font_name == NULL)
		{
			return false;
		}

#ifdef _WIN32

		//Vitaly: I'm not sure that this code works on all versions of Windows

		tu_stringi fontname = font_name;
		if (is_bold)
		{
			fontname += " Bold";
		}
		if (is_italic)
		{
			fontname +=  " Italic";
		}
		fontname += " (TrueType)";

		HKEY hKey;

		// try WinNT
		DWORD retCode = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			"Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 
			0, 
			KEY_ALL_ACCESS,
			&hKey);

		if (retCode != ERROR_SUCCESS)
		{
			// try Windows
			retCode = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				"Software\\Microsoft\\Windows\\CurrentVersion\\Fonts", 
				0, 
				KEY_ALL_ACCESS,
				&hKey);

			if (retCode != ERROR_SUCCESS)
			{
				return false;
			}
		}

		// Get the value of the 'windir' environment variable.
		tu_string windir(getenv("WINDIR"));

		// Get value count. 
		DWORD    cValues;              // number of values for key 
		retCode = RegQueryInfoKey(
			hKey,	// key handle 
			NULL,	// buffer for class name 
			NULL,	// size of class string 
			NULL,	// reserved 
			NULL,	// number of subkeys 
			NULL,	// longest subkey size 
			NULL,	// longest class string 
			&cValues,	// number of values for this key 
			NULL,	// longest value name 
			NULL,	// longest value data 
			NULL,	// security descriptor 
			NULL);	// last write time 

		// Enumerate the key values. 
		BYTE szValueData[MAX_PATH];
		TCHAR  achValue[MAX_PATH]; 
		for (DWORD i = 0, retCode = ERROR_SUCCESS; i < cValues; i++) 
		{ 
			DWORD cchValue = MAX_PATH; 
			DWORD dwValueDataSize = sizeof(szValueData) - 1;
			achValue[0] = '\0'; 
			retCode = RegEnumValueA(hKey, i, 
				achValue, 
				&cchValue, 
				NULL, 
				NULL,
				szValueData,
				&dwValueDataSize);

			if (retCode == ERROR_SUCCESS) 
			{ 
				if ((fontname == (char*) achValue) || ((strstr(achValue, font_name) != NULL) && !is_italic && !is_bold))
				{
					file_name = windir + tu_string("\\Fonts\\") + (char*) szValueData;
					RegCloseKey(hKey);
					return true;
				}
			} 
		}

		RegCloseKey(hKey);
		return false;

#else

	//TODO for Linux

	// hack
	if (strstr(font_name, "Times New Roman"))
	{
		file_name = "/usr/share/fonts/truetype/times";
	}
	else
	if (strstr(font_name, "Arial"))
	{
		file_name = "/usr/share/fonts/truetype/arial";
	}
	else
	{
		return false;
	}

	if (is_bold && is_italic)
	{
		file_name += "bi";
	}
	else
	if (is_bold)
	{
		file_name +=  "b";
	}
	else
	if (is_italic)
	{
		file_name +=  "b";
	}
	file_name += ".ttf";

	return true;

#endif
	}


	// 
	//	glyph provider implementation
	//

	glyph_freetype_provider::glyph_freetype_provider() :
		m_scale(0)
	{
	}

	glyph_freetype_provider::~glyph_freetype_provider()
	{
		m_face_entity.clear();

		int error = FT_Done_FreeType(m_lib);
		if (error)
		{
			fprintf(stderr, "FreeType provider: can't close FreeType!  error = %d\n", error);
		}
		m_lib = NULL;
	}

	//
	// Get image of character as bitmap
	//

	bitmap_info* glyph_freetype_provider::get_char_image(character_def* shape_glyph, Uint16 code, 
			const tu_string& fontname, bool is_bold,
			bool is_italic, int fontsize, rect* bounds, float* advance)
	{
		face_entity* fe = get_face_entity(fontname, is_bold, is_italic);
		if (fe == NULL)
		{
			return NULL;
		}

		// form hash key
		int key = (fontsize << 16) | code;

		// try to find the stored image of character
		glyph_entity* ge = NULL;
		if (fe->m_ge.get(key, &ge) == false)
		{
			FT_Set_Pixel_Sizes(fe->m_face, fontsize, fontsize);
			if (FT_Load_Char(fe->m_face, code, FT_LOAD_RENDER))
			{
				return NULL;
			}

			ge = new glyph_entity();

			image::alpha* im = draw_bitmap(fe->m_face->glyph->bitmap);
			ge->m_bi = render::create_bitmap_info_alpha(im->m_width, im->m_height, im->m_data);

			delete im;

			ge->m_bounds.m_x_max = float(fe->m_face->glyph->bitmap.width) /float(ge->m_bi->get_width());
			ge->m_bounds.m_y_max = float(fe->m_face->glyph->bitmap.rows) / float(ge->m_bi->get_height());

			ge->m_bounds.m_x_min = float(fe->m_face->glyph->metrics.horiBearingX) / float(fe->m_face->glyph->metrics.width);
			ge->m_bounds.m_y_min = float(fe->m_face->glyph->metrics.horiBearingY) / float(fe->m_face->glyph->metrics.height);
			ge->m_bounds.m_x_min *= -ge->m_bounds.m_x_max;
			ge->m_bounds.m_y_min *= ge->m_bounds.m_y_max;

			float scale = 16.0f / fontsize;	// hack
			ge->m_advance = (float) fe->m_face->glyph->metrics.horiAdvance * scale;

			// keep image of character
			fe->m_ge.add(key, ge);
		}

		if (bounds)
		{
			*bounds = ge->m_bounds;
		}

		if (advance)
		{
			*advance = ge->m_advance;
		}

		return ge->m_bi.get_ptr();
	}

	face_entity* glyph_freetype_provider::get_face_entity(const tu_string& fontname, bool is_bold, bool is_italic)
	{
		// form hash key
		tu_string key = fontname;
		if (is_bold)
		{
			key += "B";
		}
		if (is_italic)
		{
			key += "I";
		}

		// first try to find from hash
		gc_ptr<face_entity> fe;
		if (m_face_entity.get(key, &fe))
		{
			return fe.get_ptr();
		}

		tu_string font_filename;
		if (get_fontfile(fontname, font_filename, is_bold, is_italic) == false)
		{
			log_error("can't find font file '%s'\n", fontname.c_str());
			m_face_entity.add(key, NULL);
			return NULL;
		}

		FT_Face face = NULL;
		FT_New_Face(m_lib, font_filename.c_str(), 0, &face);
		if (face)
		{
			if (is_bold)
			{
				face->style_flags |= FT_STYLE_FLAG_BOLD;
			}

			if (is_italic)
			{
				face->style_flags |= FT_STYLE_FLAG_ITALIC;
			}

			fe = new face_entity(face);
			m_face_entity.add(key, fe);
		}
		else
		{
			log_error("some error opening font '%s'\n", font_filename.c_str());
		}
		return fe.get_ptr();
	}

	image::alpha* glyph_freetype_provider::draw_bitmap(const FT_Bitmap& bitmap)
	{
		// You must use power-of-two dimensions!!
		int	w = 1; while (w <= bitmap.width) { w <<= 1; }
		int	h = 1; while (h <= bitmap.rows)  { h <<= 1; }

		image::alpha* alpha = image::create_alpha(w, h);
		memset(alpha->m_data, 0,  w * h);

		for (int j = 0; j < bitmap.rows; ++j)
		{
			Uint8* dst = alpha->m_data + j * w;
			const Uint8* src = bitmap.buffer + j * bitmap.width;
			for (int i = 0; i < bitmap.width; ++i)
			{
				//since w and h have a good chance of getting larger than the bitmaps height and/or width
				*dst++ = *src++;
			}
		}

		return alpha;
	}

	//
	// freetype callbacks, called from freetype lib  through get_char_def()
	//

	// static
	int glyph_freetype_provider::move_to_callback(FT_CONST FT_Vector* to, void* user)
	{
		glyph_freetype_provider* _this = static_cast<glyph_freetype_provider*>(user);
		_this->m_canvas->move_to(to->x * _this->m_scale, - to->y * _this->m_scale);
		return 0;
	}
	
	// static 
	int glyph_freetype_provider::line_to_callback(FT_CONST FT_Vector* to, void* user)
	{
		glyph_freetype_provider* _this = static_cast<glyph_freetype_provider*>(user);
		_this->m_canvas->line_to(to->x * _this->m_scale, - to->y * _this->m_scale);
		return 0;
	}

	//static
	int glyph_freetype_provider::conic_to_callback(FT_CONST FT_Vector* ctrl, FT_CONST FT_Vector* to,
		void* user)
	{
		glyph_freetype_provider* _this = static_cast<glyph_freetype_provider*>(user);
		_this->m_canvas->curve_to(ctrl->x * _this->m_scale, - ctrl->y * _this->m_scale, 
				to->x * _this->m_scale, - to->y * _this->m_scale);
		return 0;
	}

	//static
	int glyph_freetype_provider::cubic_to_callback(FT_CONST FT_Vector* ctrl1, FT_CONST FT_Vector* ctrl2,
			FT_CONST FT_Vector* to, void* user)
	{
		glyph_freetype_provider* _this = static_cast<glyph_freetype_provider*>(user);
		float x = float(ctrl1->x + ((ctrl2->x - ctrl1->x) * 0.5));
		float y = float(ctrl1->y + ((ctrl2->y - ctrl1->y) * 0.5));
		_this->m_canvas->curve_to(x * _this->m_scale, - y * _this->m_scale, 
				to->x * _this->m_scale, - to->y * _this->m_scale);
		return 0;
	}

	//
	// Get image of character as shape
	//

	shape_character_def* glyph_freetype_provider::get_char_def(Uint16 code, 
		const char* fontname, bool is_bold, bool is_italic, int fontsize,
		rect* box, float* advance)
	{
		assert(0);	// FIXME
/*
		FT_Face face = 0; //get_face(fontname, is_bold, is_italic);
		if (face == NULL)
		{
			return NULL;
		}

		if (FT_Load_Char(face, code, FT_LOAD_NO_BITMAP | FT_LOAD_NO_SCALE))
		{
			return NULL;
		}

		if (face->glyph->format != FT_GLYPH_FORMAT_OUTLINE)
		{
			return NULL;
		}


		m_canvas = new canvas();
		m_canvas->begin_fill(rgba(255, 255, 255, 255));

		FT_Outline_Funcs callback_func;
 		callback_func.move_to = move_to_callback;
		callback_func.line_to = line_to_callback;
		callback_func.conic_to = conic_to_callback;
		callback_func.cubic_to = cubic_to_callback;

		m_scale = 1024.0f / face->units_per_EM;
		*advance = (float) face->glyph->metrics.horiAdvance * m_scale;

		FT_Outline_Decompose(&face->glyph->outline, &callback_func, this);

		m_canvas->end_fill();

		return m_canvas.get_ptr();
		*/
		return 0;
	}

	glyph_provider*	create_glyph_provider_freetype()
	{
		int	error = FT_Init_FreeType(&m_lib);
		if (error)
		{
			fprintf(stderr, "FreeType provider: can't init FreeType!  error = %d\n", error);
			return NULL;
		}
		return new glyph_freetype_provider();
	}
#endif

}	// end of namespace gameswf
