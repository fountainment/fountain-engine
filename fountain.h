#ifndef _FOUNTAIN_H_
#define _FOUNTAIN_H_
#include <string>
#include "ft_data.h"
#include "ft_time.h"
#include "ft_3dmodel.h"
#include "ft_render.h"
#include "ft_input.h"

namespace fountain
{
	void checkSysState()
	{

	}

	void readSettingFile()
	{

	}

	void setBasicVarible()
	{
		mainWin.w = 800;
		mainWin.h = 600;
		mainWin.w4 = mainWin.w / 4;
		mainWin.h4 = mainWin.h / 4;
		mainWin.title = "fountain-prototype 0.03";
	}

	void initAllSystem()
	{
		ftTime::init();
		ft3DModel::init();
		ftRender::init();
		ftInput::init();
	}

	void init()
	{
		checkSysState();
		readSettingFile();
		setBasicVarible();
		initAllSystem();
	}

	void gameInit();

	void singleFrame();
}

#ifdef linux
#include "linFountain.h"
#endif

#ifdef _WIN32
#include "winFountain.h"
#endif

#endif
