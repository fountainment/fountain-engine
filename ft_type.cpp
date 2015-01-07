#include <fountain/ft_type.h>
#include <fountain/ft_render.h>
#include <fountain/ft_algorithm.h>
#include <cstdio>
#include <vector>

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

void copyBitmapToBufferData(FT_Bitmap &bitmap, unsigned char *expanded_data, int imgW, int w, int h, int row, int col)
{
	for(int j = 0; j < h; j++) {
		for(int i = 0; i < w; i++) {
			int r = row * h + j;
			int c = col * w + i;
			expanded_data[2 * (r * imgW + c)] = 255;
			expanded_data[2 * (r * imgW + c) + 1] =
			    (i >= bitmap.width || j >= bitmap.rows) ?
			    0 : bitmap.buffer[i + bitmap.width * (bitmap.rows - 1 - j)];
		}
	}
}

FontMan::FontMan()
{
	picID = 0;
}

FontMan::~FontMan()
{
	//TODO: fix the bug (double free when delete the pic)
	//ftRender::deletePicture(picID);
}

bool FontMan::loadFont(const char *fontname)
{
	int error = FT_New_Face(library, fontname, 0, &face);
	if (error) {
		return false;
	}
	useKerning = FT_HAS_KERNING(face);
	return true;
}

//TODO: make these two functions less code
void FontMan::genAsciiTable(int h)
{
	//TODO: check the state of picID
	char asciiTable[129];
	asciiTable[0] = ' ';
	for (int i = 1; i < 128; i++)
		asciiTable[i] = i;
	asciiTable[128] = '\0';
	this->genStringTable(asciiTable, h);
}

void FontMan::genStringTable(const char *str, int h)
{
	//TODO: check the state of picID
	//TODO: create a SubImage to store default char
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
	FT_GlyphSlot slot = face->glyph;
	unsigned char* expanded_data = new unsigned char[2 * imgW * imgH];
	for (int ci = 0; ci < strSize; ci++) {
		FT_Load_Char(face, v[ci], FT_LOAD_RENDER);
		FT_Bitmap& bitmap = slot->bitmap;
		//TODO: save the slot's data in FontMan
		//std::printf("%d %d %d %d %ld\n", slot->bitmap_left, slot->bitmap_top - bitmap.rows, bitmap.width, bitmap.rows, (slot->advance.x >> 6));
		//TODO: gen SubImage
		int row = ci / cols;
		int col = ci % cols;
		copyBitmapToBufferData(bitmap, expanded_data, imgW, w, h, row, col);
	}
	picID = ftRender::getPicture(expanded_data, imgW, imgH, FT_GRAY_ALPHA);
	delete [] expanded_data;
}

//TODO: complete this function
void FontMan::drawString(const char *str, int startX, int startY)
{
	std::vector<unsigned long> s = ftAlgorithm::utf8toUnicode(str);
	ftVec2 pen(startX, startY);
	for (unsigned i = 0; i < s.size(); i++) {
		ftType::charInfo ch = unicode2charInfo[s[i]];
		ftRender::SubImage im = unicode2SubImage[s[i]];
		ftRender::transformBegin();
		  ftRender::ftTranslate(pen + ch.center);
		  ftRender::drawImage(im);
		ftRender::transformEnd();
		pen = pen + ch.advance;
	}
}
