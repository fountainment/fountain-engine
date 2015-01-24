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
	float a[3];
	a[0] = ftAlgorithm::randRangef(0.8f, 0.85f);
	a[1] = ftAlgorithm::randRangef(0.1f, 0.9f);
	a[2] = ftAlgorithm::randRangef(0.0f, 0.05f);
	int rn = (int)ftAlgorithm::randRangef(0.0f, 2.99f);
	float r = a[rn];

	int gn = (int)ftAlgorithm::randRangef(0.0f, 2.99f);
	while (gn == rn)
		gn = (int)ftAlgorithm::randRangef(0.0f, 2.99f);
	float g = a[gn];

	int bn = (int)ftAlgorithm::randRangef(0.0f, 2.99f);
	while (bn == rn || bn == gn)
		bn = (int)ftAlgorithm::randRangef(0.0f, 2.99f);
	float b = a[bn];
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

	OC tmp;
	for (int i = 0; i < 100; i++) {
		tmp.shape = ftShape::makeRegularPolygonShape(ftAlgorithm::randRangef(3, 6.99),ftAlgorithm::randRangef(20, 40));
		tmp.setColor(OC::randColor());
		tmp.speed = ftVec2(ftAlgorithm::randRangef(-5.0f, 5.0f), ftAlgorithm::randRangef(-5.0f, 5.0f));
		ocPool.add(tmp);
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

