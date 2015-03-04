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
	ftAnime::close();
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
}

ftVec2 getWinSize()
{
	return ftVec2(mainWin.w, mainWin.h);
}

ftRect getWinRect()
{
	return ftRect(0, 0, mainWin.w, mainWin.h);
}

}
