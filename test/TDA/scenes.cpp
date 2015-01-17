#include "scenes.h"
#include <vector>

void TDAScene::init()
{
	mainCamera.setViewport(fountain::getWinRect());

	std::vector<ftVec2> v;
	v.push_back(ftVec2(-10, -5));
	v.push_back(ftVec2(-10, 5));
	v.push_back(ftVec2(10, 7));
	v.push_back(ftVec2(25, -5));
	ftShape x = ftShape(v, 4, true);

	fly = Body(-200, 0);
	fly.setShape(x);
	fly.setColor(ftColor("#CCCCFF"));

	ground = Body(0, -250, FT_Static);
	ground.setShape(ftRect(0, 0, 800, 50));
	ground.setColor(ftColor("#88FF88"));

	ball = Body(0, 0);
	ball.setShape(ftShape(10));

	for (int i = 0; i < 10; i++) {
		Body *b = new Body();
		*b = ball;
		world.addBody(b);
	}

	world.addBody(&fly);
	fly.body->SetFixedRotation(true);
	fly.body->SetGravityScale(0.0f);
	world.addBody(&ground);
	flySpeed = 400.0f;
}

void TDAScene::update()
{
	world.update();
	ftVec2 speed(0, 0);
	if (fountain::sysKeyboard.getState(FT_W))
		speed.move(0, flySpeed);
	if (fountain::sysKeyboard.getState(FT_A))
		speed.move(-flySpeed, 0);
	if (fountain::sysKeyboard.getState(FT_S))
		speed.move(0, -flySpeed);
	if (fountain::sysKeyboard.getState(FT_D))
		speed.move(flySpeed, 0);
	fly.setVelocity(speed);
	if (fountain::sysKeyboard.getState(FT_L))
		flySpeed = 200.0f;
	else flySpeed = 400.0f;
	mainCamera.update();
}

void TDAScene::draw()
{
	world.draw();
}

