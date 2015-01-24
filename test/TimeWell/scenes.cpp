#include "scenes.h"

#include <cstdio>
#include <cmath>

b2Body *ccb;
bool ccbb = false;

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
	ftRender::useColor(FT_White);
}

void OC::update()
{
	b2Vec2 bv = body->GetPosition();
	float angle = body->GetAngle();
	this->setPosition(ftVec2(bv.x, bv.y) * ftPhysics::getRatio());
	this->setAngle(angle);
	if (this->getTag() == 3) {
		ccb = body;
		ccbb = true;
		this->setTag(1);
	}
}

void CL::Presolve(b2Contact *contact, const b2Manifold* oldManifold)
//void CL::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	const b2Manifold* manifold = contact->GetManifold();



	 if (manifold->pointCount == 0){

		   return;

		    }
}
void CL::BeginContact(b2Contact *contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	b2Body *ba = fixtureA->GetBody();
	b2Body *bb = fixtureB->GetBody();
	//b2World *world = ba->GetWorld();

	void* userDataA = ba->GetUserData();
	void* userDataB = bb->GetUserData();
	
	int atag, btag; 

	if (userDataA)
	{
		ftSprite *A = (ftSprite*)userDataA;
		atag = A->getTag();
		if (atag == 1) {
			//b2Vec2 t = ba->GetPosition() - bb->GetPosition();
			//bb->ApplyForceToCenter(-10000.0f * t, true);
			//b2WeldJointDef jd;
			//b2DistanceJointDef jd;
			//jd.frequencyHz = 5.0f;
			//jd.dampingRatio = 0.7f;
			//jd.Initialize(bb, ba, ba->GetPosition());
			//jd.Initialize(ba, bb, ba->GetPosition(), bb->GetPosition());
			//world->CreateJoint(&jd);
			ftSprite *B = (ftSprite*)userDataB;
			B->setTag(3);
		}
	}

	if (userDataB)
	{
		ftSprite *B = (ftSprite*)userDataB;
		btag = B->getTag();
		if (btag == 1) {
			//b2Vec2 t = bb->GetPosition() - ba->GetPosition();
			//ba->ApplyForceToCenter(-10000.0f * t, true);
			//b2DistanceJointDef jd;
			//jd.frequencyHz = 5.0f;
			//jd.dampingRatio = 0.7f;
			//jd.Initialize(ba, bb, bb->GetPosition());
			//jd.Initialize(ba, bb, ba->GetPosition(), bb->GetPosition());
			//world->CreateJoint(&jd);
			ftSprite *A = (ftSprite*)userDataB;
			A->setTag(3);
		}
	}
}

void ocSetUserData(OC & oc)
{
	oc.body->SetUserData((ftSprite*)&oc);
}

void GameScene::otherInit()
{
	ftRender::setClearColor(ftColor("#E30039"));

	ftPhysics::setRatio(64.0f);
	world = new b2World(b2Vec2(0, 0));
	world->SetContactListener(&cListener);

	mc.image = ftRender::getImage("res/image/me.png"); 
	mc.body = ftPhysics::createBodyInWorld(world, 0, 0, FT_Dynamic);
	ftShape seven = ftShape::makeRegularPolygonShape(7, 50);
	b2Shape *b2shape = ftPhysics::createb2ShapeWithFtShape(seven);
	mc.body->CreateFixture(b2shape, 1.0f);
	mc.body->SetUserData((ftSprite*)&mc);
	mc.setTag(1);
	b2Fixture *fx = mc.body->GetFixtureList();
	fx->SetSensor(true);	
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
		ocPool.add(tmp);
	}
	ocPool.doWith(ocSetUserData);
}

void GameScene::otherUpdate()
{
	world->Step(mainClock.getDeltaT(), 8, 3);

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

	ocPool.update();

	if (ccbb) {
		b2DistanceJointDef jd;
		jd.Initialize(mc.body, ccb, mc.body->GetPosition(), ccb->GetPosition());
		world->CreateJoint(&jd);
		ccbb = false;
	}
}

void GameScene::otherDraw()
{
	mainCamera.update();
	mc.draw();
	ocPool.draw();
}

