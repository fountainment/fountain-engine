#include "editor.h"

void editorScene::init()
{
        ftFile aniSet;
        char pngName[100], sipName[100];
        aniSet.open("resources/des.txt");
        aniSet.read("%d", &aniN);
        for (int i = 0; i < aniN; i++) {
                aniSet.read("%s%s", pngName, sipName);
                ftRender::SubImagePool sip = ftRender::SubImagePool(pngName, sipName);
                anime[i] = ftAnime::FrameAnime(sip, 15.0f);
                anime[i].play();
                mainAFSM.setMasterClock(&mainClock);
                mainAFSM.regAnime(i, &anime[i]);
        }
        aniSet.close();
        mainAFSM.startWith(0);
        mouse = ftRender::getPicture("resources/image/mouseWhite.png");
        sc = 1.0f;
}

void editorScene::update()
{
	for (int i = 0; i < aniN; i++) {
		anime[i].update();
	}
	mainAFSM.update();
	if (fountain::sysKeyboard.getState(FT_Ctrl) && fountain::sysKeyboard.getState(FT_S) == FT_KeyDown)
                mainAFSM.saveFSM("1.afsm");
	if (fountain::sysMouse.getState(FT_ScrollUp)) sc *= 1.1f;
	if (fountain::sysMouse.getState(FT_ScrollDown)) sc *= 0.9f;
	mainCamera.setScale(sc);
}

void editorScene::draw()
{
	mainCamera.update();
	for (int i = 0; i < aniN; i++) {
		anime[i].draw();
	}
        mainAFSM.draw();
        ftVec2 mp = fountain::sysMouse.getPos();
        mp = mainCamera.mouseToWorld(mp);
        ftRender::transformBegin(mp.x, mp.y);
        ftRender::drawAlphaPic(mouse);
        ftRender::transformEnd();
}
