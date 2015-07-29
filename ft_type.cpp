#include <fountain/fountaindef.h>
#include <cstdio>
#include <vector>

using ftType::charInfo;
using ftType::FontMan;

static bool alive = false;
FT_Library library;

FontMan *defaultFontMan;

bool ftType::init()
{
	int error = FT_Init_FreeType(&library);
	if (error) {
		return false;
	}
	alive = true;
	return true;
}

void ftType::close()
{
	FT_Done_FreeType(library);
	alive = false;
}

bool ftType::isAlive()
{
	return alive;
}

//class ftType::charInfo
charInfo::charInfo()
{
	center = ftVec2(0.0f, 0.0f);
	advance = ftVec2(0.0f, 0.0f);
}

charInfo::charInfo(const ftVec2 & ct, const ftVec2 & adv)
{
	center = ct;
	advance = adv;
}

void copyBitmapToBufferData(FT_Bitmap &bitmap, unsigned char *expanded_data, int imgW, unsigned w, unsigned h, int row, int col)
{
	for(unsigned j = 0; j < h; j++) {
		for(unsigned i = 0; i < w; i++) {
			int r = row * h + j;
			int c = col * w + i;
			expanded_data[2 * (r * imgW + c)] = 255;
			expanded_data[2 * (r * imgW + c) + 1] =
			    (i >= bitmap.width || j >= bitmap.rows) ?
			    0 : bitmap.buffer[i + bitmap.width * (bitmap.rows - 1 - j)];
		}
	}
}

//class ftType::FontMan
FontMan::FontMan()
{
	picID = 0;
}

FontMan::~FontMan()
{
	if (ftType::isAlive()) FT_Done_Face(face);
	if (ftRender::isAlive()) ftRender::deletePicture(picID);
	if (defaultFontMan == this) defaultFontMan = NULL;
}

bool FontMan::loadFont(const char *fontname)
{
	int error = FT_New_Face(library, fontname, 0, &face);
	if (error) {
		FT_ERROR("ftType: \"%s\" loading error!\n", fontname);
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
	std::vector<ftRect> rectV;
	for (int ci = 0; ci < strSize; ci++) {
		FT_Load_Char(face, v[ci], FT_LOAD_RENDER);
		FT_Bitmap& bitmap = slot->bitmap;
		ftVec2 center(slot->bitmap_left + bitmap.width / 2.0f,
		              slot->bitmap_top - bitmap.rows / 2.0f);
		ftVec2 advance(slot->advance.x >> 6, slot->advance.y >> 6);
		charInfo ch(center, advance);
		unicode2charInfo[v[ci]] = ch;
		int row = ci / cols;
		int col = ci % cols;
		ftRect si(col * w, row * h, bitmap.width, bitmap.rows);
		rectV.push_back(si);
		copyBitmapToBufferData(bitmap, expanded_data, imgW, w, h, row, col);
	}
	picID = ftRender::getPicture(expanded_data, imgW, imgH, FT_GRAY_ALPHA);
	for (int ci = 0; ci < strSize; ci++) {
		ftRender::SubImage t(picID, rectV[ci]);
		unicode2SubImage[v[ci]] = t;
	}
	delete [] expanded_data;
	fontSize = h;
}

//TODO: complete this function
int FontMan::drawString(const std::vector<unsigned long> & s)
{
	ftVec2 pen(0, 0);
	FT_Int previous = 0;
	FT_UInt glyphIndex;
	for (unsigned i = 0; i < s.size(); i++) {
		if (s[i] == '\n') {
			pen.y -= getFontSize();
			pen.x = 0;
			continue;
		}
		charInfo ch = unicode2charInfo[s[i]];
		ftRender::SubImage im = unicode2SubImage[s[i]];
		ftRender::transformBegin();
		ftRender::ftTranslate(pen + ch.center);
		ftRender::drawImage(im);
		ftRender::transformEnd();
		pen = pen + ch.advance;
		//kerning
		glyphIndex = FT_Get_Char_Index(face, s[i]);
		if (useKerning && previous && glyphIndex) {
			FT_Vector delta;
			FT_Get_Kerning(face, previous, glyphIndex, FT_KERNING_DEFAULT, &delta);
			pen.x += delta.x >> 6;
		}
		previous = glyphIndex;
	}
	return pen.x;
}

int FontMan::getStringLength(const std::vector<unsigned long> & s)
{
	ftVec2 pen(0, 0);
	FT_Int previous = 0;
	FT_UInt glyphIndex;
	for (unsigned i = 0; i < s.size(); i++) {
		charInfo ch = unicode2charInfo[s[i]];
		pen = pen + ch.advance;
		//kerning
		glyphIndex = FT_Get_Char_Index(face, s[i]);
		if (useKerning && previous && glyphIndex) {
			FT_Vector delta;
			FT_Get_Kerning(face, previous, glyphIndex, FT_KERNING_DEFAULT, &delta);
			pen.x += delta.x >> 6;
		}
		previous = glyphIndex;
	}
	return pen.x;
}

int FontMan::drawString(const char *str)
{
	std::vector<unsigned long> s = ftAlgorithm::utf8toUnicode(str);
	return drawString(s);
}

int FontMan::getFontSize()
{
	return fontSize;
}
