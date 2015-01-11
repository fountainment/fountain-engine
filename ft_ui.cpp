#include <fountain/ft_ui.h>
#include <fountain/ft_type.h>

using ftUI::Button;
using ftUI::NineSprite;
using ftUI::Label;

bool ftUI::init()
{
	return true;
}

void ftUI::close()
{
}

//clase ftUI::Button
Button::Button()
{
	backColor = FT_White;
	foreColor = FT_Black;
}

void Button::click()
{
}

void Button::update()
{
}

void Button::draw()
{
	ftRect rct = getRect();
	ftRender::drawRect(rct);
}

//class ftUI::NineSprite
NineSprite::NineSprite(const char *picName)
{
}

NineSprite::NineSprite(int picID)
{
}

void NineSprite::draw()
{
}

//class ftUI::Label
Label::Label()
{
	text = "";
}
