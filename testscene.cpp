#include "testscene.h"
#include <cstdio>

const char *str[] = {"图片读取渲染", "3D模型读取渲染", "物理引擎", "字体系统", "音频系统", "UI系统", "着色器", "输入检测", "时间检测"};
const char *strEn[] = {"Texture", "3DModel", "Physics", "Type", "Audio", "UI", "Shader", "Input", "Time"};

//class SButton
SButton::SButton()
{
	ftUI::Button();
	setForeColor(ftColor("#333"));
	setBackColor(ftColor("#CCC"));
}

void SButton::update()
{
	ftUI::Button::update();
	int st = getState();
	if (st == FT_isOn) {
		setForeColor(FT_White);
		setBackColor(FT_Orange);
	} else if (st == FT_isDown || st == FT_ButtonDown || st == FT_ButtonUp) {
		setForeColor(FT_Yellow);
		setBackColor(FT_Orange);
	} else {
		setForeColor(ftColor("#333"));
		setBackColor(ftColor("#CCC"));
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
		t.setRectSize(ftVec2(300, 40));
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
	button.setRectSize(ftVec2(120, 40));
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
	ftRender::drawAlphaPic(picID);
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
	y += 1.0f;
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
	ftPhysics::setRatio(100);

	a = ftPhysics::Body(0, -100, FT_Static);
	a.setShape(ftShape(ftRect(0, 0, 200, 20)));
	world.addBody(&a);

	b = ftPhysics::Body(0, 0, FT_Dynamic);
	b.setShape(ftShape(10));
	world.addBody(&b);

	c = ftPhysics::Body(-5, 100, FT_Dynamic);
	c.setShape(ftShape(10));
	world.addBody(&c);
}

void PhysicsScene::customUpdate()
{
	world.update();
}

void PhysicsScene::customDraw()
{
	world.draw();
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
	x.setRectSize(ftVec2(100, 100));
	x.setPosition(0, 0);
	x.setCaption("Play");
	ch.init();
	ch.load("resources/sound/test.wav");
	ch.setLoop(true);
}

void AudioScene::customUpdate()
{
	x.update();
	if (x.getState() == FT_ButtonDown) {
		ch.play();
	}
}

void AudioScene::customDraw()
{
	x.draw();
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
	ftVec2 mp = fountain::sysMouse.getPos();
	SP.setUniform("time", mainClock.secondsFromInit());
	SP.setUniform("resolution", fountain::getWinSize());
	SP.setUniform("mouse", mp);
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
}

void TimeScene::customUpdate()
{
}

void TimeScene::customDraw()
{
}

