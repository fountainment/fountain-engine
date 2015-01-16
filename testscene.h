#include <fountain/fountaindef.h>

extern const char *str[];
extern const char *strEn[];

class SButton : public ftUI::Button
{
public:
	SButton();
	int id;
	void click();
	void update();
};

class HelloWorld : public ftScene::Scene
{
private:
	container<SButton, 10> butCon;

public:
	void init();
	void update();
	void draw();
};

class TestScene : public ftScene::Scene
{
private:
	SButton button;
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
public:
	void customInit();
	void customUpdate();
	void customDraw();
};
