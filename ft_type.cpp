#include <fountain/ft_type.h>
#include <fountain/ft_render.h>
#include <fountain/ft_algorithm.h>
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
	//TODO: fix the bug
	//ftRender::deletePicture(picID);
}

bool FontMan::loadFont(const char *fontname)
{
	int error = FT_New_Face(library, fontname, 0, &face);
	if (error) {
		return false;
	}
	return true;
}

//TODO: make these two functions less code
void FontMan::genAsciiTable(int h)
{
	//TODO: check the state of picID
	h = ftAlgorithm::nextPower2(h);
	int w = h;
	int cols = 16;
	int rows = 8;
	int imgW = w * cols;
	int imgH = h * rows;

	FT_Set_Pixel_Sizes(face, 0, h);
	FT_Glyph glyph;
	unsigned char* expanded_data = new unsigned char[2 * imgW * imgH];
	for (unsigned char ch = 0; ch < 128; ch++) {
		FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT);
		FT_Get_Glyph(face->glyph, &glyph);
		FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
		FT_Bitmap& bitmap = bitmap_glyph->bitmap;
		int row = ch / cols;
		int col = ch % cols;
		for(int j = 0; j < h; j++) {
			for(int i = 0; i < w; i++) {
				int r = row * h + j;
				int c = col * w + i;
				expanded_data[2 * (r * imgW + c)] = 255;
				expanded_data[2 * (r * imgW + c) + 1] =
				    (i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width * (bitmap.rows - 1 - j)];
			}
		}
	}
	//TODO: use SubImage to draw?
	picID = ftRender::getPicture(expanded_data, imgW, imgH, FT_GRAY_ALPHA);
	delete [] expanded_data;
}

void FontMan::genStringTable(const char *str, int h)
{
	//TODO: check the state of picID
	h = ftAlgorithm::nextPower2(h);
	std::vector<unsigned long> v = ftAlgorithm::utf8toUnicode(str);
	int strSize = v.size();
	int size = ftAlgorithm::nextPower2(strSize);
	int cols = 1;
	while (size / cols > cols) cols <<= 1;
	int rows = size / cols;
	int w = h;
	int imgW = w * cols;
	int imgH = h * rows;

	FT_Set_Pixel_Sizes(face, 0, h);
	FT_Glyph glyph;
	unsigned char* expanded_data = new unsigned char[2 * imgW * imgH];
	for (int ci = 0; ci < strSize; ci++) {
		FT_Load_Glyph(face, FT_Get_Char_Index(face, v[ci]), FT_LOAD_DEFAULT);
		FT_Get_Glyph(face->glyph, &glyph);
		FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
		FT_Bitmap& bitmap = bitmap_glyph->bitmap;
		int row = ci / cols;
		int col = ci % cols;
		for(int j = 0; j < h; j++) {
			for(int i = 0; i < w; i++) {
				int r = row * h + j;
				int c = col * w + i;
				expanded_data[2 * (r * imgW + c)] = 255;
				expanded_data[2 * (r * imgW + c) + 1] =
				    (i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width * (bitmap.rows - 1 - j)];
			}
		}
	}
	//TODO: use SubImage to draw?
	picID = ftRender::getPicture(expanded_data, imgW, imgH, FT_GRAY_ALPHA);
	delete [] expanded_data;
}
