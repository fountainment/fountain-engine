#ifndef _FT_SCENE_H_
#define _FT_SCENE_H_

#define FT_SceneMax 32

#include <fountain/ft_data.h>
#include <fountain/ft_time.h>

namespace ftScene {

bool init();
void close();

//TODO: delete function pointer, use virtual function
class Scene
{
private:
	ftTime::Clock mainClock;

public:
	bool pause;
	bool end;
	bool isInit;
	bool needDestroy;
	int next;
	int prev;
	Scene();
	virtual void init();
	virtual void update();
	virtual void draw();
	virtual void destroy();
	void Pause();
	void Continue();
	void gotoScene(int next, int animeSceneIndex = FT_None, bool destroyCurScene = true);
};

class SceneSelector
{
private:
	ftTime::Clock mainClock;
	int cur;
	Scene *scene[FT_SceneMax];
	//void gotoScene(int next, int animeSceneIndex);
public:
	SceneSelector();
	void registerScene(Scene *sc, int index);
	void update();
	void draw();
	void sceneSolve();
	void doAll();
};

}

namespace fountain {
	extern ftScene::SceneSelector sceneSelector;
}

#endif
