#include <Box2D/Box2D.h>
#define abs(x) ((x)>0?(x):-(x))
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((b)>(a)?(a):(b))

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
ftRender::Camera mainCamera(0, 0, 500);

void init(ftScene::Scene* sc)
{
	pic1 = ftRender::getPicture("./test.jpg");
	mainCamera.setWinSize(fountain::mainWin.w, fountain::mainWin.h);
}

void update(ftScene::Scene* sc)
{
	if (fountain::sysKeyboard.getState(FT_Space) == FT_KeyUp)
		sc->gotoScene(FT_Scene2);
}

void draw(ftScene::Scene* sc)
{
	mainCamera.update();
	ftRender::drawPic(pic1);
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

int mode = 1;

double scale = 1.0f;
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
b2Body *groundBoxx;

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
		md.bodyA = groundBoxx;
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

void init(ftScene::Scene* sc)
{
	mainCamera.setWinSize(fountain::mainWin.w, fountain::mainWin.h);
	mainCamera.setPosition(0, 0, 500);
	scale = 1.0f;
	mode = 1;

	v.clear();
	v.push_back(ftVec2(0,-2));
	v.push_back(ftVec2(1,0));
	v.push_back(ftVec2(0,2));
	v.push_back(ftVec2(-1,0));
	testShape0 = new ftShape(v , 4, true);
	testShape1 = new ftShape();
	testShape2 = new ftShape(1);
	groundBox = new ftShape(ftRect(0, 0, 100, 1));
	rect = new ftShape(ftRect(0, 0, 1, 1));
	card = new ftShape(ftRect(0, 0, 0.3, 3));
	addShape = rect;

	//Test
	bdPoint = new ftPhysics::Body(0, -1000, false);
	mainWorld.addBody(bdPoint);
	groundBoxx = bdPoint->body;
	mouseJoint = NULL;

	for (int i = 0; i < 10; i++) {
		for (int j = 0 - i; j <= i; j++) {
			bdPoint = new ftPhysics::Body(j, -i, false);
			bdPoint->shape = *addShape;
			if (!mainWorld.addBody(bdPoint)) {
				mainWorld.delHeadBody();
				mainWorld.addBody(bdPoint);
			}
		}
	}

	bdPoint = new ftPhysics::Body(0, -100, false);
	bdPoint->shape = *groundBox;
	if (!mainWorld.addBody(bdPoint)) {
		mainWorld.delHeadBody();
		mainWorld.addBody(bdPoint);
	}

	for (int i = -50; i <= 50; i+=2) {
		bdPoint = new ftPhysics::Body(i, -100 + 3.6);
		bdPoint->shape = *card;
		if (!mainWorld.addBody(bdPoint)) {
			mainWorld.delHeadBody();
			mainWorld.addBody(bdPoint);
		}
	}
}


void update(ftScene::Scene* sc)
{
	ftVec2 mPos = fountain::sysMouse.getPos();
	mPos = mainCamera.mouseToWorld(mPos);

	if (fountain::sysKeyboard.getState(FT_Space) == FT_KeyUp) sc->gotoScene(FT_StartScene, true);
	if (fountain::sysKeyboard.getState(FT_Q)) mainClock.Pause();
	if (fountain::sysKeyboard.getState(FT_W)) mainClock.Continue();

	if (fountain::sysKeyboard.getState(FT_1)) addShape = testShape0;
	if (fountain::sysKeyboard.getState(FT_2)) addShape = testShape1;
	if (fountain::sysKeyboard.getState(FT_3)) addShape = testShape2;
	if (fountain::sysKeyboard.getState(FT_4)) addShape = rect;

	if (fountain::sysKeyboard.getState(FT_E)) mode = 1;
	if (fountain::sysKeyboard.getState(FT_R)) mode = 2;
	if (fountain::sysKeyboard.getState(FT_T)) mode = 3;

	if (mode == 1) {
		if (fountain::sysMouse.getState(FT_LButton)) {
			bdPoint = new ftPhysics::Body(mPos.x, mPos.y);
			bdPoint->shape = *addShape;
			if (!mainWorld.addBody(bdPoint)) {
				mainWorld.delHeadBody();
				mainWorld.addBody(bdPoint);
			}
		}

		if (fountain::sysMouse.getState(FT_RButton)) {
			bdPoint = new ftPhysics::Body(mPos.x, mPos.y, false);
			bdPoint->shape = *addShape;
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
			makeRect = ftRect(min(aP.x, bP.x), min(aP.y, bP.y), abs(aP.x - bP.x), abs(aP.y - bP.y));
			ftRender::drawRect(makeRect);
		}
		else if (fountain::sysMouse.getState(FT_LButton) == FT_ButtonUp) {
			ftVec2 tmpP = makeRect.getCenter();
			if (makeRect.getSize().x > 0.1 && makeRect.getSize().y > 0.1) {
				bdPoint = new ftPhysics::Body(tmpP.x, tmpP.y, true);
				bdPoint->shape = ftShape(makeRect);
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
			makeRect = ftRect(min(aP.x, bP.x), min(aP.y, bP.y), abs(aP.x - bP.x), abs(aP.y - bP.y));
			ftRender::drawRect(makeRect);
		}
		else if (fountain::sysMouse.getState(FT_RButton) == FT_ButtonUp) {
			ftVec2 tmpP = makeRect.getCenter();
			if (makeRect.getSize().x > 0.1 && makeRect.getSize().y > 0.1) {
				bdPoint = new ftPhysics::Body(tmpP.x, tmpP.y, false);
				bdPoint->shape = ftShape(makeRect);
				if (!mainWorld.addBody(bdPoint)) {
					mainWorld.delHeadBody();
					mainWorld.addBody(bdPoint);
				}
			}
		}
	}
	if (mode == 3) {
		if (fountain::sysMouse.getState(FT_LButton) == FT_ButtonDown) MouseDown(mPos);	
		if (fountain::sysMouse.getState(FT_LButton) == FT_isDown) MouseMove(mPos);	
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
	mainWorld.draw();
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

ftScene::Scene startScene(scene1::init, scene1::update, scene1::draw, scene1::destroy);
ftScene::Scene gameScene(scene2::init, scene2::update, scene2::draw, scene2::destroy);
