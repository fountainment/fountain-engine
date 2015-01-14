#include <fountain/ft_ui.h>
#include <fountain/ft_type.h>
#include <fountain/ft_input.h>

#include <cstdio>

using ftUI::Button;
using ftUI::NineSprite;
using ftUI::Label;

static ftType::FontMan *defaultFont = NULL;

bool ftUI::init()
{
	return true;
}

void ftUI::close()
{
}

void ftUI::setDefaultFont(ftType::FontMan *font)
{
	defaultFont = font;
}

//class ftUI::Label
Label::Label()
{
	font = defaultFont;
	align = FT_AlignLeft;
	strLength = 0;
}

Label::Label(const char *str)
{
	font = defaultFont;
	setString(str);
	align = FT_AlignLeft;
	strLength = 0;
}

Label::Label(std::string str)
{
	font = defaultFont;
	setString(str.c_str());
	align = FT_AlignLeft;
	strLength = 0;
}

void Label::setString(const char *str)
{
	text = ftAlgorithm::utf8toUnicode(str);
}

void Label::setFont(ftType::FontMan *font)
{
	this->font = font;
}

void Label::setAlign(int align)
{
	this->align = align;
}

void Label::draw()
{
	ftRender::transformBegin();
	ftRender::ftTranslate(getPosition());
	if (font != NULL) {
		strLength = font->drawString(text);
	} else {
		font = defaultFont;
	}
	ftRender::transformEnd();
}

int Label::getStrLength()
{
	return strLength;
}

int Label::getFontSize()
{
	if (font != NULL) return font->getFontSize();
	return 0;
}

//clase ftUI::Button
Button::Button()
{
	state = FT_None;
	backColor = FT_White;
	setColor(FT_Black);
	label.setString("Button");
	label.setRect(getRect());
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
	label.setRect(getRect());
	label.move(ftVec2(label.getStrLength() * -0.5f, label.getFontSize() * -0.40f));
}

void Button::draw()
{
	ftRect rct = getRect();
	ftRender::useColor(backColor);
	ftRender::drawRect(rct);
	ftRender::useColor(getColor());
	label.draw();
}

int Button::getState()
{
	return state;
}

void Button::setBackColor(ftColor c)
{
	backColor = c;
}

void Button::setForeColor(ftColor c)
{
	setColor(c);
}

void Button::setCaption(const char *str)
{
	label.setString(str);
}

//class ftUI::NineSprite
NineSprite::NineSprite()
{
}


NineSprite::NineSprite(const char *picName)
{
	int picID = ftRender::getPicture(picName);
	image = ftRender::getImage(picID);
	init();
}

NineSprite::NineSprite(int picID)
{
	image = ftRender::getImage(picID);
	init();
}

void NineSprite::init()
{
	ftVec2 imageSize = image.getSize();
	gridSize = imageSize / 3.0f;
	setRectSize(imageSize);
	setPosition(0, 0);

	ftRect cornerRect[4];
	cornerRect[0] = ftRect(ftVec2(0, 0), gridSize);
	cornerRect[1] = ftRect(ftVec2(gridSize.x * 2, 0), gridSize);
	cornerRect[2] = ftRect(gridSize * 2, gridSize);
	cornerRect[3] = ftRect(ftVec2(0, gridSize.y * 2), gridSize);
	for (int i = 0; i < 4; i++)
		cornerImage[i] = ftRender::SubImage(image, cornerRect[i]);

	ftRect borderRect[4];
	borderRect[0] = ftRect(ftVec2(gridSize.x, 0), gridSize);
	borderRect[1] = ftRect(ftVec2(gridSize.x * 2, gridSize.y), gridSize);
	borderRect[2] = ftRect(ftVec2(gridSize.x, gridSize.y * 2), gridSize);
	borderRect[3] = ftRect(ftVec2(0, gridSize.y), gridSize);
	for (int i = 0; i < 4; i++)
		borderImage[i] = ftRender::SubImage(image, borderRect[i]);

	ftRect centerRect(gridSize, gridSize);
	centerImage = ftRender::SubImage(image, centerRect);
}

void NineSprite::setSize(ftVec2 size)
{
	setRectSize(size);
	ftVec2 centerSize = getRectSize() - (gridSize * 2);
	centerImage.setSize(centerSize);
	borderImage[0].setSize(ftVec2(centerSize.x, gridSize.y));
	borderImage[1].setSize(ftVec2(gridSize.x, centerSize.y));
	borderImage[2].setSize(ftVec2(centerSize.x, gridSize.y));
	borderImage[3].setSize(ftVec2(gridSize.x, centerSize.y));
}

void NineSprite::draw()
{
	ftVec2 centerSize = getRectSize() - (gridSize * 2.0f);

	float cx = (centerSize.x + gridSize.x) * 0.5f;
	float cy = (centerSize.y + gridSize.y) * 0.5f;

	ftVec2 cornerPos[4] = {ftVec2(-cx, -cy), ftVec2(cx, -cy),
	                       ftVec2(cx, cy), ftVec2(-cx, cy)
	                      };
	ftVec2 borderPos[4] = {ftVec2(.0f, -cy), ftVec2(cx, .0f),
	                       ftVec2(.0f, cy), ftVec2(-cx, .0f)
	                      };

	ftRender::transformBegin();
	ftRender::ftTranslate(getPosition());
	for (int i = 0; i < 4; i++) {
		ftRender::transformBegin();
		ftRender::ftTranslate(cornerPos[i]);
		ftRender::drawImage(cornerImage[i]);
		ftRender::transformEnd();
	}
	for (int i = 0; i < 4; i++) {
		ftRender::transformBegin();
		ftRender::ftTranslate(borderPos[i]);
		ftRender::drawImage(borderImage[i]);
		ftRender::transformEnd();
	}
	ftRender::drawImage(centerImage);
	ftRender::transformEnd();
}
