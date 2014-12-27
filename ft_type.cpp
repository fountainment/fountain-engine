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
	picID = 0;
}

FontMan::~FontMan()
{
	//TODO: release the pic generated
}

bool FontMan::loadFont(const char *fontname)
{
	int error = FT_New_Face(library, fontname, 0, &face);
	if (error) {
		return false;
	}
	return true;
}

void FontMan::genAsciiTable(int h)
{
	//TODO: check the state of picID
	FT_Set_Pixel_Sizes(face, 0, h);
	FT_Glyph glyph;
	int w = h;
	int imgW = w * 16;
	int imgH = h * 8;
	unsigned char* expanded_data = new unsigned char[2 * imgW * imgH];
	for (unsigned char ch = 0; ch < 128; ch++) {
		FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT);
		FT_Get_Glyph(face->glyph, &glyph);
		FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
		FT_Bitmap& bitmap = bitmap_glyph->bitmap;
		int row = ch / 16;
		int col = ch % 16;
		for(int j = 0; j < h; j++) {
			for(int i = 0; i < w; i++) {
				//int r = (8 * h - 1) - (row * h + j);
				int r = row * h + j;
				int c = col * w + i;
				expanded_data[2 * (r * imgW + c)] = 255;
				expanded_data[2 * (r * imgW + c) + 1] =
				    (i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width * (bitmap.rows - 1 - j)];
			}
		}
	}
	//TODO: use SubImage to draw?
	picID = ftRender::getPicture(expanded_data, w * 16, h * 8, FT_GRAY_ALPHA);
	delete [] expanded_data;
}
