#include <fountain.h>
#include <string>
#define abs(x) ((x)>0?(x):-(x))

ftPhysics::Body* bdPoint;
ftRect rect;
Shape* testShape;
Shape* testShape1;
Shape shape;

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

	for (int i = 0; i < 10; i++) {
		for (int j = 0 - i; j <= i; j++) {
			bdPoint = new ftPhysics::Body(j, -i, false);
			rect.setSize(ftVec2(1, 1));
			shape = Shape(rect);
			bdPoint->shape = shape;
			if (!fountain::mainWorld.addBody(bdPoint)) {
				fountain::mainWorld.delHeadBody();
				fountain::mainWorld.addBody(bdPoint);
			}
		}
	}

	bdPoint = new ftPhysics::Body(0, -100, false);
	bdPoint->setRectSize(ftVec2(100, 1));
	rect.setSize(ftVec2(100, 1));
	rect.setCenter(ftVec2(0, 0));
	shape = Shape(rect);
	bdPoint->shape = shape;
	if (!fountain::mainWorld.addBody(bdPoint)) {
		fountain::mainWorld.delHeadBody();
		fountain::mainWorld.addBody(bdPoint);
	}

	for (int i = -50; i <= 50; i+=2) {
		bdPoint = new ftPhysics::Body(i, -100 + 3.6);
		bdPoint->setRectSize(ftVec2(0.5, 3));
		rect.setSize(ftVec2(0.5, 3));
		rect.setCenter(ftVec2(0, 0));
		shape = Shape(rect);
		bdPoint->shape = shape;
		if (!fountain::mainWorld.addBody(bdPoint)) {
			fountain::mainWorld.delHeadBody();
			fountain::mainWorld.addBody(bdPoint);
		}
	}
	std::vector<ftVec2> v;
	v.push_back(ftVec2(1,1));
	v.push_back(ftVec2(2,2));
	v.push_back(ftVec2(3,1));
	v.push_back(ftVec2(2,-1));
	v.push_back(ftVec2(1,0));
	testShape = new Shape(v , 5, true);
	testShape1 = new Shape();
}

void fountain::singleFrame()
{
	ftVec2 mPos = fountain::sysMouse.getPos();
	mPos = Game::mainCamera.mouseToWorld(mPos);

	//TODO: move this internal
	fountain::mainWorld.update(Game::mainClock.getDeltaT());

	if (fountain::sysKeyboard.getState(FT_W)) Game::mainClock.Pause();
	if (fountain::sysKeyboard.getState(FT_R)) Game::mainClock.Continue();

	//TODO: move these to update(ftScene hook)
	if (fountain::sysMouse.getState(FT_LButton)) {
		bdPoint = new ftPhysics::Body(mPos.x, mPos.y);
		rect.setSize(ftVec2(1, 1));
		rect.setCenter(ftVec2(0, 0));
		shape = Shape(rect);
		bdPoint->shape = shape;
		if (!fountain::mainWorld.addBody(bdPoint)) {
			fountain::mainWorld.delHeadBody();
			fountain::mainWorld.addBody(bdPoint);
		}
	}

	if (fountain::sysMouse.getState(FT_RButton)) {
		bdPoint = new ftPhysics::Body(mPos.x, mPos.y, false);
		rect.setSize(ftVec2(1, 1));
		rect.setCenter(ftVec2(0, 0));
		shape = Shape(rect);
		bdPoint->shape = shape;
		if (!fountain::mainWorld.addBody(bdPoint)) {
			fountain::mainWorld.delHeadBody();
			fountain::mainWorld.addBody(bdPoint);
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

	fountain::mainWorld.draw();

	ftRender::transformBegin();
	ftRender::ftTranslate(10,10);
	ftRender::drawShape(*testShape);
	ftRender::transformEnd();

	ftRender::transformBegin();
	ftRender::drawShape(*testShape1);
	ftRender::transformEnd();

	//TODO: move this internal(fountainMain)
	Game::mainClock.tick();
}
