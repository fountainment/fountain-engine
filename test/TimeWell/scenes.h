#include <fountain/fountaindef.h>

extern ftRender::ShaderProgram bg;

class MC : public ftPhysics::Body
{
private:
	ftRender::SubImage im;
	float scale;
public:
	MC();
	MC(ftRender::SubImage image);
	void draw();
};

class BaseScene : public ftScene::Scene
{
private:
	int cursorID;
	ftRender::Camera screenC;
public:
	void init();
	void update();
	void draw();
	void drawBG();
	void drawCursor();
	virtual void otherInit();
	virtual void otherDraw();
	virtual void otherUpdate();
};

class OpenScene : public BaseScene
{
private:
public:
	void otherInit();
	void otherUpdate();
	void otherDraw();
};

class GameScene : public BaseScene
{
private:
	ftPhysics::World myWorld;
	MC mainC;
public:
	void otherInit();
	void otherUpdate();
	void otherDraw();
};

class ScoreScene : public BaseScene
{
private:
public:
	void otherInit();
	void otherUpdate();
	void otherDraw();
};
