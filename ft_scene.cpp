#include <fountain/fountaindef.h>
#include <fountain/ft_scene.h>

using ftScene::Scene;
using ftScene::SceneSelector;

bool ftScene::init()
{
	return true;
}

void ftScene::close()
{
}

Scene::Scene(void (*init)(Scene *sc), void (*update)(Scene *sc), void (*draw)(Scene *sc), void (*uninit)(Scene *sc)) {
	initPtr = init;
	updatePtr = update;
	drawPtr = draw;
	destroyPtr = uninit;
	end = false;
	isInit = false;
	needDestroy = false;
}

void Scene::init()
{
	if (initPtr != NULL)
		initPtr(this);
	mainClock.init();
	isInit = true;
}

void Scene::update()
{
	mainClock.tick();
	if (updatePtr != NULL)
		updatePtr(this);
}

void Scene::draw()
{
	if (drawPtr != NULL)
		drawPtr(this);
}

void Scene::destroy()
{
	if (destroyPtr != NULL)
		destroyPtr(this);
	isInit = false;
}

void Scene::Pause()
{
	mainClock.Pause();
	pause = true;
}

void Scene::Continue()
{
	mainClock.Continue();
	pause = false;
}

void Scene::gotoScene(int next, int animeSceneIndex, bool destroyCurScene)
{
	end = true;
	needDestroy = destroyCurScene;
	this->next = next;
}

SceneSelector::SceneSelector()
{
	for (int i = 0; i < FT_SceneMax; i++)
		scene[i] = NULL;
	cur = FT_StartScene;
}

void SceneSelector::registerScene(Scene *sc, int index)
{
	if (index >= 0 && index < FT_SceneMax) {
		scene[index] = sc;
		if (index == FT_StartScene) sc->init();
	} else {
		//TODO:add debug info output
	}
}

void SceneSelector::update()
{
	Scene *sc = scene[cur];
	mainClock.tick();
	if (sc != NULL) sc->update();
	else {
		//TODO:add debug info output
	}
}

void SceneSelector::draw()
{
	Scene *sc = scene[cur];
	if (sc != NULL) sc->draw();
	else {
		//TODO:add debug info output
	}
}

void SceneSelector::sceneSolve()
{
	Scene *sc = scene[cur];
	if (sc != NULL && sc->end) {
		if (sc->needDestroy)
			sc->destroy();
		cur = sc->next;
		sc->end = false;
		sc = scene[cur];
		if (!sc->isInit) sc->init();
	}
}

