#ifndef _FT_SCENE_H_
#define _FT_SCENE_H_

#define FT_SCENE_MAX 32

#include <fountain/ft_data.h>
#include <fountain/ft_time.h>

namespace ftScene {

void init();

class Scene {
private:
	ftTime::Clock mainClock;
	bool end;
	int next;
	int prev;
	void (*initPtr)(Scene *sc);
	void (*updatePtr)(Scene *sc);
	void (*drawPtr)(Scene *sc);
	void (*uninitPtr)(Scene *sc);
public:
	Scene(void (*init)(Scene *sc), void (*update)(Scene *sc), void (*draw)(Scene *sc), void (*uninit)(Scene *sc));
	void init();
	void update();
	void draw();
	void uninit();
	void Pause();
	void Continue();
	void gotoScene(int next, int animeSceneIndex);

};

class SceneSelector {
private:
	int cur;
	Scene *sn[FT_SCENE_MAX];
	void gotoScene(int next, int animeSceneIndex);
public:
	sceneSelector();
	void registerScene(Scene *sc, int index);
	void update();
	void draw();
	void sceneSolve();
};

};

#endif
