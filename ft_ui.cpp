#include <fountain/fountaindef.h>

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

void ftUI::setDefaultFont(ftType::FontMan *font)
{
	defaultFontMan = font;
}

//class ftUI::Label
Label::Label()
{
	font = NULL;
	align = FT_AlignLeft;
	strLength = 0;
}

Label::Label(const char *str)
{
	font = NULL;
	setString(str);
	align = FT_AlignLeft;
	strLength = 0;
}

Label::Label(std::string str)
{
	font = NULL;
	setString(str.c_str());
	align = FT_AlignLeft;
	strLength = 0;
}

void Label::setString(const char *str)
{
	text = ftAlgorithm::utf8toUnicode(str);
	if (font != NULL) {
		strLength = font->getStringLength(text);
	} else {
		strLength = defaultFontMan->getStringLength(text);
	}
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
	ftRender::useColor(getColor());
	ftRender::transformBegin();
	ftRender::ftTranslate(getPosition());
	if (font != NULL) {
		strLength = font->drawString(text);
	} else {
		strLength = defaultFontMan->drawString(text);
	}
	ftRender::transformEnd();
}

int Label::getStrLength()
{
	return strLength;
}

int Label::getFontSize()
{
	if (font != NULL) {
		return font->getFontSize();
	} else {
		return defaultFontMan->getFontSize();
	}
	return 0;
}

//clase ftUI::Button
Button::Button()
{
	state = FT_None;
	backColor = FT_White;
	haveDown = false;
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
		if ((state == FT_isUp) || (state == FT_isDown && !haveDown)) {
			state = FT_isOn;
		} else if (state == FT_ButtonDown) {
			haveDown = true;
		} else if (state == FT_ButtonUp) {
			if (haveDown) {
				click();
				haveDown = false;
			} else {
				state = FT_isOn;
			}
		}
	} else {
		if (haveDown && mState == FT_ButtonUp) haveDown = false;
		state = FT_None;
	}
}

void Button::draw()
{
	ftRect rct = getRect();
	ftRender::useColor(backColor);
	ftRender::drawRect(rct);
	label.setColor(getColor());
	label.draw();
}

int Button::getState()
{
	return state;
}

void Button::setBackColor(const ftColor c)
{
	backColor = c;
}

void Button::setForeColor(const ftColor c)
{
	setColor(c);
}

void Button::setCaption(const char *str)
{
	label.setString(str);
	label.setPosition(getPosition() + ftVec2(label.getStrLength() * -0.5f, label.getFontSize() * -0.37f));
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

//TODO: complete this function(NineSprite(ftRender::SubImage image))
NineSprite::NineSprite(ftRender::SubImage image)
{
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

void NineSprite::setSize(const ftVec2 & size)
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

	ftRender::useColor(getColor());
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
