#include "testscene.h"
#include <cstdio>
#include <cstring>

#define CN 0
#define EN 1

const char *str[] = {"图片读取渲染",
                     "3D模型读取渲染",
                     "物理引擎调用",
                     "字体渲染",
                     "音频播放",
                     "动画演示",
                     "着色器渲染",
                     "输入模块示例",
                     "时间模块示例",
                     "返回",
                     "测试",
                     "En",
                     "调试绘图: 关",
                     "调试绘图: 开",
                     "播放",
                     "停止",
                     "循环: 关",
                     "循环: 开",
                     "按一下空格试试"
                    };
const char *strEn[] = {"Texture",
                       "3DModel",
                       "Physics",
                       "Font",
                       "Audio",
                       "Anime",
                       "Shader",
                       "Input",
                       "Time",
                       "Back",
                       "Demo",
                       "中文",
                       "DebugDraw: Off",
                       "DebugDraw: On",
                       "Play",
                       "Stop",
                       "Repeat: Off",
                       "Repeat: On",
                       "Press Space"
                      };
const char **useStr[] = {str, strEn};
int language = CN;

//class SButton
SButton::SButton()
{
	ftUI::Button();
	setForeColor(FT_White);
	setBackColor(FT_Black);
}

void SButton::update()
{
	ftUI::Button::update();
	int st = getState();
	if (st == FT_isOn) {
		setForeColor(FT_White);
		setBackColor(ftColor("#0099CC"));
	} else if (st == FT_isDown || st == FT_ButtonDown || st == FT_ButtonUp) {
		setForeColor(FT_Yellow);
		setBackColor(ftColor("#0099CC"));
	} else {
		setForeColor(FT_White);
		setBackColor(FT_Black);
	}
}

void SButton::setId(int idd)
{
	id = idd;
	setCaption(useStr[language][id]);
}

//class HWButton
HWButton::HWButton()
{
	SButton();
}

void updateCaption(HWButton & but)
{
	but.setId(but.id);
}

void HWButton::click()
{
	FT_OUT("Button %d click!\n", id);
	switch (id) {
	case 0:
		fountain::sceneSelector.gotoScene(new TextureScene());
		break;
	case 1:
		fountain::sceneSelector.gotoScene(new ModelScene());
		break;
	case 2:
		fountain::sceneSelector.gotoScene(new PhysicsScene());
		break;
	case 3:
		fountain::sceneSelector.gotoScene(new TypeScene());
		break;
	case 4:
		fountain::sceneSelector.gotoScene(new AudioScene());
		break;
	case 5:
		fountain::sceneSelector.gotoScene(new AnimeScene());
		break;
	case 6:
		fountain::sceneSelector.gotoScene(new ShaderScene());
		break;
	case 7:
		fountain::sceneSelector.gotoScene(new InputScene());
		break;
	case 8:
		fountain::sceneSelector.gotoScene(new TimeScene());
		break;
	case 9:
		fountain::sceneSelector.gotoScene(new HelloWorld());
		break;
	/*
	case 10:
		fountain::sceneSelector.gotoScene(new FragmentScene());
		break;
	*/
	case 11:
		if (language == CN) {
			language = EN;
		}
		else {
			language = CN;
		}
		break;
	};
}

//class MyShaderProgram
void MyShaderProgram::update()
{
	ftVec2 mp = fountain::sysMouse.getPos();
	this->setUniform("time", fountain::mainClock.getTotalT());
	this->setUniform("resolution", fountain::getWinSize());
	this->setUniform("mouse", mp);
}

//class HelloWorld
void HelloWorld::init()
{
	HWButton t;
	for (int i = 0; i < 9; i++) {
		t.setPosition(ftVec2(0, 240 - i * 60));
		t.setRectSize(ftVec2(300, 50));
		t.setId(i);
		butCon.add(t);
	}
	/*
	t.setPosition(ftVec2(280, 180));
	t.setRectSize(ftVec2(200, 200));
	t.setId(10);
	butCon.add(t);
	*/
	t.setPosition(ftVec2(-320, -220));
	t.setRectSize(ftVec2(100, 100));
	t.setId(11);
	butCon.add(t);
	mainCamera.setViewport(fountain::getWinRect());
}

void HelloWorld::update()
{
	butCon.update();
	butCon.doWith(updateCaption);
}

void HelloWorld::draw()
{
	mainCamera.update();
	butCon.draw();
}

//class TestScene
void TestScene::init()
{
	button.setPosition(ftVec2(320, 260));
	button.setRectSize(ftVec2(130, 50));
	button.setId(9);
	mainCamera.setViewport(fountain::getWinRect());
	customInit();
}

