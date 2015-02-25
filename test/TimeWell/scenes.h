#include <fountain/fountaindef.h>

extern ftRender::ShaderProgram bg;

extern ftType::FontMan fm;
extern ftType::FontMan lf;

class BaseScene : public ftScene::Scene
{
private:
	int cursorID;
public:
	ftRender::Camera screenC;
	void init();
	void update();
	void draw();
	void drawBG();
	void drawCursor();
	virtual void otherInit();
	virtual void otherDraw();
	virtual void otherUpdate();
};

class OSButton : public ftUI::Button
{
public:
	ftColor bcolor;
	void update();
};

class OpenScene : public BaseScene
{
private:
	OSButton startB;
	ftUI::Button title;
	ftUI::Button intro;
	ftUI::Button intro1;
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
	int score;
	void update();
	void draw();
	void drawAt(float x, float y);
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
	b2Body *body;
	ftColor bcolor;
	float r;
	int en;
	RP tmp;
	ftShape shape;
	ftVec2 speed;
	float aSpeed;
	void draw();
	void update();
};

class BH : public ftSprite
{
public:
	static BH create();
	void draw();
	void update();
	int en;
	b2Body *hole;
	ftRender::SubImage image;
	ftShape shape;
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
	int state;
	b2World *world;
	MC mc;
	ocContainer ocPool;
	CL cListener;
	BH bh;
	ftUI::Label scoreB;
	ftUI::Label timeB;
	ftUI::Button startB;
	ftUI::Button title;
	OSButton repl;
public:
	void otherInit();
	void otherUpdate();
	void otherDraw();
	void destroy();
};

class ScoreScene : public BaseScene
{
private:
public:
	void otherInit();
	void otherUpdate();
	void otherDraw();
};
