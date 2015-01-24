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
	b2Body *body;
	ftRender::SubImage image;
	void update();
	void draw();
};

class OC : public ftSprite
{
public:
	static ftColor randColor();
	static ftShape randShape();
	b2Body *body;
	float r;
	int en;
	ftShape shape;
	ftVec2 speed;
	float aSpeed;
	void draw();
	void update();
};

class CL : public b2ContactListener
{
public:
	void BeginContact(b2Contact *contact);
};

class GameScene : public BaseScene
{
private:
	b2World *world;
	MC mc;
	container<OC, 1000> ocPool;
	CL cListener;
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
