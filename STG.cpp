#include "STG.h"

using STG::Bullet;
using STG::BulletCon;
using STG::MainCharactor;
using STG::Enemy;

//class STG::Bullet
void Bullet::setSpeed(ftVec2 v)
{
	speed = v;
}

void Bullet::update()
{
	float deltaT = fountain::getCurSceneDeltaT();
	move(speed * deltaT);
}

void Bullet::draw()
{
	ftVec2 pos = getPosition();
	ftRender::transformBegin(pos.x, pos.y);
	ftRender::useColor(ftColor("#F99"));
	ftRender::drawCircle(3.0f);
	ftRender::useColor(FT_Red);
	ftRender::drawCircleEdge(5.0f);
	ftRender::transformEnd();
}

//class STG::BulletCon
bool BulletCon::willLive(Bullet & b)
{
	ftRender::Camera *cam = &((fountain::sceneSelector.getCurScene())->mainCamera);
	if (cam != NULL) {
		ftRect cr = cam->getCameraRect();
		ftVec2 xy = cr.getCenter();
		cr.inflate(2, 2);
		cr.setCenter(xy);
		return cr.collidePoint(b.getPosition());
	}
	return false;
}

//class STG::MainCharactor
void MainCharactor::init()
{
	animeTest = ftRender::SubImagePool("resources/image/Breath.png", "resources/image/Breath.sip");
	breath = ftAnime::FrameAnime(animeTest, 15.0f);
	setCurAnime(&breath);
	curAnime->setLoop(true);
	speed = ftVec2(0.0, 0.0);
	curAnime->play();
}

void MainCharactor::setCurAnime(ftAnime::FrameAnime *ca)
{
	curAnime = ca;
}

void MainCharactor::attack()
{
	Bullet b = Bullet();
	b.setPosition(getPosition());
	b.setSpeed(ftVec2(0.0, 2000.0));
	bulletCon.add(b);
}

void MainCharactor::setSpeed(ftVec2 v)
{
	speed = v;
}

void MainCharactor::update()
{

	float deltaT = fountain::getCurSceneDeltaT();
	move(speed * deltaT);
	bulletCon.update();
	curAnime->update();
}

void MainCharactor::draw()
{
	bulletCon.draw();
	ftVec2 pos = getPosition();
	ftRender::transformBegin(pos.x, pos.y);
	ftRender::useColor(FT_White);
	curAnime->draw();
	ftRender::transformEnd();
}
