#include "editor.h"

void AnimeSprite::setAnime(ftAnime::Anime *ani)
{
	ap = ani;
}

void AnimeSprite::update()
{
	ap->update();
}

void AnimeSprite::draw()
{
	ftVec2 pos = getPosition();
	ftRender::transformBegin(pos.x, pos.y);
	ap->draw();
	ftRender::transformEnd();
}

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
		anime[i].setMasterClock(&mainClock);
		anime[i].play();
		animeBackUp[i] = anime[i];
		animeBackUp[i].setLoop(true);
		animeS[i].setAnime(&animeBackUp[i]);
		animeS[i].setRectSize(100, 100);
		mainAFSM.setMasterClock(&mainClock);
		mainAFSM.regAnime(i, &anime[i]);
	}
	aniSet.close();
	ftFile loc;
	loc.open("1.loc");
	if (!loc.exist())
		for (int i = 0; i < aniN; i++)
			animeS[i].setPosition(ftAlgorithm::randRangef(-300, 300), ftAlgorithm::randRangef(-300, 300));
	else {
		int n;
		float tx, ty;
		loc.read("%d", &n);
		for (int i = 0; i < n; i++) {
			loc.read("%f%f", &tx, &ty);
			animeS[i].setPosition(tx, ty);
		}
	}
	loc.close();
	ftFile afsm;
	afsm.open("1.afsm");
	if (afsm.exist()) {
		mainAFSM.loadFSM("1.afsm");
	}
	afsm.close();
	mainAFSM.startWith(26);
	mouse = ftRender::getPicture("resources/image/mouseWhite.png");
	selectAS = -1;
	lastSelect = -1;
	pa = -1;
	pb = -1;
	signal = 0;
	showAll = false;
}

void editorScene::update()
{
	ftVec2 mp = fountain::sysMouse.getPos();
	mp = mainCamera.mouseToWorld(mp);
	if (fountain::sysMouse.getState(FT_LButton) == FT_ButtonUp) {
		selectAS = -1;
	}
	if (fountain::sysMouse.getState(FT_LButton) == FT_ButtonDown) lastSelect = -1;
	for (int i = 0; i < aniN; i++) {
		animeS[i].update();
		ftRect rct = animeS[i].getRect();
		if (rct.collidePoint(mp)) {
			if (fountain::sysMouse.getState(FT_LButton) == FT_ButtonDown) {
				selectAS = i;
				lastSelect = i;
			}
			if (fountain::sysMouse.getState(FT_RButton) == FT_ButtonDown) pa = i;
			if (fountain::sysMouse.getState(FT_RButton) == FT_ButtonUp) {
				pb = i;
				if (pa != -1 && pa != pb) {
					if (mainAFSM.getConnection(pa, signal) != pb) {
						mainAFSM.addConnection(pa, signal, pb);
					} else mainAFSM.delConnection(pa, signal);
					pa = pb = -1;
				}
			}
		}
	}
	if (selectAS >= 0 && fountain::sysMouse.getState(FT_LButton)) {
		ftVec2 dv = fountain::sysMouse.getDeltaV();
		animeS[selectAS].move(dv);
	}
	if (fountain::sysKeyboard.getState(FT_0) == FT_ButtonDown) {
		signal *= 10;
		signal += 0;
	}
	if (fountain::sysKeyboard.getState(FT_1) == FT_ButtonDown) {
		signal *= 10;
		signal += 1;
		mainAFSM.inputSignal(1, 3);
	}
	if (fountain::sysKeyboard.getState(FT_2) == FT_ButtonDown) {
		signal *= 10;
		signal += 2;
		mainAFSM.inputSignal(2, 3);
	}
	if (fountain::sysKeyboard.getState(FT_3) == FT_ButtonDown) {
		signal *= 10;
		signal += 3;
		mainAFSM.inputSignal(3, 3);
	}
	if (fountain::sysKeyboard.getState(FT_4) == FT_ButtonDown) {
		signal *= 10;
		signal += 4;
		mainAFSM.inputSignal(4, 3);
	}
	if (fountain::sysKeyboard.getState(FT_5) == FT_ButtonDown) {
		signal *= 10;
		signal += 5;
		mainAFSM.inputSignal(5, 3);
	}
	if (fountain::sysKeyboard.getState(FT_6) == FT_ButtonDown) {
		signal *= 10;
		signal += 6;
		mainAFSM.inputSignal(6, 3);
	}
	if (fountain::sysKeyboard.getState(FT_7) == FT_ButtonDown) {
		signal *= 10;
		signal += 7;
		mainAFSM.inputSignal(7, 3);
	}
	if (fountain::sysKeyboard.getState(FT_8) == FT_ButtonDown) {
		signal *= 10;
		signal += 8;
		mainAFSM.inputSignal(8, 3);
	}
	if (fountain::sysKeyboard.getState(FT_9) == FT_ButtonDown) {
		signal *= 10;
		signal += 9;
		mainAFSM.inputSignal(9, 3);
	}
	if (fountain::sysKeyboard.getState(FT_A) == FT_ButtonDown) {
		showAll = !showAll;
	}
	signal %= 100;
	mainAFSM.update();
	if (fountain::sysKeyboard.getState(FT_Ctrl) && fountain::sysKeyboard.getState(FT_S) == FT_KeyDown) {
		ftFile loc;
		loc.open("1.loc");
		loc.write("%d\n", aniN);
		for (int i = 0; i < aniN; i++) {
			ftVec2 pos = animeS[i].getPosition();
			loc.write("%f %f\n", pos.x, pos.y);
		}
		loc.close();
		mainAFSM.saveFSM("1.afsm");
	}
	if (fountain::sysMouse.getState(FT_ScrollUp)) {
		if (lastSelect != -1) {
			int lay = mainAFSM.getLayer(lastSelect);
			if (lay < 4) {
				mainAFSM.setLayer(lastSelect, lay + 1);
			}
		}
	}
	if (fountain::sysMouse.getState(FT_ScrollDown)) {
		if (lastSelect != -1) {
			int lay = mainAFSM.getLayer(lastSelect);
			if (lay > 0) {
				mainAFSM.setLayer(lastSelect, lay - 1);
			}
		}

	}
	if (fountain::sysKeyboard.getState(FT_L) == FT_ButtonDown) {
		if (lastSelect != -1) {
			bool loop = mainAFSM.isLoop(lastSelect);
			mainAFSM.setLoop(lastSelect, !loop);
		}
	}
}