void TestScene::update()
{
	button.update();
	customUpdate();
}

void TestScene::draw()
{
	mainCamera.update();
	button.draw();
	customDraw();
}

void TestScene::customInit() {}
void TestScene::customUpdate() {}
void TestScene::customDraw() {}

//class TextureScene
void TextureScene::customInit()
{
	picID = ftRender::getPicture("resources/image/logo.png");
}

void TextureScene::customUpdate()
{
}

void TextureScene::customDraw()
{
	ftRender::useColor(FT_White);
	ftRender::ftScale(0.5f);
	ftRender::drawAlphaPic(picID);
}

//class ModelScene
void ModelScene::customInit()
{
	modelCamera = ftRender::Camera(0, 0, 1000);
	modelCamera.setProjectionType(FT_PERSPECTIVE);
	x.loadObj("resources/model/teapot.obj", true);
	lightSP.load("resources/shader/vs.vert", "resources/shader/model.frag");
	lightSP.init();
	rx = ry = 0;
}

void ModelScene::customUpdate()
{
	if (fountain::sysMouse.getState(FT_LButton) == FT_isDown) {
		ftVec2 dv = fountain::sysMouse.getDeltaV();
		ry += dv.x;
		rx -= dv.y;
	}
}

void ModelScene::customDraw()
{
	modelCamera.update();
	lightSP.use();
	ftRender::useColor(FT_White);
	ftRender::transformBegin();
	ftRender::ftScale(220.0f);
	ftRender::ftRotate(rx, ry, 0);
	x.render();
	ftRender::transformEnd();
	ftRender::useBasicShader();
}

//class PhysicsScene
void PhysicsScene::customInit()
{
	ftPhysics::setRatio(100.0f);

	ground = ftPhysics::Body(0, -100, FT_Static);
	ground.setShape(ftShape(ftRect(0, 0, 500, 20)));
	world.addBody(&ground);

	ball = ftPhysics::Body(-5, 200, FT_Dynamic);
	ball.setShape(ftShape(15));
	world.addBody(&ball);

	for (int i = 0; i < 25; i++) {
		card[i] = ftPhysics::Body(-240 + i * 20, -75, FT_Dynamic);
		card[i].setShape(ftShape(ftRect(0, 0, 3, 30)));
		world.addBody(&card[i]);
	}

	debugDraw.setRectSize(ftVec2(300, 80));
	debugDraw.setPosition(0, -180);
	debugDraw.setCaption(useStr[language][12]);
	ddFlag = false;
	world.setDebugDraw(ddFlag);
	tmpn = 0;
}

void PhysicsScene::customUpdate()
{
	if (fountain::sysMouse.getState(FT_LButton) == FT_ButtonDown) {
		ftVec2 pos = mainCamera.mouseToWorld(fountain::sysMouse.getPos());
		tmp[tmpn] = ftPhysics::Body(pos.x, pos.y, FT_Dynamic);
		tmp[tmpn].setShape(ftShape(15));
		world.addBody(&tmp[tmpn]);
		if (tmpn < 100) tmpn++;
	}
	debugDraw.update();
	if (debugDraw.getState() == FT_ButtonDown) {
		ddFlag = !ddFlag;
		if (ddFlag) {
			world.setDebugDraw(true);
			debugDraw.setCaption(useStr[language][13]);
		} else {
			world.setDebugDraw(false);
			debugDraw.setCaption(useStr[language][12]);
		}
	}
	world.update(mainClock.getDeltaT());
}

void PhysicsScene::customDraw()
{
	world.draw();
	debugDraw.draw();
}

//class TypeScene
//TODO: simplize drawString with different font size
void TypeScene::customInit()
{
	fontMan2.loadFont("resources/font/test.ttc");
	fontMan2.genStringTable("HWerlod! ", 32);
	fontMan.loadFont("resources/font/test.ttc");
	fontMan.genStringTable("HWerlod! ", 16);
	fontMan1.loadFont("resources/font/test.ttc");
	fontMan1.genStringTable("HWerlod! ", 8);
}

void TypeScene::customUpdate()
{
}

void TypeScene::customDraw()
{
	ftRender::useColor(FT_White);
	ftRender::transformBegin();
	ftRender::ftTranslate(-100, 0);
	fontMan.drawString("Hello World!");
	ftRender::transformEnd();
	ftRender::transformBegin();
	ftRender::ftTranslate(-100, -100);
	fontMan1.drawString("Hello World!");
	ftRender::transformEnd();
	ftRender::transformBegin();
	ftRender::ftTranslate(-100, 100);
	fontMan2.drawString("Hello World!");
	ftRender::transformEnd();
}

