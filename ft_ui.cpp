#include <fountain/ft_ui.h>
#include <fountain/ft_type.h>
#include <fountain/ft_input.h>

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
	state = FT_None;
	backColor = FT_White;
	foreColor = FT_Black;
}

void Button::click()
{
}

void Button::update()
{
	ftVec2 mPos = fountain::sysMouse.getPos();
	ftRender::Camera *cam = ftRender::getCurrentCamera();
	if (cam != NULL) mPos = cam->mouseToWorld(mPos);
	int mState = fountain::sysMouse.getState(FT_LButton);
	ftRect rct = getRect();
	if (rct.collidePoint(mPos)) {
		state = mState;
		if (state == FT_isUp) state = FT_isOn;
	} else {
		state = FT_None;
	}
}

void Button::draw()
{
	ftRect rct = getRect();
	ftRender::useColor(backColor);
	ftRender::drawRect(rct);
}

int Button::getState()
{
	return state;
}

void Button::setBackColor(ftColor c)
{
	backColor = c;
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
