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
	ftRender::SubImagePool animeTest;
	ftAnime::FrameAnime anime;
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
	ftPhysics::Body ground, ball;
	ftPhysics::Body card[25];
	SButton debugDraw;
	bool ddFlag;
public:
	void customInit();
	void customUpdate();
	void customDraw();
};

class TypeScene : public TestScene
{
private:
	ftType::FontMan fontMan;
	ftType::FontMan fontMan1;
	ftType::FontMan fontMan2;
public:
	void customInit();
	void customUpdate();
	void customDraw();
};

class AudioScene : public TestScene
{
private:
	SButton play, stop, loop;
	ptrContainer<SButton*, 3> buttonCon;
	ftAudio::Channel ch;
	bool loopFlag;
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

class MyShaderProgram : public ftRender::ShaderProgram
{
public:
	void update()
	{
		ftVec2 mp = fountain::sysMouse.getPos();
		this->setUniform("time", fountain::mainClock.getTotalT());
		this->setUniform("resolution", fountain::getWinSize());
		this->setUniform("mouse", mp);
	}
};

class ShaderScene : public TestScene
{
private:
	int shaderNumber;
	MyShaderProgram sp[6];
	SButton b[6];
	int use;
	float scale;
	ftVec2 observePos;
public:
	void customInit();
	void customUpdate();
	void customDraw();
	void destroy();
};

class InputScene : public TestScene
{
private:
	ftUI::Button info;
public:
	void customInit();
	void customUpdate();
	void customDraw();
	void destroy();
};

class TimeScene : public TestScene
{
private:
	char s[20];
	ftUI::Button fps;
public:
	void customInit();
	void customUpdate();
	void customDraw();
};
