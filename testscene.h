#include <Box2D/Box2D.h>

float ttt;

class QueryCallback : public b2QueryCallback
{
public:
         QueryCallback(const b2Vec2& point)
         {
                 m_point = point;
                 m_fixture = NULL;
         }

         bool ReportFixture(b2Fixture* fixture)
         {
                 b2Body* body = fixture->GetBody();
                 if (body->GetType() == b2_dynamicBody)
                 {
                         bool inside = fixture->TestPoint(m_point);
                         if (inside)
                         {
                                 m_fixture = fixture;

                                 // We are done, terminate the query.
                                 return false;
                         }
                 }

                 // Continue the query.
                 return true;
         }

         b2Vec2 m_point;
         b2Fixture* m_fixture;
};


namespace scene1 {

int pic1;
int pic2;
float transp;
float scale;
float x = 0;
ftRender::Camera mainCamera(0, 0, 500);

void init(ftScene::Scene* sc)
{
	ttt = 1;
	pic1 = ftRender::getPicture("./logo.png");
	pic2 = ftRender::getPicture("./title.png");
	transp = 0.0;
	scale = 1.0;
	x = 0;
	mainCamera.setViewport(fountain::mainWin.w, fountain::mainWin.h);
}

void update(ftScene::Scene* sc)
{
	if (ttt > 0.01) ttt -= 0.01;
	if (transp > 2.0) {
		ttt += 0.02;
		if (ttt > 1)
			sc->gotoScene(FT_Scene2);
	}
	if (scale > 0.30) {
		x -= 1.7;
		scale -= 0.006;
	} else transp += 0.008;
}

void draw(ftScene::Scene* sc)
{
	mainCamera.update();
	ftRender::transformBegin();
	ftRender::ftTranslate(x, 0);
	ftRender::ftScale(scale);
	ftRender::drawAlphaPic(pic1);
	ftRender::transformEnd();

	ftRender::transformBegin();
	ftRender::ftTransparency(transp);
	ftRender::ftTranslate(90, 0);
	ftRender::drawAlphaPic(pic2);
	ftRender::transformEnd();

	ftRender::transformBegin();
	ftRender::ftColor4f(0, 0, 0, ttt);
	ftRender::drawQuad(fountain::mainWin.w, fountain::mainWin.h);
	ftRender::transformEnd();

}

void destroy(ftScene::Scene* sc)
{
}

};

