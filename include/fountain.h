#ifndef _FOUNTAIN_H_
#define _FOUNTAIN_H_

#include <fountain/fountaindef.h>
#include <fountain/ft_data.h>
#include <fountain/ft_time.h>
#include <fountain/ft_3dmodel.h>
#include <fountain/ft_render.h>
#include <fountain/ft_physics.h>
#include <fountain/ft_input.h>
#include <fountain/ft_algorithm.h>
#include <fountain/ft_scene.h>
#include <fountain/ft_type.h>
#include <fountain/ft_audio.h>
#include <fountain/ft_ui.h>

namespace fountain {

void checkSysState() {

}

void readSettingFile() {

}

void setBasicVarible();

void basicSetting() {
	checkSysState();
	readSettingFile();
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
}

void gameInit();

void singleFrame();

void closeAllSystem()
{
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

#ifndef FT_Console

#ifdef linux
#include <fountain/linFountain.h>
#endif

#ifdef _WIN32
#include <fountain/winFountain.h>
#endif

#else
//console
void fountain::setBasicVarible()
{
}

void fountain::gameInit()
{
}

void fountain::singleFrame()
{
}
#endif

#endif
