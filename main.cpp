#include <fountain.h>
#include <string>

#define abs(x) ((x)>0?(x):-(x))
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((b)>(a)?(a):(b))

ftPhysics::Body *bdPoint;
ftPhysics::World mainWorld(ftVec2(0, -10));

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

bool mode = true;

namespace Game {

double xAngle = 0.0f;
double yAngle = 0.0f;
double scale = 1.0f;
//int testPic;
ftVec2 deltaV;

//TODO: move this internal(ftTime)
ftTime::Clock mainClock(60.0);

//ft3DModel::ObjModel simpleModel("first.obj");

//TODO: move this internal(ftRender)
ftRender::Camera mainCamera(0, 0, 500);
};

void fountain::setBasicVarible()
{
	mainWin.setW(800);
	mainWin.setH(600);
	mainWin.title = std::string("fountain-prototype ") + std::string(FOUNTAIN_VERSION);
	mainWin.icon = "fountain.ico";
	mainWin.isFullScreen = false;
	mainWin.hideCursor = false;
}

void fountain::gameInit()
{
	ftRender::openLineSmooth();
	glLineWidth(1.5f);
	ftRender::openPointSmooth();

	Game::mainCamera.setWinSize(fountain::mainWin.w, fountain::mainWin.h);
	Game::mainClock.init();
//	Game::testPic = ftRender::getPicture("test.jpg");
//	glEnable(GL_DEPTH_TEST);

	std::vector<ftVec2> v;
	v.push_back(ftVec2(0,-2));
	v.push_back(ftVec2(1,0));
	v.push_back(ftVec2(0,2));
	v.push_back(ftVec2(-1,0));
	testShape0 = new Shape(v , 4, true);
	testShape1 = new Shape();
	testShape2 = new Shape(1);
	groundBox = new Shape(ftRect(0, 0, 100, 1));
	rect = new Shape(ftRect(0, 0, 1, 1));
	card = new Shape(ftRect(0, 0, 0.3, 3));
	addShape = rect;

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

void fountain::singleFrame()
{
	ftVec2 mPos = fountain::sysMouse.getPos();
	mPos = Game::mainCamera.mouseToWorld(mPos);

	//TODO: move this internal
	mainWorld.update(Game::mainClock.getDeltaT());

	if (fountain::sysKeyboard.getState(FT_W)) Game::mainClock.Pause();
	if (fountain::sysKeyboard.getState(FT_R)) Game::mainClock.Continue();

	if (fountain::sysKeyboard.getState(FT_1)) addShape = testShape0;
	if (fountain::sysKeyboard.getState(FT_2)) addShape = testShape1;
	if (fountain::sysKeyboard.getState(FT_3)) addShape = testShape2;
	if (fountain::sysKeyboard.getState(FT_4)) addShape = rect;

	if (fountain::sysKeyboard.getState(FT_M) == FT_KeyUp) mode = !mode;

	//TODO: move these to update(ftScene hook)
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
		Game::scale *= 1.15f;
	}
	if (fountain::sysMouse.getState(FT_ScrollDown)) {
		Game::scale /= 1.15f;
	}
	if (fountain::sysMouse.getState(FT_MButton)) {
		Game::deltaV = fountain::sysMouse.getDeltaV();
		Game::mainCamera.move(-Game::deltaV.x / Game::scale, -Game::deltaV.y / Game::scale);
	}

	Game::mainCamera.setScale(Game::scale);
	Game::mainCamera.update();

//	ftRender::transformBegin();
//	ftRender::drawPic(Game::testPic);
//	ftRender::transformEnd();

	mainWorld.draw();

	//TODO: move this internal(fountainMain)
	Game::mainClock.tick();
}
