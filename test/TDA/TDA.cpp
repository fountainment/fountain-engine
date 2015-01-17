#include <fountain.h>
#include "scenes.h"

using namespace fountain;

void fountain::setBasicVarible()
{
	mainWin.title = "The Day of Alpha";
	mainWin.setSize(800, 600);
}

void fountain::gameInit()
{
	ftRender::setClearColor(FT_Black);
	ftPhysics::setRatio(100.0f);
	sceneSelector.gotoScene(new TDAScene());
}

void fountain::singleFrame()
{
	sceneSelector.doAll();
	mainClock.tick();
}
