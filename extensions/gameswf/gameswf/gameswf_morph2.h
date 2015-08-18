// gameswf_morph2.h -- Mike Shaver <shaver@off.net> 2003, , Vitalij Alexeev <tishka92@mail.ru> 2004.

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#ifndef GAMESWF_MORPH2_H
#define GAMESWF_MORPH2_H

#include "gameswf_shape.h"


namespace gameswf
{
	struct morph2_character_def : public shape_character_def
	{
		morph2_character_def(player* player);
		virtual ~morph2_character_def();
		void	read(stream* in, int tag_type, bool with_style, movie_definition_sub* m);
		virtual void	display(character* inst);
		void lerp_matrix(matrix& t, const matrix& m1, const matrix& m2, const float ratio);

	private:

		shape_character_def m_shape1;
		shape_character_def m_shape2;
		unsigned int m_offset;
		float m_last_ratio;
		mesh_set*	m_mesh;
	};
}


#endif // GAMESWF_MORPH2_H

// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