namespace scene2 {

ftShape shape;

ftVec2 aP, bP;
ftRect makeRect;
ftScene::Scene *scene;

int mode;

double scale;
//int testPic;
ftVec2 deltaV;
ftShape *rect;
ftShape *testShape0;
ftShape *testShape1;
ftShape *testShape2;
ftShape *addShape;
ftShape *groundBox;
ftShape *card;
ftRender::Camera mainCamera(0, 0, 500);
std::vector<ftVec2> v;

ftPhysics::Body *bdPoint;
ftPhysics::World mainWorld(ftVec2(0, -10));

//Test
b2MouseJoint *mouseJoint;
ftRender::SubImage xx;
ftRender::SubImage yy;

void MouseDown(ftVec2 p)
{
	b2World *world = mainWorld.world;
	b2AABB aabb;
	ftVec2 d(0.001f, 0.001f);
	ftVec2 upB = p - d;
	ftVec2 downB = p + d;

	aabb.lowerBound = b2Vec2(downB.x, downB.y);
	aabb.upperBound = b2Vec2(upB.x, upB.y);

	QueryCallback callback(b2Vec2(p.x, p.y));
	world->QueryAABB(&callback, aabb);

	if (callback.m_fixture) {
		b2Body* body = callback.m_fixture->GetBody();
		b2MouseJointDef md;
		//md.bodyA = groundBoxx;
		md.bodyA = body;
		md.bodyB = body;
		md.target = b2Vec2(p.x, p.y);
		md.maxForce = 1000.0f * body->GetMass();
		mouseJoint = (b2MouseJoint*)world->CreateJoint(&md);
		body->SetAwake(true);
	}
}

void MouseMove(ftVec2 p)
{
	if (mouseJoint != NULL) {
		mouseJoint->SetTarget(b2Vec2(p.x, p.y));
	}
}

void MouseUp()
{
	b2World *world = mainWorld.world;
	if (mouseJoint) {
		world->DestroyJoint(mouseJoint);
		mouseJoint = NULL;
	}
}

ftColor randColor()
{
	float r = ftAlgorithm::randRangef(0.5f, 1.0f);
	float g = ftAlgorithm::randRangef(0.5f, 1.0f);
	float b = ftAlgorithm::randRangef(0.5f, 1.0f);
	return ftColor(r, g, b);
}

void init(ftScene::Scene* sc)
{
	ttt = 1;
	mainCamera.setViewport(fountain::mainWin.w, fountain::mainWin.h);
	mainCamera.setPosition(0, 0, 500);
	scale = 1.0f;
	mode = 1;
	ftPhysics::setRatio(128.0f);

	v.clear();
	v.push_back(ftVec2(0,-50));
	v.push_back(ftVec2(25,0));
	v.push_back(ftVec2(0,50));
	v.push_back(ftVec2(-25,0));
	testShape0 = new ftShape(v , 4, true);
	testShape1 = new ftShape(10);
	testShape2 = new ftShape(20);
	groundBox = new ftShape(ftRect(0, 0, 2000, 20));
	rect = new ftShape(ftRect(0, 0, 20, 20));
	card = new ftShape(ftRect(0, 0, 6, 60));
	addShape = rect;

	//test
	ftRender::SubImagePool ui("resources/ui.png", "resources/ui.sip");
	xx = ui.getImage("button01.png");
	yy = ui.getImage("button02.png");

	//Test
	mouseJoint = NULL;

	bdPoint = new ftPhysics::Body(0, -200, false);
	bdPoint->shape = *groundBox;
	bdPoint->setColor(randColor());
	if (!mainWorld.addBody(bdPoint)) {
		mainWorld.delHeadBody();
		mainWorld.addBody(bdPoint);
	}

	for (int i = -2000; i <= 2000; i += 40) {
		bdPoint = new ftPhysics::Body(i, -200 + 40);
		bdPoint->shape = *card;
		bdPoint->setColor(randColor());
		if (!mainWorld.addBody(bdPoint)) {
			mainWorld.delHeadBody();
			mainWorld.addBody(bdPoint);
		}
	}
}


void update(ftScene::Scene* sc)
{
	if (ttt > 0.01) ttt -= 0.01;

	ftVec2 mPos = fountain::sysMouse.getPos();
	mPos = mainCamera.mouseToWorld(mPos);

	if (fountain::sysKeyboard.getState(FT_P) == FT_KeyUp) sc->gotoScene(FT_Scene2, true);
	if (fountain::sysKeyboard.getState(FT_T) == FT_KeyUp) sc->gotoScene(FT_Scene3, true);
	if (fountain::sysKeyboard.getState(FT_Space) == FT_KeyDown) {
		if (mainClock.isPause())
			mainClock.Continue();
		else
			mainClock.Pause();
	}


	int tmp = ftAlgorithm::randRangef(1, 5);

	switch (tmp) {
	case 1:
		addShape = testShape0;
		break;
	case 2:
		addShape = testShape2;
		break;
	case 3:
		addShape = testShape1;
		break;
	case 4:
		addShape = rect;
		break;
	}

	if (fountain::sysKeyboard.getState(FT_1)) mode = 1;
	if (fountain::sysKeyboard.getState(FT_2)) mode = 2;
	if (fountain::sysKeyboard.getState(FT_3)) mode = 3;

	if (mode == 1) {
		if (fountain::sysMouse.getState(FT_LButton)) {
			bdPoint = new ftPhysics::Body(mPos.x, mPos.y);
			bdPoint->shape = *addShape;
			bdPoint->setColor(randColor());
			if (!mainWorld.addBody(bdPoint)) {
				mainWorld.delHeadBody();
				mainWorld.addBody(bdPoint);
			}
		}

		if (fountain::sysMouse.getState(FT_RButton)) {
			bdPoint = new ftPhysics::Body(mPos.x, mPos.y, false);
			bdPoint->shape = *addShape;
			bdPoint->setColor(randColor());
			if (!mainWorld.addBody(bdPoint)) {
				mainWorld.delHeadBody();
				mainWorld.addBody(bdPoint);
			}
		}
	}
	if (mode == 2) {
		if (fountain::sysMouse.getState(FT_LButton) == FT_ButtonDown) {
			aP = mPos;
		}
		else if (fountain::sysMouse.getState(FT_LButton) == FT_isDown) {
			bP = mPos;
			makeRect = ftRect(aP.x, aP.y, bP.x - aP.x, bP.y - aP.y);
			makeRect.normalize();
			ftRender::drawRect(makeRect);
		}
		else if (fountain::sysMouse.getState(FT_LButton) == FT_ButtonUp) {
			ftVec2 tmpP = makeRect.getCenter();
			if (makeRect.getSize().x > 0.01 && makeRect.getSize().y > 0.01) {
				bdPoint = new ftPhysics::Body(tmpP.x, tmpP.y, true);
				bdPoint->shape = ftShape(makeRect);
				bdPoint->setColor(randColor());
				if (!mainWorld.addBody(bdPoint)) {
					mainWorld.delHeadBody();
					mainWorld.addBody(bdPoint);
				}
			}
		}
		else if (fountain::sysMouse.getState(FT_RButton) == FT_ButtonDown) {
			aP = mPos;
		}
		else if (fountain::sysMouse.getState(FT_RButton) == FT_isDown) {
			bP = mPos;
			makeRect = ftRect(aP.x, aP.y, bP.x - aP.x, bP.y - aP.y);
			makeRect.normalize();
			ftRender::drawRect(makeRect);
		}
		else if (fountain::sysMouse.getState(FT_RButton) == FT_ButtonUp) {
			ftVec2 tmpP = makeRect.getCenter();
			if (makeRect.getSize().x > 0.1 && makeRect.getSize().y > 0.1) {
				bdPoint = new ftPhysics::Body(tmpP.x, tmpP.y, false);
				bdPoint->shape = ftShape(makeRect);
				bdPoint->setColor(randColor());
				if (!mainWorld.addBody(bdPoint)) {
					mainWorld.delHeadBody();
					mainWorld.addBody(bdPoint);
				}
			}
		}
	}
	if (mode == 3) {
		if (fountain::sysMouse.getState(FT_LButton) == FT_ButtonDown) MouseDown(ftPhysics::render2Physics(mPos));
		if (fountain::sysMouse.getState(FT_LButton) == FT_isDown) MouseMove(ftPhysics::render2Physics(mPos));
		if (fountain::sysMouse.getState(FT_LButton) == FT_ButtonUp) MouseUp();
	}

	if (fountain::sysMouse.getState(FT_ScrollUp)) {
		scale *= 1.15f;
	}
	if (fountain::sysMouse.getState(FT_ScrollDown)) {
		scale /= 1.15f;
	}
	if (fountain::sysMouse.getState(FT_MButton)) {
		deltaV = fountain::sysMouse.getDeltaV();
		mainCamera.move(-deltaV.x / scale, -deltaV.y / scale);
	}

	mainWorld.update(mainClock.getDeltaT());
	mainCamera.setScale(scale);
}

void draw(ftScene::Scene* sc)
{
	mainCamera.update();
	ftRender::transformBegin();
	ftRender::useColor(FT_Orange);
	mainWorld.draw();
	ftRender::transformEnd();

	ftRender::transformBegin();
	ftRender::ftColor4f(0, 0, 0, ttt);
	ftRender::drawQuad(fountain::mainWin.w, fountain::mainWin.h);
	ftRender::transformEnd();

	ftRender::transformBegin();
	ftRender::ftTranslate(0, 100);
	if (fountain::sysMouse.getState(FT_LButton))
		ftRender::drawImage(yy);
	else
		ftRender::drawImage(xx);
	ftRender::transformEnd();
}

void destroy(ftScene::Scene* sc)
{
	delete testShape0;
	delete testShape1;
	delete testShape2;
	delete card;
	delete groundBox;
	delete rect;
	while (!mainWorld.empty())
		mainWorld.delHeadBody();
}

};

