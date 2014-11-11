#include <fountain.h>
#include <string>
#include <utility>
#define abs(x) ((x)>0?(x):-(x))

void Sprite::draw()
{
	ftRender::transformBegin();
	ftRender::ftTranslate(Sprite::getPosition().x, Sprite::getPosition().y);
	ftRender::ftRotate(0.0f, 0.0f, Sprite::getAngle());
	ftRender::drawLine(ftVec2(-0.5, -0.5), ftVec2(-0.5, 0.5));
	ftRender::drawLine(ftVec2(-0.5, 0.5),  ftVec2(0.5, 0.5));
	ftRender::drawLine(ftVec2(0.5, 0.5),   ftVec2(0.5, -0.5));
	ftRender::drawLine(ftVec2(0.5, -0.5),  ftVec2(-0.5, -0.5));
	ftRender::transformEnd();
}

void Sprite::update()
{
}

ftPhysics::Body* bdPoint;

namespace Game {
double xAngle = 0.0f;
double yAngle = 0.0f;
double scale = 1.0f;
int testPic;
ftVec2 deltaV;

//TODO: move this internal(ftTime)
ftTime::Clock mainClock(60.0);

ft3DModel::ObjModel simpleModel("first.obj");

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
	glLineWidth(2.0f);
	ftRender::openPointSmooth();
	Game::mainCamera.setWinSize(fountain::mainWin.w, fountain::mainWin.h);
	Game::testPic = ftRender::getPicture("test.jpg");
	Game::mainClock.init();
	glEnable(GL_DEPTH_TEST);

	//TODO: move this internal(ftPhysics)
	for (int i = 0; i < 10; i++) {
		for (int j = 0 - i; j <= i; j++) {
			bdPoint = new ftPhysics::Body(j, -i, false);
			fountain::mainWorld.addBody(bdPoint);
		}
	}
	//TODO: add particle generater to do this
	for (int i = 0; i < 1000; i++) {
		float xx = ftAlgorithm::randRangef(-100, 100);
		float yy = ftAlgorithm::randRangef(100, 1000);
		bdPoint = new ftPhysics::Body(xx, yy);
		fountain::mainWorld.addBody(bdPoint);
	}
}

void fountain::singleFrame()
{
	//TODO: move this internal
	fountain::mainWorld.update();

	//TODO: move this to update(ftScene hook)
	if (fountain::sysMouse.getState(4)) {
		Game::scale *= 1.15f;
	}
	if (fountain::sysMouse.getState(5)) {
		Game::scale /= 1.15f;
	}
	if (fountain::sysMouse.getState(1)) {
		Game::deltaV = fountain::sysMouse.getDeltaV();
		Game::mainCamera.move(-Game::deltaV.x / Game::scale, -Game::deltaV.y / Game::scale);
	}
	if (fountain::sysKeyboard.getState(FT_W))
		Game::mainCamera.move(0, 3);
	if (fountain::sysKeyboard.getState(FT_S))
		Game::mainCamera.move(0, -3);
	if (fountain::sysKeyboard.getState(FT_A))
		Game::mainCamera.move(-3, 0);
	if (fountain::sysKeyboard.getState(FT_D))
		Game::mainCamera.move(3, 0);

	Game::mainCamera.setScale(Game::scale);
	Game::mainCamera.update();

	fountain::mainWorld.draw();

	//TODO: move this internal(fountainMain)
	Game::mainClock.tick();
}
