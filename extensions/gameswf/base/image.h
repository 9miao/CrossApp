// image.h	-- Thatcher Ulrich <tu@tulrich.com> 2002

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Handy image utilities for RGB surfaces.

#ifndef IMAGE_H
#define IMAGE_H


#include "tu_config.h"
#include "tu_types.h"
#include "container.h"

class tu_file;
namespace jpeg { struct input; };

// misc, get power-of-two dimension
int p2(int n);

namespace image
{
	struct image_base
	{
		enum id_image
		{
			INVALID,
			RGB,
			RGBA,
			ALPHA
		};

		id_image m_type;
		Uint8*	m_data;
		int	m_width;
		int	m_height;
		int	m_pitch;	// byte offset from one row to the next

		image_base(Uint8* data, int width, int height, int pitch, id_image type);
		virtual ~image_base();
	};

	// 24-bit RGB image.  Packed data, red byte first (RGBRGB...)
	struct rgb : public image_base
	{
		rgb(int width, int height);
		~rgb();
	};

	// 32-bit RGBA image.  Packed data, red byte first (RGBARGBA...)
	struct rgba : public image_base
	{
		rgba(int width, int height);
		~rgba();

		void	set_pixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	};

	// 8-bit alpha image.
	struct alpha : public image_base
	{
		alpha(int width, int height);
		~alpha();

		void	set_pixel(int x, int y, Uint8 a);

		// Bitwise content comparison.
		bool	operator==(const alpha& a) const;

		// Return a hash code based on the image contents.
		unsigned int	compute_hash() const;
	};


	// Make a system-memory 24-bit bitmap surface.  24-bit packed
	// data, red byte first.
	rgb*	create_rgb(int width, int height);


	// Make a system-memory 32-bit bitmap surface.  Packed data,
	// red byte first.
	rgba*	create_rgba(int width, int height);


	// Make a system-memory 8-bit bitmap surface.
	alpha*	create_alpha(int width, int height);

	
	Uint8*	scanline(image_base* surf, int y);
	const Uint8*	scanline(const image_base* surf, int y);


	void	resample(rgb* out, int out_x0, int out_y0, int out_x1, int out_y1,
			 rgb* in, float in_x0, float in_y0, float in_x1, float in_y1);

	void	resample(rgba* out, int out_x0, int out_y0, int out_x1, int out_y1,
			 rgba* in, float in_x0, float in_y0, float in_x1, float in_y1);

	void	zoom(image_base* src, image_base* dst);

	void	write_jpeg(tu_file* out, rgb* image, int quality);
	void	write_tga(tu_file* out, rgba* image);

	exported_module rgb*	read_jpeg(const char* filename);
	exported_module rgb*	read_jpeg(tu_file* in);

	// For reading SWF JPEG2-style image data (slight variation on
	// ordinary JPEG).
	rgb*	read_swf_jpeg2(tu_file* in);

	// For reading SWF JPEG2-style image data, using pre-loaded
	// headers stored in the given jpeg::input object.
	rgb*	read_swf_jpeg2_with_tables(jpeg::input* loader);

	// For reading SWF JPEG3-style image data, like ordinary JPEG, 
	// but stores the data in rgba format.
	rgba*	read_swf_jpeg3(tu_file* in);

	// Fast, in-place, DESTRUCTIVE resample.  For making mip-maps.
	// Munges the input image to produce the output image.
	void	make_next_miplevel(rgb* image);
	void	make_next_miplevel(rgba* image);
};


#endif // IMAGE_H

// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