//class AudioScene
void AudioScene::customInit()
{
	play.setRectSize(ftVec2(100, 100));
	play.setPosition(-70, 70);
	play.setCaption(useStr[language][14]);
	buttonCon.add(&play);

	stop.setRectSize(ftVec2(100, 100));
	stop.setPosition(70, 70);
	stop.setCaption(useStr[language][15]);
	buttonCon.add(&stop);

	loop.setRectSize(ftVec2(240, 100));
	loop.setPosition(0, -60);
	loop.setCaption(useStr[language][17]);
	buttonCon.add(&loop);

	loopFlag = true;
	ch.init();
	ch.load("resources/sound/test.wav");
	ch.setLoop(loopFlag);
}

void AudioScene::customUpdate()
{
	buttonCon.update();
	if (play.getState() == FT_ButtonDown) {
		ch.play();
	}
	if (stop.getState() == FT_ButtonDown) {
		ch.stop();
	}
	if (loop.getState() == FT_ButtonDown) {
		loopFlag = !loopFlag;
		ch.setLoop(loopFlag);
		if (loopFlag) {
			loop.setCaption(useStr[language][17]);
		}
		else {
			loop.setCaption(useStr[language][16]);
		}
	}
}

void AudioScene::customDraw()
{
	buttonCon.draw();
}

//class AnimeScene
void AnimeScene::customInit()
{
	animeTest = ftRender::SubImagePool("resources/image/PlaceWeapon.png", "resources/image/PlaceWeapon.sip");
	anime.init(animeTest, 15.0f);
	anime.setLoop(true);
	anime.setMasterClock(&mainClock);
	anime.play();
}

void AnimeScene::customUpdate()
{
	anime.update();
}

void AnimeScene::customDraw()
{
	ftRender::useColor(FT_White);
	ftRender::transformBegin();
	ftRender::ftScale(2.0f);
	anime.draw();
	ftRender::transformEnd();
}

//class ShaderScene
void ShaderScene::customInit()
{
	const char *titleStr[][7] = {
		{"效果1", "效果2", "效果3", "高斯模糊", "法线贴图", "曼德博集", "立方贴图"},
		{"star", "wave", "smoke", "blur", "normal", "mandelbrot", "cubemap"}
	};
	teaPot.loadObj("resources/model/teapot.obj", true);
	rx = ry = 0;
	scale = 150.0f;
	observePos = ftVec2(0.0, 0.0);
	shaderNumber = 7;
	sp[0].load("resources/shader/vs.vert", "resources/shader/magicStar.frag");
	sp[1].load("resources/shader/vs.vert", "resources/shader/wave.frag");
	sp[2].load("resources/shader/vs.vert", "resources/shader/purple.frag");
	sp[3].load("resources/shader/vs.vert", "resources/shader/blur.frag");
	sp[4].load("resources/shader/vs.vert", "resources/shader/normalmap.frag");
	sp[5].load("resources/shader/vs.vert", "resources/shader/mandelbrot.frag");
	sp[6].load("resources/shader/vs.vert", "resources/shader/cubemap.frag");
	for (int i = 0; i < shaderNumber; i++) sp[i].init();
	use = 0;
	b[0].setPosition(-300, -210);
	b[1].setPosition(-100, -210);
	b[2].setPosition(100, -210);
	b[3].setPosition(300, -210);
	b[4].setPosition(-300, -270);
	b[5].setPosition(-100, -270);
	b[6].setPosition(100, -270);
	for (int i = 0; i < shaderNumber; i++) {
		b[i].setRectSize(ftVec2(190, 50));
		b[i].setCaption(titleStr[language][i]);
	}
}

void ShaderScene::customUpdate()
{
	if (use == 5) {
		if (fountain::sysMouse.getState(FT_ScrollUp)) scale *= 0.8;
		if (fountain::sysMouse.getState(FT_ScrollDown)) scale *= 1.2;
		if (fountain::sysMouse.getState(FT_RButton) == FT_isDown) {
			ftVec2 v = fountain::sysMouse.getDeltaV();
			v = v / scale;
			observePos += v;
		}
	}
	if (use == 6) {
		if (fountain::sysMouse.getState(FT_LButton) == FT_isDown) {
			ftVec2 dv = fountain::sysMouse.getDeltaV();
			ry += dv.x;
			rx -= dv.y;
		}
	}
	for (int i = 0; i < shaderNumber; i++) {
		b[i].update();
		if (b[i].getState() == FT_ButtonUp) {
			use = i;
		}
	}
}

