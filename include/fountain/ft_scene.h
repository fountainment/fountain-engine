#ifndef _FT_SCENE_H_
#define _FT_SCENE_H_

#define FT_SceneMax 32

#include <fountain/ft_data.h>
#include <fountain/ft_time.h>

namespace ftScene {

void init();

class Scene {
private:
	ftTime::Clock mainClock;
	void (*initPtr)(Scene *sc);
	void (*updatePtr)(Scene *sc);
	void (*drawPtr)(Scene *sc);
	void (*destroyPtr)(Scene *sc);

public:
	bool pause;
	bool end;
	bool isInit;
	bool needDestroy;
	int next;
	int prev;
	Scene(void (*init)(Scene *sc), void (*update)(Scene *sc), void (*draw)(Scene *sc), void (*destroy)(Scene *sc));
	void init();
	void update();
	void draw();
	void destroy();
	void Pause();
	void Continue();
	void gotoScene(int next, int animeSceneIndex = FT_None, bool destroyCurScene = true);
};

class SceneSelector {
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
};

};

#endif
