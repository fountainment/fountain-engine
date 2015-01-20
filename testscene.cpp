#include "testscene.h"
#include <cstdio>
#include <cstring>

const char *str[] = {"图片读取渲染", "3D模型读取渲染", "物理引擎", "字体系统", "音频系统", "UI系统", "着色器", "输入检测", "时间检测"};
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
	mainCamera.update();
	butCon.update();
}

void HelloWorld::draw()
{
	butCon.draw();
}

//class TestScene
void TestScene::init()
{
	button.setPosition(ftVec2(320, 260));
	button.setRectSize(ftVec2(130, 50));
	button.setCaption("返回");
	button.id = 9;
	customInit();
	mainCamera.setViewport(fountain::getWinRect());

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
	ftRender::transformBegin();
	ftRender::ftScale(0.5f);
	ftRender::drawAlphaPic(picID);
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
void TypeScene::customInit()
{
	fontMan.loadFont("resources/font/test.ttc");
	fontMan.genAsciiTable();
}

void TypeScene::customUpdate()
{
}

void TypeScene::customDraw()
{
	ftRender::useColor(FT_White);
	fontMan.drawString("Hello World!");
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
	if (fountain::sysMouse.getState(FT_LButton)) sz += 2.0f;
	if (fountain::sysMouse.getState(FT_RButton)) sz -= 2.0f;
	nineS.setSize(ftVec2(sz, sz));
}

void UIScene::customDraw()
{
	nineS.draw();
}

//class ShaderScene
void ShaderScene::customInit()
{
	SP.load("resources/shader/vs.vert", "resources/shader/fs.frag");
	SP.init();
	SP.use();
}

void ShaderScene::customUpdate()
{
	SP.update();
}

void ShaderScene::customDraw()
{
	ftRender::useColor(FT_White);
	ftRender::drawQuad(300, 300);
}

void ShaderScene::destroy()
{
	ftRender::useFFP();
}

//class InputScene
void InputScene::customInit()
{
}

void InputScene::customUpdate()
{
}

void InputScene::customDraw()
{
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

