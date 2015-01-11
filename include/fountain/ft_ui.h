#ifndef _FT_UI_H_
#define _FT_UI_H_

#include <fountain/ft_render.h>
#include <fountain/ft_data.h>
#include <fountain/ft_input.h>
#include <string>

namespace ftUI {

bool init();
void close();

class Button : public ftSprite {
private:
	int state;
	ftColor backColor;
	ftColor foreColor;
public:
	Button();
	void click();
	void update();
	void draw();
	int getState();
	void setBackColor(ftColor c);
};

class NineSprite : public ftSprite {
private:
	ftRender::SubImage image;
public:
	NineSprite(const char *picName);
	NineSprite(int picID);
	NineSprite(ftRender::SubImage image);
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
