#include <fountain/fountaindef.h>
#include <fountain/ft_scene.h>

using ftScene::Scene;
using ftScene::SceneSelector;

namespace fountain {
SceneSelector sceneSelector;
}

bool ftScene::init()
{
	return true;
}

void ftScene::close()
{
}

Scene::Scene()
{
	init();
}

Scene::~Scene()
{
}

void Scene::init()
{
	mainClock.init();
}

void Scene::update()
{
}

void Scene::draw()
{
}

void Scene::destroy()
{
}

void Scene::pause()
{
	mainClock.pause();
	isPause = true;
}

void Scene::resume()
{
	mainClock.resume();
	isPause = false;
}

SceneSelector::SceneSelector()
{
	curScene = NULL;
	oldScene = NULL;
	destroyOldScene = false;
}

void SceneSelector::update()
{
	mainClock.tick();
	if (curScene != NULL) curScene->update();
	else {
		//TODO:add debug info output
	}
}

void SceneSelector::draw()
{
	if (curScene != NULL) curScene->draw();
	else {
		//TODO:add debug info output
	}
}

void SceneSelector::doAll()
{
	update();
	if (destroyOldScene && oldScene != NULL) {
		delete oldScene;
		oldScene = NULL;
		destroyOldScene = false;
	}
	draw();
}

void SceneSelector::gotoScene(Scene *nextScene, int animeSceneIndex, bool destroyCurScene)
{
	if (curScene != NULL && destroyCurScene) {
		curScene->destroy();
		destroyOldScene = true;
	}
	curScene = nextScene;
	//TODO: to init or not
	if (curScene != NULL) curScene->init();
}
