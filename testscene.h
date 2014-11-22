Shape *rect;
Shape *testShape0;
Shape *testShape1;
Shape *testShape2;
Shape *addShape;
Shape *groundBox;
Shape *card;

Shape shape;

ftVec2 aP, bP;
ftRect makeRect;
ftScene::SceneSelector SS;
ftScene::Scene *scene;

bool mode = true;

double scale = 1.0f;
//int testPic;
ftVec2 deltaV;

//TODO: move this internal(ftTime)
ftTime::Clock mainClock(60.0);

//ft3DModel::ObjModel simpleModel("first.obj");

//TODO: move this internal(ftRender)
ftRender::Camera mainCamera(0, 0, 500);

ftPhysics::Body *bdPoint;
ftPhysics::World mainWorld(ftVec2(0, -10));

#define abs(x) ((x)>0?(x):-(x))
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((b)>(a)?(a):(b))

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

void init(ftScene::Scene* sc)
{
}


void update(ftScene::Scene* sc)
{
	ftVec2 mPos = fountain::sysMouse.getPos();
	mPos = mainCamera.mouseToWorld(mPos);

	if (fountain::sysKeyboard.getState(FT_Space) == FT_KeyUp) sc->gotoScene(FT_StartScene);
	if (fountain::sysKeyboard.getState(FT_W)) mainClock.Pause();
	if (fountain::sysKeyboard.getState(FT_R)) mainClock.Continue();

	if (fountain::sysKeyboard.getState(FT_1)) addShape = testShape0;
	if (fountain::sysKeyboard.getState(FT_2)) addShape = testShape1;
	if (fountain::sysKeyboard.getState(FT_3)) addShape = testShape2;
	if (fountain::sysKeyboard.getState(FT_4)) addShape = rect;

	if (fountain::sysKeyboard.getState(FT_M) == FT_KeyUp) mode = !mode;

	if (mode) {
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
	else {
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
				bdPoint->shape = Shape(makeRect);
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
				bdPoint->shape = Shape(makeRect);
				if (!mainWorld.addBody(bdPoint)) {
					mainWorld.delHeadBody();
					mainWorld.addBody(bdPoint);
				}
			}
		}
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
}

};

ftScene::Scene startScene(scene1::init, scene1::update, scene1::draw, scene1::destroy);
ftScene::Scene gameScene(scene2::init, scene2::update, scene2::draw, scene2::destroy);
