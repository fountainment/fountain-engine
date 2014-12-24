#include <fountain/ft_type.h>
#include <fountain/ft_render.h>
#include <cstdio>
#include <ftglyph.h>

using ftType::FontMan;

FT_Library library;

bool ftType::init()
{
	int error = FT_Init_FreeType(&library);
	if (error) {
		return false;
	}
	return true;
}

FontMan::FontMan()
{
	h = 16;
}

FontMan::~FontMan()
{
}

bool FontMan::loadFont(const char *fontname)
{
	int error = FT_New_Face(library, fontname, 0, &face);
	if (error) {
		return false;
	}
	return true;
}

void FontMan::genAsciiTable()
{
	FT_Set_Pixel_Sizes(face, 0, h);
	FT_Glyph glyph;
	int w = h;
	unsigned char* expanded_data = new unsigned char[2 * w * h * 16 * 8];
	for (unsigned char ch = 0; ch < 128; ch++) {
		FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT);
		FT_Get_Glyph(face->glyph, &glyph);
		FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
		FT_Bitmap& bitmap = bitmap_glyph->bitmap;
		int row = ch / 16;
		int col = ch % 16;
		int pos = row * 16 * 16 + col * 16;
		for(int j = 0; j < h; j++) {
			for(int i = 0; i < w; i++) {
				expanded_data[pos + 2 * (i + j * h)] = 255;
				expanded_data[pos + 2 * (i + j * h) + 1] =
				(i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width * j];
			}
		}
		//TODO: use the data to create texture
		//TODO: or get all the bitmaps to generate a big texture and use SubImage to draw
	}
	delete [] expanded_data;
}
