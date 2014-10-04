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

	void setBasicVarible();

	void basicSetting()
	{
		checkSysState();
		readSettingFile();
		setBasicVarible();
	}

	void initAllSystem()
	{
		ftTime::init();
		ft3DModel::init();
		ftRender::init();
		ftInput::init();
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
