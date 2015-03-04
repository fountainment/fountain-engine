#include "testscene.h"
#include <cstdio>
#include <cstring>

const char *str[] = {"图片读取渲染", "3D模型读取渲染", "物理引擎调用", "字体渲染", "音频播放", "UI组件", "着色器渲染", "输入模块示例", "时间模块示例"};
const char *strEn[] = {"Texture", "3DModel", "Physics", "Type", "Audio", "UI", "Shader", "Input", "Time"};

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

//class HWButton
HWButton::HWButton()
{
	SButton();
}

void HWButton::click()
{
	std::printf("Button %d click!\n", id);
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
		fountain::sceneSelector.gotoScene(new UIScene());
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
	};
}

//class HelloWorld
void HelloWorld::init()
{
	for (int i = 0; i < 9; i++) {
		HWButton t;
		t.setPosition(ftVec2(0, 240 - i * 60));
		t.setRectSize(ftVec2(300, 50));
		t.setCaption(str[i]);
		t.id = i;
		butCon.add(t);
	}
	mainCamera.setViewport(fountain::getWinRect());
}

void HelloWorld::update()
{
	butCon.update();
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
	button.setCaption("返回");
	button.id = 9;
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
	animeTest = ftRender::SubImagePool("resources/image/PlaceWeapon.png", "resources/image/PlaceWeapon.sip");
	anime = ftAnime::FrameAnime(animeTest, 15.0f);

}

void TextureScene::customUpdate()
{
	if (fountain::sysKeyboard.getState(FT_Space) == FT_KeyUp) anime.play();
	anime.update();
}

void TextureScene::customDraw()
{
	ftRender::useColor(FT_White);
	ftRender::transformBegin(-100, 0);
	ftRender::ftScale(0.5f);
	ftRender::drawAlphaPic(picID);
	ftRender::transformEnd();
	ftRender::transformBegin(100, 0);
	ftRender::ftScale(2.0f);
	anime.draw();
	ftRender::transformEnd();
}

//class ModelScene
void ModelScene::customInit()
{
	modelCamera = ftRender::Camera(0, 0, 1000);
	modelCamera.setProjectionType(FT_PERSPECTIVE);
	x.loadObj("resources/model/first.obj");
	y = 0;
}

void ModelScene::customUpdate()
{
	y += mainClock.getDeltaT() * 90.0f;
}

void ModelScene::customDraw()
{
	modelCamera.update();
	ftRender::transformBegin();
	ftRender::ftScale(130.0f);
	ftRender::ftRotate(0, y, 0);
	x.render();
	ftRender::transformEnd();
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
	debugDraw.setCaption("调试绘图: 关");
	ddFlag = false;
	world.setDebugDraw(ddFlag);
}

void PhysicsScene::customUpdate()
{
	world.update(mainClock.getDeltaT());
	debugDraw.update();
	if (debugDraw.getState() == FT_ButtonDown) {
		ddFlag = !ddFlag;
		if (ddFlag) {
			world.setDebugDraw(true);
			debugDraw.setCaption("调试绘图: 开");
		} else {
			world.setDebugDraw(false);
			debugDraw.setCaption("调试绘图: 关");
		}
	}
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
	play.setCaption("播放");
	buttonCon.add(&play);

	stop.setRectSize(ftVec2(100, 100));
	stop.setPosition(70, 70);
	stop.setCaption("停止");
	buttonCon.add(&stop);

	loop.setRectSize(ftVec2(240, 100));
	loop.setPosition(0, -60);
	loop.setCaption("循环: 开");
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
			loop.setCaption("循环: 开");
		}
		else {
			loop.setCaption("循环: 关");
		}
	}
}

void AudioScene::customDraw()
{
	buttonCon.draw();
}

//class UIScene
void UIScene::customInit()
{
	sz = 100.0f;
	nineS = ftUI::NineSprite("resources/image/nine.png");
	nineS.setSize(ftVec2(sz, sz));
}

void UIScene::customUpdate()
{
	if (fountain::sysMouse.getState(FT_LButton)) sz += 120.0f * mainClock.getDeltaT();
	if (fountain::sysMouse.getState(FT_RButton)) sz -= 120.0f * mainClock.getDeltaT();
	nineS.setSize(ftVec2(sz, sz));
}

void UIScene::customDraw()
{
	nineS.draw();
}

//class ShaderScene
void ShaderScene::customInit()
{
	scale = 150.0f;
	observePos = ftVec2(0.0, 0.0);
	shaderNumber = 6;
	sp[0].load("resources/shader/vs.vert", "resources/shader/magicStar.frag");
	sp[1].load("resources/shader/vs.vert", "resources/shader/wave.frag");
	sp[2].load("resources/shader/vs.vert", "resources/shader/purple.frag");
	sp[3].load("resources/shader/vs.vert", "resources/shader/blur.frag");
	sp[4].load("resources/shader/vs.vert", "resources/shader/normalmap.frag");
	sp[5].load("resources/shader/vs.vert", "resources/shader/mandelbrot.frag");
	for (int i = 0; i < shaderNumber ;i++) sp[i].init();
	use = 0;
	for (int i = 0; i < shaderNumber ;i++) b[i].setRectSize(ftVec2(180, 50));
	b[0].setPosition(-300, -200);
	b[0].setCaption("magicStar");
	b[1].setPosition(-100, -200);
	b[1].setCaption("wave");
	b[2].setPosition(100, -200);
	b[2].setCaption("purple");
	b[3].setPosition(300, -200);
	b[3].setCaption("blur");
	b[4].setPosition(-300, -260);
	b[4].setCaption("normal");
	b[5].setPosition(-100, -260);
	b[5].setCaption("mandelbrot");
}

void ShaderScene::customUpdate()
{
	if (use == 5) {
		if (fountain::sysMouse.getState(FT_ScrollUp)) scale *= 0.8;
		if (fountain::sysMouse.getState(FT_ScrollDown)) scale *= 1.2;
		if (fountain::sysMouse.getState(FT_LButton) == FT_isDown) {
			ftVec2 v = fountain::sysMouse.getDeltaV();
			v = v / scale;
			observePos += v;
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
		sp[4].setTexture("nmTex", ftRender::getPicture("resources/image/banner-nmm.png"), 1);
		ftRender::useColor(FT_White);
		ftRender::transformBegin();
		ftRender::drawAlphaPic(ftRender::getPicture("resources/image/banner.png"));
		ftRender::transformEnd();
	} else if (use == 5) {
		sp[5].setUniform("scale", scale);
		sp[5].setUniform("observePos", observePos);
		ftRender::drawQuad(800, 400);
	} else {
		ftRender::drawQuad(300, 300);
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
	info.setCaption("按一下空格试试");
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
	std::sprintf(s, "fps: %.1f", t);
	fps.setCaption(s);
	fps.setForeColor(FT_White);
}

void TimeScene::customUpdate()
{
	float t = ftTime::getFps();
	std::sprintf(s, "fps: %.1f", t);
	fps.setCaption(s);
}

void TimeScene::customDraw()
{
	fps.draw();
}

