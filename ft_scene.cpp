#include <fountain/fountaindef.h>
#include <fountain/ft_scene.h>

using ftScene::Scene;
using ftScene::SceneSelector;

void ftScene::init()
{
}

Scene::Scene(void (*init)(Scene *sc), void (*update)(Scene *sc), void (*draw)(Scene *sc), void (*uninit)(Scene *sc)) {
	Scene::initPtr = init;
	Scene::updatePtr = update;
	Scene::drawPtr = draw;
	Scene::destroyPtr = uninit;
	Scene::end = false;
	Scene::isInit = false;
	Scene::needDestroy = false;
}

void Scene::init()
{
	if (Scene::initPtr != NULL)
		initPtr(this);
	Scene::mainClock.init();
	Scene::isInit = true;
}

void Scene::update()
{
	if (Scene::updatePtr != NULL)
		updatePtr(this);
}

void Scene::draw()
{
	if (Scene::drawPtr != NULL)
		drawPtr(this);
}

void Scene::destroy()
{
	if (Scene::destroyPtr != NULL)
		destroyPtr(this);
	Scene::isInit = false;
}

void Scene::Pause()
{
	Scene::mainClock.Pause();
	Scene::pause = true;
}

void Scene::Continue()
{
	Scene::mainClock.Continue();
	Scene::pause = false;
}

void Scene::gotoScene(int next, int animeSceneIndex, bool destroyCurScene)
{
	Scene::end = true;
	Scene::needDestroy = destroyCurScene;
	Scene::next = next;
}

SceneSelector::SceneSelector()
{
	for (int i = 0; i < FT_SceneMax; i++)
		SceneSelector::scene[i] = NULL;
	SceneSelector::cur = FT_StartScene;
}

void SceneSelector::registerScene(Scene *sc, int index)
{
	if (index >= 0 && index < FT_SceneMax) {
		SceneSelector::scene[index] = sc;
		if (index == FT_StartScene) sc->init();
	} else {
		//TODO:add debug info output
	}
}

void SceneSelector::update()
{
	Scene *sc = SceneSelector::scene[SceneSelector::cur];
	SceneSelector::mainClock.tick();
	if (sc != NULL) sc->update();
	else {
		//TODO:add debug info output
	}
}

void SceneSelector::draw()
{
	Scene *sc = SceneSelector::scene[SceneSelector::cur];
	if (sc != NULL) sc->draw();
	else {
		//TODO:add debug info output
	}
}

void SceneSelector::sceneSolve()
{
	Scene *sc = SceneSelector::scene[SceneSelector::cur];
	if (sc->end) {
		if (sc->needDestroy)
			sc->destroy();
		SceneSelector::cur = sc->next;
		sc->end = false;
		sc = SceneSelector::scene[SceneSelector::cur];
		if (!sc->isInit) sc->init();
	}
}

