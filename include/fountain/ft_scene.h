#ifndef _FT_SCENE_H_
#define _FT_SCENE_H_

#include <fountain/ft_data.h>
#include <fountain/ft_time.h>
#include <fountain/ft_render.h>

namespace ftScene {

bool init();
void close();

//TODO: delete function pointer, use virtual function
class Scene
{
public:
	ftTime::Clock mainClock;
	ftRender::Camera mainCamera;
	bool isPause;
	Scene();
	virtual ~Scene();
	void baseInit();
	virtual void init();
	void baseUpdate();
	virtual void update();
	virtual void draw();
	virtual void destroy();
	void pause();
	void resume();
};

class SceneSelector
{
private:
	Scene *curScene;
	Scene *oldScene;
	bool destroyOldScene;
	void update();
	void draw();
public:
	ftTime::Clock mainClock;
	SceneSelector(ftTime::Clock *masterClock);
	void doAll();
	void gotoScene(Scene *nextScene, int animeSceneIndex = FT_None, bool destroyCurScene = true);
};

}

namespace fountain {
extern ftScene::SceneSelector sceneSelector;
}

#endif
