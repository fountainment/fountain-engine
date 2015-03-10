#include "scenes.h"

std::stack<b2Body*> bStack;
container<OC, 1000> drawCon;
bool over;

ftType::FontMan fm;
ftType::FontMan lf;

void BaseScene::init()
{
	screenC.setViewport(fountain::getWinRect());
	mainCamera.setViewport(fountain::getWinRect());
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
	ftRender::useColor(FT_White);
	ftRender::transformBegin();
	ftRender::ftTranslate(screenC.mouseToWorld(fountain::sysMouse.getPos()));
	ftRender::drawAlphaPic(cursorID);
	ftRender::transformEnd();
}

void OSButton::update()
{
	Button::update();
	if (getState() == FT_None) {
		setBackColor(FT_White);
	} else {
		setBackColor(bcolor);
	}
}

void OpenScene::otherInit()
{
	ftRender::setClearColor(OC::randColor());

	startB.bcolor = OC::randColor();
	startB.setRectSize(ftVec2(500, 200));
	startB.setPosition(0, -200);
	startB.setCaption("Start");

	title.setPosition(0, 200);
	title.setCaption("TimeWell");

	intro.setPosition(0, 40);
	intro.label.setFont(&lf);
	intro.setCaption("Color blocks raise your score, black blocks reduce your score!");
	intro.label.setFont(&lf);
	intro.setColor(FT_White);

	intro1.setPosition(0, -40);
	intro1.label.setFont(&lf);
	intro1.setCaption("Your target is the blackhole!");
	intro1.label.setFont(&lf);
	intro1.setColor(FT_White);
}

void OpenScene::otherUpdate()
{
	startB.update();
	title.update();
	if (startB.getState() == FT_ButtonUp) {
		fountain::sceneSelector.gotoScene(new GameScene());
	}
}

