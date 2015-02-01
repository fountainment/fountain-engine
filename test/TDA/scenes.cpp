#include "scenes.h"
#include <vector>

void TDAScene::init()
{
	mainCamera.setViewport(fountain::getWinRect());

	std::vector<ftVec2> v;
	v.push_back(ftVec2(-12, -20));
	v.push_back(ftVec2(12, -20));
	v.push_back(ftVec2(0, 27));
	ftShape x = ftShape(v, 3, true);

	fly = Body(-200, 0);
	fly.setShape(x);
	fly.setColor(ftColor("#F99", 0.5f));

	ball = Body(0, 100);
	ball.setShape(ftShape(10));

	world.addBody(&fly);
	fly.body->SetFixedRotation(true);
	fly.body->SetGravityScale(0.0f);
	flySpeed = 400.0f;
}

void TDAScene::update()
{
	world.update(mainClock.getDeltaT());
	if (fountain::sysKeyboard.getState(FT_L))
	flySpeed = 200.0f;
	else flySpeed = 400.0f;
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
	mainCamera.update();
}

void TDAScene::draw()
{
	world.draw();
}
