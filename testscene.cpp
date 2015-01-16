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
		fountain::sceneSelector.gotoScene(new TestScene());
		break;
	case 4:
		fountain::sceneSelector.gotoScene(new TestScene());
		break;
	case 5:
		fountain::sceneSelector.gotoScene(new TestScene());
		break;
	case 6:
		fountain::sceneSelector.gotoScene(new TestScene());
		break;
	case 7:
		fountain::sceneSelector.gotoScene(new TestScene());
		break;
	case 8:
		fountain::sceneSelector.gotoScene(new TestScene());
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

