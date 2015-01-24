#include "scenes.h"

#include <cstdio>
#include <cmath>
#include <stack>
#include <vector>

std::stack<b2Body*> bStack;

typedef struct {
	b2Body *master;
	b2Body *slave;
	b2Vec2 rp;
	float rd;
	float md;
} RP;

std::vector<RP> rpVector;

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

void MC::update()
{
	b2Vec2 bv = body->GetPosition();
	float angle = body->GetAngle();
	this->setPosition(ftVec2(bv.x, bv.y) * ftPhysics::getRatio());
	this->setAngle(angle);
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

	if (this->getTag() == 3) {
		ftRender::transformBegin(pos.x, pos.y, this->getAngle(), 1.0f - (5.0f / (this->r * std::cos(3.14159f / this->en))) - 0.5, FT_Black);
		ftRender::drawShape(shape);
		ftRender::drawShapeEdge(shape);
		ftRender::transformEnd();
	}

	ftRender::useColor(FT_White);
}

void OC::update()
{
	b2Vec2 bv = body->GetPosition();
	float angle = body->GetAngle();
	this->setPosition(ftVec2(bv.x, bv.y) * ftPhysics::getRatio());
	this->setAngle(angle);
}
/*
void CL::Presolve(b2Contact *contact, const b2Manifold* oldManifold)
//void CL::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	const b2Manifold* manifold = contact->GetManifold();
	if (manifold->pointCount == 0){
		return;
	}
}
*/
//void CL::PreSolve(b2Contact *contact, const b2Manifold* oldManifold)
void CL::BeginContact(b2Contact *contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	b2Body *ba = fixtureA->GetBody();
	b2Body *bb = fixtureB->GetBody();

	void* userDataA = ba->GetUserData();
	void* userDataB = bb->GetUserData();
	
	int atag, btag; 

	ftSprite *A = (ftSprite*)userDataA;
	atag = A->getTag();
	ftSprite *B = (ftSprite*)userDataB;
	btag = B->getTag();
	
	if ((atag == 1) ^ (btag == 1)) {
		if (atag == 1) {
			bStack.push(bb);
			B->setTag(3);
		} else {
			bStack.push(ba);
			A->setTag(3);
		}
		//A->setTag(1);
		//B->setTag(1);
	}
}

void ocSetUserData(OC & oc)
{
	oc.body->SetUserData((ftSprite*)&oc);
}

void GameScene::otherInit()
{
	ftRender::setClearColor(ftColor("#E30039"));

	rpVector.clear();

	ftPhysics::setRatio(64.0f);
	world = new b2World(b2Vec2(0, 0));
	world->SetContactListener(&cListener);
	world->SetAllowSleeping(false);

	mc.image = ftRender::getImage("res/image/me.png"); 
	mc.body = ftPhysics::createBodyInWorld(world, 0, 0, FT_Kinematic);
	ftShape seven = ftShape::makeRegularPolygonShape(7, 50);
	b2Shape *b2shape = ftPhysics::createb2ShapeWithFtShape(seven);
	mc.body->CreateFixture(b2shape, 1.0f);
	mc.body->SetUserData((ftSprite*)&mc);
	mc.setTag(1);
	//b2Fixture *fx = mc.body->GetFixtureList();
	//fx->SetSensor(true);
	delete b2shape;

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
		tmp.setTag(0);
		ocPool.add(tmp);
	}
	ocPool.doWith(ocSetUserData);
}

void GameScene::otherUpdate()
{
	world->Step(mainClock.getDeltaT(), 20, 20);

	ftVec2 mcPos = mc.getPosition();
	ftVec2 target = mainCamera.mouseToWorld(fountain::sysMouse.getPos());
	ftVec2 deltaV = target - mcPos;
	ftVec2 tv = deltaV * (mainClock.getDeltaT() * 3.0f);
	
	float d = std::atan(deltaV.y / deltaV.x);
	if (deltaV.x > 0) d -= 3.14159f / 2.0f;
	else d += 3.14159f / 2.0f;

	mc.body->SetTransform(mc.body->GetPosition(), d);
	mc.body->SetLinearVelocity(b2Vec2(tv.x, tv.y));

	mc.update();

	ftVec2 camPos = mainCamera.getPosition();
	deltaV = mcPos - camPos;
	mainCamera.move(deltaV * (mainClock.getDeltaT() * 2.0f));

	while (bStack.size()) {
		b2Body *ba = bStack.top();

		//ba->SetType(b2_kinematicBody);

		RP tmp;
		tmp.master = mc.body;
		tmp.slave = ba;
		tmp.rp = ba->GetPosition() - mc.body->GetPosition();
		tmp.rd = ba->GetAngle() - mc.body->GetAngle();
		tmp.md = mc.body->GetAngle();
		rpVector.push_back(tmp);

		bStack.pop();
		/* dangerous
		b2RevoluteJointDef jd;
		jd.collideConnected = true;
		jd.enableLimit = true;
		jd.Initialize(ba, bb, ba->GetPosition());
		world->CreateJoint(&jd);
		*/
	}

	for (unsigned i = 0; i < rpVector.size(); i++) {
		RP tmp = rpVector[i];
		b2Vec2 xyz = tmp.rp;
		float md = tmp.master->GetAngle() - tmp.md;
		xyz.x = tmp.rp.x * std::cos(md) - tmp.rp.y * std::sin(md);
		xyz.y = tmp.rp.x * std::sin(md) + tmp.rp.y * std::cos(md);
		tmp.slave->SetTransform(tmp.master->GetPosition() + xyz, tmp.master->GetAngle() + tmp.rd);
	}

	ocPool.update();
}

void GameScene::otherDraw()
{
	mainCamera.update();
	mc.draw();
	ocPool.draw();
	ftRender::useColor(FT_Black);
	ftRender::setLineWidth(5.0f);
	for (unsigned i = 0; i < rpVector.size() - 1; i++) {
		b2Vec2 a = ftPhysics::getRatio() * rpVector[i].slave->GetPosition(); 
		b2Vec2 b = ftPhysics::getRatio() * rpVector[i + 1].slave->GetPosition(); 
		ftRender::drawLine(a.x, a.y, b.x, b.y);
	}
	ftRender::setLineWidth(1.0f);
	ftRender::useColor(FT_White);
	ftVec2 target = mainCamera.mouseToWorld(fountain::sysMouse.getPos());
	ftVec2 line = target - mc.getPosition();
	line = line * 0.125f;
	for (int i = 1; i < 8; i++) {
		ftRender::transformBegin();
		ftRender::ftTranslate(mc.getPosition() + (line * i));
		ftRender::drawCircle(5);
		ftRender::transformEnd();
	}
}

