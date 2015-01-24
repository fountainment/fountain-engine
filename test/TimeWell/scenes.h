#include <fountain/fountaindef.h>

extern ftRender::ShaderProgram bg;

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

class MC : public ftSprite
{
public:
	ftRender::SubImage image;
	void draw();
};

class FX : public ftSprite
{
public:
	ftColor foreColor;
	ftShape shape;
	void draw();
};

class GameScene : public BaseScene
{
private:
	MC mc;
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
