#include "scenes.h"

#include <cstdio>

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
	ftRender::ftTranslate(mainCamera.mouseToWorld(fountain::sysMouse.getPos()));
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

void GameScene::otherInit()
{
	ftPhysics::setRatio(100.0f);
	myWorld = ftPhysics::World(ftVec2(0, 0));
	ftRender::setClearColor(ftColor("#CC3333"));
	mainC = MC(ftRender::getImage("res/image/me.png"));
	mainC.setShape(ftShape(100.0f));
}

void GameScene::otherUpdate()
{
}

void GameScene::otherDraw()
{
	mainCamera.update();
	mainC.draw();
}

