#ifndef _FT_TYPE_H_
#define _FT_TYPE_H_

#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <fountain/ft_render.h>

namespace ftType {

bool init();
void close();
bool isAlive();

class charInfo
{
public:
	charInfo();
	charInfo(const ftVec2 & ct, const ftVec2 & adv);
	ftVec2 center;
	ftVec2 advance;
};

class FontMan
{
private:
	int picID;
	FT_Face face;
	FT_Bool useKerning;
	std::map<unsigned long, ftRender::SubImage> unicode2SubImage;
	std::map<unsigned long, ftType::charInfo> unicode2charInfo;
	int fontSize;

public:
	FontMan();
	~FontMan();
	bool loadFont(const char *fontname);
	void genAsciiTable(int h = 16);
	void genStringTable(const char *str, int h = 16);
	int drawString(const char *str);
	int drawString(const std::vector<unsigned long> & s);
	int getStringLength(const std::vector<unsigned long> & s);
	int getFontSize();
};

}

extern ftType::FontMan *defaultFontMan;

#endif
