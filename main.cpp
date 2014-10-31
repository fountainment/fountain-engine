#include <fountain.h>
#include <string>
#include <Box2D/Box2D.h>
#include <utility>
#define abs(x) ((x)>0?(x):-(x))

container < std::pair<ftVec2, b2Body*>, 1000 > con;
b2Vec2 gravity(0.0f, -100.0f);
b2World world(gravity);
b2BodyDef groundBodyDef;
b2Body* groundBody;
b2PolygonShape groundBox;
b2BodyDef bodyDef;
b2PolygonShape dynamicBox;
b2FixtureDef fixtureDef;
b2Body* body;

void drawPot(std::pair<ftVec2, b2Body*> pp)
{
	ftVec2 color = pp.first;
	b2Body* body = pp.second;
	b2Vec2 vec = body->GetPosition();
	glPointSize(2.0f);
	glColor3f(color.x, color.y, 0.3f);
	glPushMatrix();
	ftRender::ftTranslate(vec.x, vec.y);
	glRotatef(body->GetAngle() * 180.0f / 3.14159f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	glVertex2f(-0.5f, -0.5f);
	glVertex2f(0.5f, -0.5f);
	glVertex2f(0.5f, 0.5f);
	glVertex2f(-0.5f, 0.5f);
	glEnd();
	glPopMatrix();
}

namespace Game {
	double xAngle = 0.0f;
	double yAngle = 0.0f;
	double scale = 1.0f;
	int testPic;
	ftVec2 deltaV;
	 ftTime::Clock mainClock(60.0);
	 ft3DModel::ObjModel simpleModel("first.obj");
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
	//Game::mainCamera.setProjectionType(FT_PERSPECTIVE);
	Game::mainCamera.setWinSize(fountain::mainWin.w, fountain::mainWin.h);
	Game::testPic = ftRender::getPicture("test.jpg");
	Game::mainClock.init();
	glEnable(GL_DEPTH_TEST);
	groundBodyDef.position.Set(0.0f, -5.0f);
	groundBody = world.CreateBody(&groundBodyDef);
	groundBox.SetAsBox(50.0f, 5.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);
	bodyDef.type = b2_dynamicBody;
	dynamicBox.SetAsBox(0.5f, 0.5f);
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	for (int i = 0; i < 1000; i++) {
		float xx = ftAlgorithm::randRangef(-100, 100);
		float yy = ftAlgorithm::randRangef(100, 1000);
		bodyDef.position.Set(xx, yy);
		body = world.CreateBody(&bodyDef);
		body->CreateFixture(&fixtureDef);
		con.add(std::make_pair(ftVec2(ftAlgorithm::randRangef(0.0, 1.0), ftAlgorithm::randRangef(0.0, 1.0)), body));
	}
}

void fountain::singleFrame()
{
	world.Step(1.0f / 60.0f, 6, 2);
	Game::deltaV = fountain::sysMouse.getDeltaV();
	Game::xAngle -= Game::deltaV.y;
	Game::yAngle += Game::deltaV.x;
	if (fountain::sysMouse.getState(1))
		Game::scale *= 1.1f;
	if (fountain::sysKeyboard.getState(FT_W))
		Game::scale += 5.0f * Game::mainClock.getDeltaT();
	if (fountain::sysMouse.getState(3))
		Game::scale /= 1.1f;
	if (fountain::sysKeyboard.getState(FT_S))
		Game::scale -= 5.0f * Game::mainClock.getDeltaT();
	Game::mainCamera.setScale(Game::scale);
	Game::mainCamera.update();

	ftRender::transformBegin();
	//ftRender::ftRotate(Game::xAngle, Game::yAngle, 0.0f);
	//ftRender::ftScale(Game::scale);
	ftRender::drawLine(-50.0f, 0.0f, 50.0f, 0.0f);
	con.doWith(drawPot);
	ftRender::transformEnd();


/*
	ftRender::transformBegin();
	ftRender::ftRotate(Game::xAngle, Game::yAngle, 0.0f);
	ftRender::ftScale(Game::scale);
	Game::simpleModel.render();
	ftRender::transformEnd();

	ftRender::transformBegin();
	ftRender::ftRotate(Game::xAngle, Game::yAngle, 0.0f);
	ftRender::drawPic(Game::testPic);
	ftRender::transformEnd();

	ftRender::transformBegin();
	ftRender::ftRotate(Game::xAngle, Game::yAngle, 0.0f);
	ftRender::drawLine(ftAlgorithm::randRangef(-110, -90), -100, 100, 100);
	ftRender::transformEnd();
*/
	Game::mainClock.tick();
}