namespace scene3 {

ft3DModel::ObjModel robot("first.obj");
ftRender::Camera mainCamera(0, 0, 500);
float scale;
float dx, dy;
float cx, cy;

void init(ftScene::Scene* sc)
{
	mainCamera.setViewport(fountain::mainWin.w, fountain::mainWin.h);
	mainCamera.setProjectionType(FT_PERSPECTIVE);
	scale = 1.0f;
	dx = 0.0f;
	dy = 0.0f;
	cx = 0.0f;
	cy = 0.0f;
}

void update(ftScene::Scene* sc)
{
	if (fountain::sysKeyboard.getState(FT_T) == FT_KeyUp) sc->gotoScene(FT_Scene2, true);
	if (fountain::sysMouse.getState(FT_ScrollUp)) scale *= 1.1f;
	if (fountain::sysMouse.getState(FT_ScrollDown)) scale /= 1.1f;
	if (fountain::sysMouse.getState(FT_LButton) == FT_isDown) {
		ftVec2 deltaV = fountain::sysMouse.getDeltaV();
		dy += deltaV.x;
		dx -= deltaV.y;
	} else if (fountain::sysMouse.getState(FT_MButton) == FT_isDown) {
		ftVec2 cDeltaV = fountain::sysMouse.getDeltaV();
		cDeltaV = cDeltaV * -1;
		mainCamera.move(cDeltaV.x, cDeltaV.y);
	}
}

void draw(ftScene::Scene* sc)
{
	mainCamera.update();
	ftRender::transformBegin();
	ftRender::ftRotate(dx, dy, 0);
	ftRender::ftScale(scale);
	robot.render();
	ftRender::transformEnd();
	ftRender::transformBegin();
	ftRender::useColor(ftColor("#F66"));
	ftRender::drawQuad(100, 100);
	ftRender::transformEnd();
}

void destroy(ftScene::Scene* sc)
{
}

};

ftScene::Scene startScene(scene1::init, scene1::update, scene1::draw, scene1::destroy);
ftScene::Scene gameScene(scene2::init, scene2::update, scene2::draw, scene2::destroy);
ftScene::Scene modelScene(scene3::init, scene3::update, scene3::draw, scene3::destroy);
