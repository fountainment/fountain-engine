#include <fountain/fountaindef.h>

namespace fountain {

void checkSysState() {

}

void readSettingFile() {

}

void defaultSetting()
{
	fountain::mainWin.setSize(500, 500);
	fountain::mainWin.title = "FountainEngine Powered";
}

void basicSetting() {
	checkSysState();
	readSettingFile();
	defaultSetting();
	setBasicVarible();
}

void initAllSystem() {
	ftDebug::init();
	ftTime::init();
	ftRender::init();
	ft3DModel::init();
	ftPhysics::init();
	ftInput::init();
	ftAlgorithm::init();
	ftScene::init();
	ftType::init();
	ftAudio::init();
	ftUI::init();
	ftAnime::init();
}

void closeAllSystem()
{
	ftAnime::close();
	ftUI::close();
	ftAudio::close();
	//FIXME: ftType close bug
	ftType::close();
	ftScene::close();
	ftAlgorithm::close();
	ftInput::close();
	ftPhysics::close();
	ft3DModel::close();
	ftRender::close();
	ftTime::close();
	ftDebug::close();
}

ftVec2 getWinSize()
{
	return ftVec2(mainWin.w, mainWin.h);
}

ftRect getWinRect()
{
	return ftRect(0, 0, mainWin.w, mainWin.h);
}

ftTime::Clock* getCurSceneClock()
{
	ftScene::Scene *sc = fountain::sceneSelector.getCurScene();
	if (sc != NULL) return &(sc->mainClock);
	else return NULL;
}

ftRender::Camera* getCurSceneCamera()
{
	ftScene::Scene *sc = fountain::sceneSelector.getCurScene();
	if (sc != NULL) return &(sc->mainCamera);
	else return NULL;
}

float getCurSceneDeltaT()
{
	ftScene::Scene *sc = fountain::sceneSelector.getCurScene();
	if (sc != NULL) return sc->mainClock.getDeltaT();
	else return 0.0f;
}

}