void ShaderScene::customDraw()
{
	if (use == 3) {
		ftRender::useColor(FT_White);
		ftRender::transformBegin();
		ftRender::ftTranslate(-154, 0);
		ftRender::ftScale(0.6f);
		ftRender::drawAlphaPic(ftRender::getPicture("resources/image/logo.png"));
		ftRender::transformEnd();
	}
	sp[use].use();
	ftRender::useColor(FT_White);
	if (use == 3) {
		ftRender::useColor(FT_White);
		ftRender::transformBegin();
		ftRender::ftTranslate(154, 0);
		ftRender::ftScale(0.6f);
		ftRender::drawAlphaPic(ftRender::getPicture("resources/image/logo.png"));
		ftRender::transformEnd();
	} else if (use == 4) {
		sp[4].setTexture("nmTex", ftRender::getPicture("resources/image/normalmap.png"), 1);
		ftRender::useColor(FT_White);
		ftRender::transformBegin();
		ftRender::drawAlphaPic(ftRender::getPicture("resources/image/normalmap.png"));
		ftRender::transformEnd();
	} else if (use == 5) {
		sp[5].setUniform("scale", scale);
		sp[5].setUniform("observePos", observePos);
		ftRender::drawQuad(800, 400);
	} else if (use == 6) {
		ftRender::Camera *camera = ftRender::getCurrentCamera();
		camera->setProjectionType(FT_PERSPECTIVE);
		camera->update();
		sp[6].setTexture("cubeTex", ftRender::getPicture("resources/image/cube.png", true));
		ftRender::transformBegin();
		ftRender::ftScale(220);
		ftRender::ftRotate(rx, ry, 0);
		teaPot.render();
		ftRender::transformEnd();
		camera->setProjectionType(FT_PLANE);
		camera->update();
	} else {
		ftRender::drawQuad(800, 400);
	}
	ftRender::useBasicShader();
	for (int i = 0; i < shaderNumber; i++) b[i].draw();
}

void ShaderScene::destroy()
{
	ftRender::useBasicShader();
}

//class InputScene
void InputScene::customInit()
{
	info.setCaption(useStr[language][18]);
	info.setForeColor(FT_White);
}

void InputScene::customUpdate()
{
	if (fountain::sysKeyboard.getState(FT_Space)) {
		ftRender::setClearColor(ftColor("#333"));
	} else
		ftRender::setClearColor(ftColor("#131313"));
}

void InputScene::customDraw()
{
	info.draw();
}

void InputScene::destroy()
{
	ftRender::setClearColor(ftColor("#131313"));
}

//class TimeScene
void TimeScene::customInit()
{
	float t = ftTime::getFps();
	float ms = ftTime::getMsPerFrame();
	std::sprintf(s, "fps: %.2f", t);
	std::sprintf(mss, "mspf: %.2f", ms);
	fps.setCaption(s);
	mspf.setCaption(mss);
	fps.setForeColor(FT_White);
	mspf.setForeColor(FT_White);
	mspf.move(0, -50);
}

void TimeScene::customUpdate()
{
	float t = ftTime::getFps();
	float ms = ftTime::getMsPerFrame();
	std::sprintf(s, "fps: %.3f", t);
	std::sprintf(mss, "mspf: %.3f", ms);
	fps.setCaption(s);
	mspf.setCaption(mss);
}

void TimeScene::customDraw()
{
	fps.draw();
	mspf.draw();
}

/*
//class FragmentScene
void FragmentScene::customInit()
{
	nt.init(&mainClock);
}

void FragmentScene::customUpdate()
{
	if (fountain::sysKeyboard.getState(FT_Z)) nt.attack();
	ftVec2 v(0.0, 0.0);
	if (fountain::sysKeyboard.getState(FT_Up)) v += ftVec2(0.0, 200.0);
	if (fountain::sysKeyboard.getState(FT_Down)) v += ftVec2(0.0, -200.0);
	if (fountain::sysKeyboard.getState(FT_Left)) v += ftVec2(-200.0, 0.0);
	if (fountain::sysKeyboard.getState(FT_Right)) v += ftVec2(200.0, 0.0);
	nt.setSpeed(v);
	nt.update();
	STG::Bullet tmpb;
	tmpb.setColor(ftColor(std::cos(mainClock.getTotalT()), std::sin(mainClock.getTotalT()), 0.3, 1.0));
	tmpb.setSpeed(ftVec2(std::cos(mainClock.getTotalT() * 10.0), std::sin(mainClock.getTotalT() * 10.0)) * 200.0);
	enemyBulletCon.add(tmpb);
	enemyBulletCon.update();
}

void FragmentScene::customDraw()
{
	nt.draw();
	enemyBulletCon.draw();
}
*/
