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
	ftRender::useColor(color);
	ftRender::drawCircle(3.0f);
	ftRender::useColor(color);
	ftRender::drawCircleEdge(5.0f);
	ftRender::transformEnd();
}

void Bullet::setColor(ftColor c)
{
	color = c;
}

//class STG::BulletCon
bool BulletCon::willLive(Bullet & b)
{
	ftRender::Camera *cam = fountain::getCurSceneCamera();
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
void MainCharactor::init(ftTime::Clock *masterClock)
{
	animeTest = ftRender::SubImagePool("resources/image/CommonAttack.png", "resources/image/CommonAttack.sip");
	breath = ftAnime::FrameAnime(animeTest, 15.0f);
	setCurAnime(&breath);
	curAnime->setLoop(true);
	speed = ftVec2(0.0, 0.0);
	charClock = ftTime::Clock(masterClock);
	charClock.init();
	curAnime->setMasterClock(&charClock);
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
	b.setColor(ftColor("#533"));
	bulletCon.add(b);
}

void MainCharactor::setSpeed(ftVec2 v)
{
	speed = v;
}

void MainCharactor::update()
{
	charClock.tick();
	float deltaT = charClock.getDeltaT();
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
