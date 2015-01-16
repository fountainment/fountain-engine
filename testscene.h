#include <fountain/fountaindef.h>

extern const char *str[];
extern const char *strEn[];

class SButton : public ftUI::Button
{
public:
	SButton();
	int id;
	void update();
};

class HWButton : public SButton
{
public:
	HWButton();
	void click();
};

class HelloWorld : public ftScene::Scene
{
private:
	container<HWButton, 10> butCon;

public:
	void init();
	void update();
	void draw();
};

class TestScene : public ftScene::Scene
{
private:
	HWButton button;
public:
	void init();
	void update();
	void draw();
	virtual void customInit();
	virtual void customUpdate();
	virtual void customDraw();
};

class TextureScene : public TestScene
{
private:
	int picID;
public:
	void customInit();
	void customUpdate();
	void customDraw();
};

class ModelScene : public TestScene
{
private:
	float y;
	ft3DModel::ObjModel x;
	ftRender::Camera modelCamera;
public:
	void customInit();
	void customUpdate();
	void customDraw();
};

class PhysicsScene : public TestScene
{
private:
	ftPhysics::World world;
	ftPhysics::Body a, b, c;
public:
	void customInit();
	void customUpdate();
	void customDraw();
};

class TypeScene : public TestScene
{
private:
	ftType::FontMan fontMan;
public:
	void customInit();
	void customUpdate();
	void customDraw();
};

class AudioScene : public TestScene
{
private:
	SButton x;
	ftAudio::Channel ch;
public:
	void customInit();
	void customUpdate();
	void customDraw();
};

class UIScene : public TestScene
{
private:
	float sz;
	ftUI::NineSprite nineS;
public:
	void customInit();
	void customUpdate();
	void customDraw();
};

class ShaderScene : public TestScene
{
private:
	ftRender::ShaderProgram SP;
public:
	void customInit();
	void customUpdate();
	void customDraw();
	void destroy();
};
