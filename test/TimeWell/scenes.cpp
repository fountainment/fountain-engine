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

	ftRender::transformBegin(pos.x, pos.y, this->getAngle(), 1.01f, FT_Black);
	ftRender::drawShape(shape);
	ftRender::drawShapeEdge(shape);
	ftRender::transformEnd();

	ftRender::transformBegin(pos.x, pos.y, this->getAngle(), 1.0f - (5.0f / (this->r * std::cos(3.14159f / this->en))), this->getColor());
	ftRender::drawShape(shape);
	ftRender::drawShapeEdge(shape);
	ftRender::transformEnd();
	ftRender::useColor(FT_White);
}

void OC::update()
{
	b2Vec2 bv = body->GetPosition();
	float angle = body->GetAngle();
	this->setPosition(ftVec2(bv.x, bv.y) * ftPhysics::getRatio());
	this->setAngle(angle);
}

void GameScene::otherInit()
{
	ftRender::setClearColor(ftColor("#E30039"));
	mc.image = ftRender::getImage("res/image/me.png"); 

	world = new b2World(b2Vec2(0, 0));

	OC tmp;
	for (int i = 0; i < 500; i++) {
		float r = ftAlgorithm::randRangef(20, 40);
		int en = ftAlgorithm::randRangef(3, 6.99);
		float x = ftAlgorithm::randRangef(-1500, 1500);
		float y = ftAlgorithm::randRangef(-750, 750);
		tmp.r = r;
		tmp.en = en;
		tmp.setPosition(x, y);
		tmp.shape = ftShape::makeRegularPolygonShape(en, r);
		b2Shape *b2shape = ftPhysics::createb2ShapeWithFtShape(tmp.shape);
		tmp.body = ftPhysics::createBodyInWorld(world, x, y, FT_Dynamic);
		tmp.body->CreateFixture(b2shape, 1.0f);
		delete b2shape;
		tmp.setColor(OC::randColor());
		tmp.speed = ftVec2(ftAlgorithm::randRangef(-5.0f, 5.0f), ftAlgorithm::randRangef(-5.0f, 5.0f));
		tmp.body->SetLinearVelocity(b2Vec2(tmp.speed.x, tmp.speed.y));
		tmp.aSpeed = ftAlgorithm::randRangef(-1.0f, 1.0f);
		ocPool.add(tmp);
	}
}

void GameScene::otherUpdate()
{
	world->Step(mainClock.getDeltaT(), 8, 3);

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

