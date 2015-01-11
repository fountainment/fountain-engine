#ifndef _FT_UI_H_
#define _FT_UI_H_

#include <fountain/ft_render.h>
#include <fountain/ft_data.h>
#include <string>

namespace ftUI {

bool init();
void close();

class Button : public ftSprite {
private:
	ftColor backColor;
	ftColor foreColor;
public:
	Button();
	void click();
	void update();
	void draw();
};

class NineSprite : public ftSprite {
private:
	ftRender::SubImage image;
public:
	NineSprite(const char *picName);
	NineSprite(int picID);
	void draw();
};

class Label : public ftSprite {
private:
	std::string text;
public:
	Label();
};

};

#endif
