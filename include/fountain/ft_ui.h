#ifndef _FT_UI_H_
#define _FT_UI_H_

#include <fountain/ft_render.h>
#include <fountain/ft_data.h>
#include <fountain/ft_input.h>
#include <fountain/ft_algorithm.h>
#include <fountain/ft_type.h>
#include <string>
#include <vector>

#define FT_AlignLeft 0
#define FT_AlignCenter 1

namespace ftUI {

bool init();
void close();

void setDefaultFont(ftType::FontMan *font);

class Label : public ftSprite
{
private:
	std::vector<unsigned long> text;
	ftType::FontMan *font;
	float fontSize;
	int align;
	int strLength;
public:
	Label();
	Label(const char *str);
	Label(std::string str);
	void setString(const char *str);
	void setFont(ftType::FontMan *font);
	void setAlign(int align);
	void draw();
	int getStrLength();
	int getFontSize();
};

class Button : public ftSprite
{
private:
	int state;
	ftColor backColor;
	bool haveDown;
public:
	Label label;
	Button();
	virtual void click();
	void update();
	void draw();
	int getState();
	void setBackColor(const ftColor c);
	void setForeColor(const ftColor c);
	void setCaption(const char *str);
};

class NineSprite : public ftSprite
{
private:
	ftRender::SubImage image;
	ftRender::SubImage cornerImage[4];
	ftRender::SubImage borderImage[4];
	ftRender::SubImage centerImage;
	ftVec2 gridSize;
public:
	void init();
	NineSprite();
	NineSprite(const char *picName);
	NineSprite(int picID);
	NineSprite(ftRender::SubImage image);
	void setSize(const ftVec2 & size);
	void draw();
};

//TODO: complete this class(Cursor)
class Cursor : public ftSprite
{
private:
public:
};

}

#endif
