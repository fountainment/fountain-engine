#include "scenes.h"

#include <cstdio>
#include <cmath>
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

ftColor OC::randColor()
{
	float r = ftAlgorithm::randRangef(0.6f, 1.0f);
	float g = ftAlgorithm::randRangef(0.45f, 1.0f);
	float b = ftAlgorithm::randRangef(0.0f, 0.2f);
	return ftColor(r, g, b);
}

void OC::draw()
{
	ftVec2 pos = this->getPosition();
	ftRender::transformBegin(pos.x, pos.y, this->getAngle(), 1.0f, this->getColor());
	ftRender::drawShape(shape);
	ftRender::transformEnd();
	ftRender::useColor(FT_White);
}

void OC::update()
{
	this->move(speed);
}

void GameScene::otherInit()
{
	ftRender::setClearColor(ftColor("#E30039"));
	mc.image = ftRender::getImage("res/image/me.png"); 

	OC x;
	x.shape = ftShape(10.0f);
	for (int i = 0; i < 100; i++) {
	x.setColor(OC::randColor());
	x.speed = ftVec2(ftAlgorithm::randRangef(-5.0f, 5.0f), ftAlgorithm::randRangef(-5.0f, 5.0f));
	ocPool.add(x);
	}
}

void GameScene::otherUpdate()
{
	ftVec2 mcPos = mc.getPosition();
	ftVec2 target = mainCamera.mouseToWorld(fountain::sysMouse.getPos());
	ftVec2 deltaV = target - mcPos;
	mc.move(deltaV * (mainClock.getDeltaT() * 3.0f));
	
	float d = std::atan(deltaV.y / deltaV.x);
	if (deltaV.x > 0) d -= 3.14159f / 2.0f;
	else d += 3.14159f / 2.0f;
	mc.setAngle(d);

	ftVec2 camPos = mainCamera.getPosition();
	deltaV = mcPos - camPos;
	mainCamera.move(deltaV * (mainClock.getDeltaT() * 2.0f));

	ocPool.update();
}

void GameScene::otherDraw()
{
	mainCamera.update();
	mc.draw();
	ocPool.draw();
}

