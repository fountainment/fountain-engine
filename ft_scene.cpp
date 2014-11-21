#include <fountain/ft_scene.h>

using ftScene::Scene, ftScene::SceneSelector;

void ftScene::init()
{
}

Scene::Scene(void (*init)(Scene *sc), void (*update)(Scene *sc), void (*draw)(Scene *sc), void (*uninit)(Scene *sc)) {
	Scene::initPtr = init;
	Scene::updatePtr = update;
	Scene::drawPtr = draw;
	Sceme::uninitPtr = uninit;
	Scene::end = false;
	Scene::pause = false;
}


