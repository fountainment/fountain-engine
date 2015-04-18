#include <fountain/fountaindef.h>
#include "STG.h"

extern const char *str[];
extern const char *strEn[];

class SButton : public ftUI::Button
{
public:
	SButton();
	int id;
	void update();
	void setId(int idd);
};

class HWButton : public SButton
{
public:
	HWButton();
	void click();
};

class MyShaderProgram : public ftRender::ShaderProgram
{
public:
	void update();
};

class HelloWorld : public ftScene::Scene
{
private:
	container<HWButton, 20> butCon;

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
	float rx, ry;
	ft3DModel::ObjModel x;
	ftRender::Camera modelCamera;
	MyShaderProgram lightSP;
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
	ftPhysics::Body tmp[101];
	int tmpn;
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

class AnimeScene : public TestScene
{
private:
	ftRender::SubImagePool animeTest;
	ftAnime::FrameAnime anime;
public:
	void customInit();
	void customUpdate();
	void customDraw();
};

class ShaderScene : public TestScene
{
private:
	int shaderNumber;
	MyShaderProgram sp[7];
	SButton b[7];
	int use;
	float scale;
	ftVec2 observePos;
	ft3DModel::ObjModel teaPot;
	float rx, ry;
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
	char mss[20];
	ftUI::Button fps;
	ftUI::Button mspf;
public:
	void customInit();
	void customUpdate();
	void customDraw();
};

class FragmentScene : public TestScene
{
private:
	STG::MainCharactor nt;
	STG::BulletCon enemyBulletCon;
public:
	void customInit();
	void customUpdate();
	void customDraw();
};
