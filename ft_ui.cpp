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
NineSprite::NineSprite()
{
}


NineSprite::NineSprite(const char *picName)
{
	int picID = ftRender::getPicture(picName);
	init(picID);
}

NineSprite::NineSprite(int picID)
{
	init(picID);
}

void NineSprite::init(int picID)
{
	image = ftRender::getImage(picID);
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
	ftVec2 centerSize = getRectSize() - (gridSize * 2);

	float cx = centerSize.x + gridSize.x;
	float cy = centerSize.y + gridSize.y;

	ftVec2 cornerPos[4];
	cornerPos[0] = ftVec2(-cx, -cy) * 0.5f;
	cornerPos[1] = ftVec2(cx, -cy) * 0.5f;
	cornerPos[2] = ftVec2(cx, cy) * 0.5f;
	cornerPos[3] = ftVec2(-cx, cy) * 0.5f;
	ftVec2 borderPos[4];
	borderPos[0] = ftVec2(0, -cy) * 0.5f;
	borderPos[1] = ftVec2(cx, 0) * 0.5f;
	borderPos[2] = ftVec2(0, cy) * 0.5f;
	borderPos[3] = ftVec2(-cx, 0) * 0.5f;
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

//class ftUI::Label
Label::Label()
{
	text = "";
}
