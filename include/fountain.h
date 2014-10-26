#ifndef _FOUNTAIN_H_
#define _FOUNTAIN_H_

#include <fountain/ft_data.h>
#include <fountain/ft_time.h>
#include <fountain/ft_3dmodel.h>
#include <fountain/ft_render.h>
#include <fountain/ft_input.h>
#include <fountain/ft_algorithm.h>

namespace fountain {
	void checkSysState() {

	} void readSettingFile() {

	}

	void setBasicVarible();

	void basicSetting() {
		checkSysState();
		readSettingFile();
		setBasicVarible();
	}

	void initAllSystem() {
		ftTime::init();
		ft3DModel::init();
		ftRender::init();
		ftInput::init();
		ftAlgorithm::init();
	}

	void gameInit();

	void singleFrame();
}

#ifdef linux
#include <fountain/linFountain.h>
#endif

#ifdef _WIN32
#include <fountain/winFountain.h>
#endif

#endif
