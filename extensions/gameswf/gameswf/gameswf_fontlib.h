// gameswf_fontlib.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Internal interfaces to fontlib.


#ifndef GAMESWF_FONTLIB_H
#define GAMESWF_FONTLIB_H

#include "../base/tu_config.h"

#include "../base/container.h"
#include "gameswf_types.h"

namespace gameswf
{
	typedef gameswf::hash<int, glyph_entity> glyph_array;
	struct glyph_provider_tu : public glyph_provider
	{
		glyph_provider_tu();
		~glyph_provider_tu();

		virtual bitmap_info* get_char_image(character_def* shape_glyph, Uint16 code, 
			const tu_string& fontname, bool is_bold, bool is_italic, int fontsize,
			rect* bounds, float* advance);

		private:
			stringi_hash< glyph_array* > m_glyph;	// fontame-glyphs-glyph
	};

}	// end namespace gameswf



#endif // GAMESWF_FONTLIB_H

// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