void OpenScene::otherDraw()
{
	mainCamera.update();
	title.draw();
	intro.draw();
	intro1.draw();
	startB.draw();
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

void MC::drawAt(float x, float y)
{
	drawImage(image, x, y, this->getAngle(),
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

	ftRender::transformBegin(pos.x, pos.y, this->getAngle(), 1.01f, bcolor);
	ftRender::drawShape(shape);
	ftRender::drawShapeEdge(shape);
	ftRender::transformEnd();

	ftRender::transformBegin(pos.x, pos.y, this->getAngle(), 1.0f - (5.0f / (this->r * std::cos(FT_Pi / this->en))), this->getColor());
	ftRender::drawShape(shape);
	ftRender::drawShapeEdge(shape);
	ftRender::transformEnd();

	if (this->getTag() > 0) {
		ftRender::transformBegin(pos.x, pos.y, this->getAngle(), 1.0f - (5.0f / (this->r * std::cos(FT_Pi / this->en))) - 0.5, FT_Black);
		ftRender::drawShape(shape);
		ftRender::drawShapeEdge(shape);
		ftRender::transformEnd();
	}

	ftRender::useColor(FT_White);
}

void OC::update()
{
	if (this->getTag() > 0) {
		b2Vec2 xyz = tmp.rp;
		float md = tmp.master->GetAngle() - tmp.md;
		xyz.x = tmp.rp.x * std::cos(md) - tmp.rp.y * std::sin(md);
		xyz.y = tmp.rp.x * std::sin(md) + tmp.rp.y * std::cos(md);
		body->SetTransform(tmp.master->GetPosition() + xyz, tmp.master->GetAngle() + tmp.rd);
	}
	b2Vec2 bv = body->GetPosition();
	float angle = body->GetAngle();
	this->setPosition(ftVec2(bv.x, bv.y) * ftPhysics::getRatio());
	this->setAngle(angle);
	if (this->getTag() > 0) {
		fountain::sceneSelector.getCurScene()->mainCamera.update();
		b2Vec2 a = tmp.master->GetPosition();
		a = ftPhysics::getRatio() * a;
		b2Vec2 b = body->GetPosition();
		b = ftPhysics::getRatio() * b;
		ftRender::useColor(this->bcolor);
		ftRender::setLineWidth(5.0f);
		ftRender::drawLine(a.x, a.y, b.x, b.y);
		ftRender::setLineWidth(1.0f);
		ftRender::useColor(FT_White);
	}
}

BH BH::create()
{
	BH x;
	x.enable = false;
	x.en = (int)ftAlgorithm::randRangef(3, 6.99);
	x.shape = ftShape(50.0f);
	switch (x.en) {
	case 3:
		x.image = ftRender::getImage("res/image/blackhole3.png");
		break;
	case 4:
		x.image = ftRender::getImage("res/image/blackhole4.png");
		break;
	case 5:
		x.image = ftRender::getImage("res/image/blackhole5.png");
		break;
	case 6:
		x.image = ftRender::getImage("res/image/blackhole6.png");
		break;
	}
	x.setColor(FT_White);
	x.setRectSize(200, 200);
	return x;
}

void BH::update()
{
	if (enable) {
		rotate(1.0f);
	}
}

void BH::draw()
{
	if (enable) {
		ftVec2 pos = getPosition();
		ftRender::transformBegin(pos.x, pos.y, getAngle(), 1.0f, FT_White);
		ftRender::drawImage(image);
		ftRender::transformEnd();
		ftRender::useColor(FT_White);
	}
}

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

	if (((atag == 1) && (btag == 2)) ||
			((atag == 2) && (btag == 1))) {
		if (atag == 2 && A->enable) over = true;
		if (btag == 2 && B->enable) over = true;
	}
	else if ((atag == 2) || (btag == 2)) {
		if (A->enable && B->enable) {
			if (atag == 0) {
				A->die = true;
			}
			else if (btag == 0) {
				B->die = true;
			}
		}
	}
	else if ((atag == 1) ^ (btag == 1)) {
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

bool ocContainer::willLive(OC & oc)
{
	ftRender::Camera *cam = &((fountain::sceneSelector.getCurScene())->mainCamera);
	if (cam != NULL) {
		ftRect cr = cam->getCameraRect();
		ftVec2 xy = cr.getCenter();
		cr.inflate(2, 2);
		cr.setCenter(xy);
		bool die = !cr.collidePoint(oc.getPosition());
		if (die || oc.die) {
			(oc.body->GetWorld())->DestroyBody(oc.body);
			return false;
		} else return true;
	} else return true;
}

void ocSetUserData(OC & oc)
{
	oc.body->SetUserData((ftSprite*)&oc);
}

void ocSetColor(OC & oc)
{
	if (oc.getTag() != 3) {
		oc.setColor(FT_Black);
		oc.bcolor = FT_White;
	}
}

void ocSetContact(OC & oc)
{
	if (oc.getTag() == 3) {
		oc.setTag(1);
	}
}

void GameScene::otherInit()
{
	state = 1;

	drawCon.clear();

	over = false;

	mainCamera.setPosition(0, 0);

	repl.bcolor = OC::randColor();
	repl.setRectSize(ftVec2(500, 200));
	repl.setPosition(300, 0);
	repl.setCaption("replay");

	scoreB.setPosition(fountain::mainWin.w / 2.0f - 300.0f, fountain::mainWin.h / 2.0f - 128.0f);
	timeB.setPosition(-fountain::mainWin.w / 2.0f + 50.0f, fountain::mainWin.h / 2.0f - 128.0f);

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
	mc.score = 0;
	//b2Fixture *fx = mc.body->GetFixtureList();
	//fx->SetSensor(true);
	delete b2shape;

	OC tmp;
	float xxxx, yyyy;
	for (int ixx = 0; ixx < 150; ixx++) {
		float r = ftAlgorithm::randRangef(20, 40);
		int en = ftAlgorithm::randRangef(3, 6.99);
		xxxx = ftAlgorithm::randRangef(-2000, -1500);
		if (ftAlgorithm::randRangef(0, 1) > 0.5) xxxx *= -1;
		yyyy = ftAlgorithm::randRangef(-1250, -750);
		if (ftAlgorithm::randRangef(0, 1) > 0.5) yyyy *= -1;
		tmp.r = r;
		tmp.en = en;
		tmp.setPosition(xxxx, yyyy);
		tmp.shape = ftShape::makeRegularPolygonShape(en, r);
		b2Shape *b2shape = ftPhysics::createb2ShapeWithFtShape(tmp.shape);
		tmp.body = ftPhysics::createBodyInWorld(world, xxxx, yyyy, FT_Dynamic);
		tmp.body->CreateFixture(b2shape, 1.0f);
		delete b2shape;
		tmp.setColor(OC::randColor());
		tmp.speed = ftVec2(ftAlgorithm::randRangef(-5.0f, 5.0f), ftAlgorithm::randRangef(-5.0f, 5.0f));
		tmp.body->SetLinearVelocity(b2Vec2(tmp.speed.x, tmp.speed.y));
		tmp.aSpeed = ftAlgorithm::randRangef(-1.0f, 1.0f);
		tmp.setTag(0);
		tmp.die = false;
		tmp.bcolor = FT_Black;
		ocPool.add(tmp);
	}
	ocPool.doWith(ocSetUserData);

	bh = BH::create();
	float xx = ftAlgorithm::randRangef(-2500, 2500);
	float yy = ftAlgorithm::randRangef(-2500, 2500);
	bh.setPosition(xx, yy);
	bh.hole = ftPhysics::createBodyInWorld(world, xx, yy, FT_Dynamic);
	b2Shape *b2shape1 = ftPhysics::createb2ShapeWithFtShape(bh.shape);
	bh.hole->CreateFixture(b2shape1, 1.0f);
	delete b2shape1;
	b2Fixture *fx = bh.hole->GetFixtureList();
	fx->SetSensor(true);
	bh.setTag(2);
	bh.hole->SetUserData((ftSprite*)&bh);
}

void GameScene::otherUpdate()
{
	if (state == 1) {
	world->Step(mainClock.getDeltaT(), 20, 20);

	ftVec2 mcPos = mc.getPosition();
	ftVec2 target = mainCamera.mouseToWorld(fountain::sysMouse.getPos());
	ftVec2 deltaV = target - mcPos;
	ftVec2 tv = deltaV * (mainClock.getDeltaT() * 3.0f);

	/*
	//Rotate MC
	if (bh.enable) {
		float d = std::atan(deltaV.y / deltaV.x);
		if (deltaV.x > 0) d -= FT_Pi / 2.0f;
		else d += FT_Pi / 2.0f;
		mc.body->SetTransform(mc.body->GetPosition(), d);
	}
	*/
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
		tmp.rp = ba->GetPosition() - mc.body->GetPosition();
		tmp.rd = ba->GetAngle() - mc.body->GetAngle();
		tmp.md = mc.body->GetAngle();

		OC *oc = (OC*)ba->GetUserData();
		oc->tmp = tmp;
		if (!bh.enable) mc.score++;
		else mc.score--;

		drawCon.add(*oc);
		bStack.pop();
	}

	ocPool.update();
	OC tmp;
	if (ocPool.getAvailN() > 350) {
		float r = ftAlgorithm::randRangef(20, 40);
		int en = ftAlgorithm::randRangef(3, 6.99);
		float x, y;
		ftRect rct = mainCamera.getCameraRect();
		do {
			x = ftAlgorithm::randRangef(mcPos.x - 1500, mcPos.x + 1500);
			y = ftAlgorithm::randRangef(mcPos.y - 750, mcPos.y + 750);
		} while(rct.collidePoint(ftVec2(x, y)));
		tmp.r = r;
		tmp.en = en;
		tmp.setPosition(x, y);
		tmp.shape = ftShape::makeRegularPolygonShape(en, r);
		b2Shape *b2shape = ftPhysics::createb2ShapeWithFtShape(tmp.shape);
		tmp.body = ftPhysics::createBodyInWorld(world, x, y, FT_Dynamic);
		tmp.body->CreateFixture(b2shape, 1.0f);
		delete b2shape;
		if (!bh.enable) {
			tmp.setColor(OC::randColor());
			tmp.bcolor = FT_Black;
		} else {
			tmp.setColor(FT_Black);
			tmp.bcolor = FT_White;
		}
		tmp.speed = ftVec2(ftAlgorithm::randRangef(-5.0f, 5.0f), ftAlgorithm::randRangef(-5.0f, 5.0f));
		tmp.body->SetLinearVelocity(b2Vec2(tmp.speed.x, tmp.speed.y));
		tmp.aSpeed = ftAlgorithm::randRangef(-1.0f, 1.0f);
		tmp.setTag(0);
		tmp.die = false;
		ocPool.add(tmp);
	}
	ocPool.doWith(ocSetUserData);
	if (mainClock.secondsFromContinue() >= 10.0f) {
		if (bh.enable == false) {
			bh.enable = true;
			ocPool.doWith(ocSetColor);
			ocPool.doWith(ocSetContact);
		}
	} else {

	}
	bh.update();

	char tt[10];
	std::sprintf(tt, "%d", mc.score);
	scoreB.setString(tt);
	if (!bh.enable) {
		std::sprintf(tt, "%.1f", 10.0f - mainClock.secondsFromContinue());
		timeB.setString(tt);
	}

	}
	if (over == true && state != 2) {
		state = 2;
		mc.body->SetTransform(b2Vec2(-fountain::mainWin.w / 4.0f / ftPhysics::getRatio(), 0), mc.body->GetAngle());
		mc.update();
		drawCon.update();
		ocPool.update();
	}
	if (state == 2) {
		screenC.update();
		repl.update();
		if (repl.getState() == FT_ButtonDown) {
			fountain::sceneSelector.gotoScene(new OpenScene());
		}
	}
}

void GameScene::otherDraw()
{
	mainCamera.update();

	ftVec2 target = mainCamera.mouseToWorld(fountain::sysMouse.getPos());
	ftVec2 line = target - mc.getPosition();
	line *= 0.125f;
	ftVec2 lineHM = bh.getPosition() - mainCamera.getPosition();
	ftVec2 lineHMC = bh.getPosition() - mc.getPosition();
	float vecH = lineHMC.getDegree();
	bh.draw();
	mc.draw();
	ocPool.draw();
	ftRender::useColor(FT_White);
	if (state == 1) {
		if (bh.enable && !bh.getRect().collideRect(mainCamera.getCameraRect())) {
			ftRender::useColor(FT_White);
			ftRect rct;
			rct = mainCamera.getCameraRect();
			std::vector<ftVec2> vv = rct.collideSegment(mainCamera.getPosition(), bh.getPosition());
			if (vv.size() > 0) {
				vv[0] -= lineHM / (lineHM.length() / 79.0f);
				ftRender::transformBegin();
				ftRender::ftTranslate(vv[0]);
				ftRender::ftRotate(0, 0, FT_R2D(vecH));
				ftRender::drawAlphaPic(ftRender::getPicture("res/image/arrow.png"));
				ftRender::transformEnd();
			}
		}
	}

	if (state == 1) {
		for (int i = 1; i < 8; i++) {
			ftRender::transformBegin();
			ftRender::ftTranslate(mc.getPosition() + (line * i));
			ftRender::drawCircle(5);
			ftRender::transformEnd();
		}
	}

	screenC.update();

	scoreB.setColor(FT_Black);
	timeB.setColor(FT_Black);
	scoreB.move(5, -5);
	timeB.move(5, -5);
	scoreB.draw();
	timeB.draw();

	scoreB.setColor(FT_White);
	timeB.setColor(FT_White);
	scoreB.move(-5, 5);
	timeB.move(-5, 5);
	scoreB.draw();
	timeB.draw();

	if (state == 2) {
		screenC.update();
		ftRender::useColor(FT_Black);
		ftRender::drawQuad(fountain::mainWin.w, fountain::mainWin.h / 2.0f);
		repl.draw();
		mc.drawAt(-fountain::mainWin.w / 4.0f, 0);
		drawCon.draw();
	}
}

void GameScene::destroy()
{
	delete world;
}

