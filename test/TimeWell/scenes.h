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

typedef struct {
	b2Body *master;
	b2Vec2 rp;
	float rd;
	float md;
} RP;

class OC : public ftSprite
{
public:
	static ftColor randColor();
	static ftShape randShape();
	b2Body *body;
	float r;
	int en;
	RP tmp;
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
	//void PreSolve(b2Contact *contact, const b2Manifold *oldManifold);
	//void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};

class ocContainer : public container<OC, 500>
{
public:
	bool willLive(OC & oc);
};

class GameScene : public BaseScene
{
private:
	b2World *world;
	MC mc;
	ocContainer ocPool;
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