void editorScene::draw()
{
	mainCamera.update();
	for (int i = 0; i < aniN; i++) {
		animeS[i].draw();
		if (lastSelect == i) {
			ftVec2 pos = animeS[i].getPosition();
			ftRender::transformBegin(pos.x, pos.y);
			ftRender::drawCircleEdge(40);
			ftRender::transformBegin(0, -50);
			if (anime[i].isLoop()) {
				ftRender::useColor(FT_Red);
			}
			ftRender::drawCircle(10);
			ftRender::useColor(FT_White);
			ftRender::transformEnd();
			ftRender::transformBegin(-8, -80);
			ftUI::Label l;
			l.setPosition(ftVec2(0.0, 0.0));
			char ts[10];
			int lay = mainAFSM.getLayer(i);
			std::sprintf(ts, "%d", lay);
			l.setString(ts);
			l.draw();
			ftRender::transformEnd();
			ftRender::transformEnd();
		}
	}
	mainAFSM.draw();
	for (int i = 0; i < aniN; i++) {
		for (int j = 0; j < 100; j++) {
			int tmp = mainAFSM.getConnection(i, j);
			if (tmp != -1) {
				ftVec2 st = animeS[i].getPosition();
				ftVec2 st2 = animeS[tmp].getPosition();
				ftVec2 vec = st2 - st;
				vec = vec.getVectorVertical();
				vec *= -10;
				st += vec;
				st2 += vec;
				ftVec2 center = st + st2;
				center = center / 2;
				center += vec;
				ftUI::Label tl;
				tl.setPosition(center);
				char ts[10];
				std::sprintf(ts, "%d", j);
				tl.setString(ts);
				if (showAll || j == signal) {
					ftRender::drawLineArrow(st, st2, 5);
					tl.draw();
				}
			}
		}
	}
	char signum[10];
	ftUI::Label sigL;
	sigL.setPosition(500, 300);
	std::sprintf(signum, "%d", signal);
	sigL.setString(signum);
	sigL.setColor(FT_Yellow);
	sigL.draw();
	ftVec2 mp = fountain::sysMouse.getPos();
	mp = mainCamera.mouseToWorld(mp);
	ftRender::transformBegin(mp.x, mp.y);
	ftRender::drawAlphaPic(mouse);
	ftRender::transformEnd();
}
