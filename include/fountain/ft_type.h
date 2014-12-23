#ifndef _FT_TYPE_H_
#define _FT_TYPE_H_

#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>

namespace ftType {

	bool init();

	class FontMan {
	private:
		int h;
		FT_Face face;
		std::map<char, unsigned> char2Tex;

	public:
		FontMan();
		~FontMan();
		bool loadFont(const char *fontname);
		void genAsciiTable();
		void drawString(const char *str);
	};
};

#endif
