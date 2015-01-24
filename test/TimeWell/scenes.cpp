#include "scenes.h"

#include <cstdio>
/*
MC::MC()
{
}

MC::MC(ftRender::SubImage image)
{
	im = image;
	this->body = NULL;
	this->bodyType = FT_Kinematic;
	setPosition(0, 0);
	setRectSize(ftVec2(1.0f, 1.0f));
	scale = 1.0f;
}

void MC::draw()
{
	ftVec2 pos = this->getPosition();
	drawImage(im, pos.x, pos.y, this->getAngle(), scale, this->getColor());
}
*/
void BaseScene::init()
{
	screenC.setViewport(fountain::getWinRect());
	cursorID = ftRender::getPicture("res/image/cursor.png");
	otherInit();
}

void BaseScene::otherInit()
{
}

void BaseScene::update()
{
	otherUpdate();
}

void BaseScene::otherUpdate()
{
}

void BaseScene::draw()
{
	drawBG();
	otherDraw();
	drawCursor();
}

void BaseScene::drawBG()
{
	screenC.update();
}

void BaseScene::otherDraw()
{
}

void BaseScene::drawCursor()
{
	screenC.update();
	ftRender::transformBegin();
	ftRender::ftTranslate(screenC.mouseToWorld(fountain::sysMouse.getPos()));
	ftRender::drawAlphaPic(cursorID);
	ftRender::transformEnd();
}

void OpenScene::otherInit()
{
}

void OpenScene::otherUpdate()
{
	if (fountain::sysMouse.getState(FT_LButton) == FT_ButtonDown)
		fountain::sceneSelector.gotoScene(new GameScene());
}

void OpenScene::otherDraw()
{
	mainCamera.update();
}

void MC::draw()
{
	ftVec2 pos = this->getPosition();
	drawImage(image, pos.x, pos.y, this->getAngle(),
			1.0f, this->getColor());
}

void GameScene::otherInit()
{
	ftRender::setClearColor(ftColor("#E30039"));
	mc.image = ftRender::getImage("res/image/me.png"); 
}

void GameScene::otherUpdate()
{
	ftVec2 mcPos = mc.getPosition();
	ftVec2 target = mainCamera.mouseToWorld(fountain::sysMouse.getPos());
	ftVec2 deltaV = target - mcPos;
	mc.move(deltaV * (mainClock.getDeltaT() * 3.0f));

	ftVec2 camPos = mainCamera.getPosition();
	deltaV = mcPos - camPos;
	mainCamera.move(deltaV * (mainClock.getDeltaT() * 2.0f));
}

void GameScene::otherDraw()
{
	mainCamera.update();
	mc.draw();
	ftRender::drawLine(-100, 0, 100, 0);
}

