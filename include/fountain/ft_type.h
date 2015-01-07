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
} charInfo;

class FontMan {
private:
	FT_Face face;
	std::map<unsigned long, ftRender::SubImage> unicode2SubImage;
	std::map<unsigned long, ftType::charInfo> unicode2charInfo;

public:
	//EXP: type
	//TODO: move the picID to private
	int picID;
	FontMan();
	~FontMan();
	bool loadFont(const char *fontname);
	void genAsciiTable(int h = 16);
	void genStringTable(const char *str, int h = 16);
	//TODO: implement drawString
	void drawString(const char *str, int size);
};
};

#endif
