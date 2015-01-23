#include <fountain/fountaindef.h>

extern ftRender::ShaderProgram bg;

class OpenScene : public ftScene::Scene
{
private:
public:
	void init();
	void update();
	void draw();
};

class GameScene : public ftScene::Scene
{
private:
public:
	void init();
	void update();
	void draw();
};

class ScoreScene : public ftScene::Scene
{
private:
public:
	void init();
	void update();
	void draw();
};
