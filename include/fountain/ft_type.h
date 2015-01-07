#ifndef _FT_TYPE_H_
#define _FT_TYPE_H_

#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <fountain/ft_render.h>

namespace ftType {

bool init();

typedef struct
{
	//TODO: data of single charactor
	ftVec2 center;
	ftVec2 advance;
} charInfo;

class FontMan {
private:
	int picID;
	FT_Face face;
	FT_Bool useKerning;
	std::map<unsigned long, ftRender::SubImage> unicode2SubImage;
	std::map<unsigned long, ftType::charInfo> unicode2charInfo;

public:
	FontMan();
	~FontMan();
	bool loadFont(const char *fontname);
	void genAsciiTable(int h = 16);
	void genStringTable(const char *str, int h = 16);
	void drawString(const char *str, int startX, int startY);
};

};

#endif
