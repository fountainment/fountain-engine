#ifndef _FOUNTAIN_H_
#define _FOUNTAIN_H_
#include <string>
#include "ft_time.h"
#include "ft_3dmodel.h"

typedef struct {
	int w, h;
	std::string title;
} winState;

namespace fountain
{
	winState mainWin;
	ftTime::Clock mainClock();
	
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
		mainWin.title = "fountain-prototype 0.03";
	}

	void initAllSystem()
	{
		ftTime::init();
		ft3DModel::init();
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
