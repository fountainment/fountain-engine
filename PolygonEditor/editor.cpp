#include "editor.h"
#include <cstdio>
#include <cstdlib>

using namespace fim;

void polygon2File(Polygon & p, ftFile ff)
{
	ff.write("%u\n", p.size());
	for (int i = 0; i < (int)p.size(); i++) {
		ff.write("%f %f\n", p[i].x, p[i].y);
	}
}

void polygonList2File(PolygonList & p, ftFile ff)
{
	ff.write("");
	for (auto it = p.begin(); it != p.end(); ++it) {
                polygon2File(*it, ff);
	}
}

void editorScene::init()
{
	ftFile inFile;
	inFile.open("in.txt");
	int n;
	inFile.read("%d", &n);
	std::vector<ftVec2> t;
	for (int i = 0; i < n; i++) {
		double tx, ty;
		inFile.read("%lf%lf", &tx, &ty);
		t.push_back(ftVec2(tx, ty));
	}
	ori = ftShape(t, t.size(), true);
	inFile.close();
	res.clear();
	mainCamera.setScale(200);
	choose = -1;
}

void editorScene::update()
{
	int n = ori.getN();
	std::vector<ftVec2> tmp;
	ftVec2 mPos = fountain::sysMouse.getPos();
	mPos = mainCamera.mouseToWorld(mPos);
	Polygon poly;
	for (int i = 0; i < n; i++) {
		float x = ori.getData()[i * 2];
		float y = ori.getData()[i * 2 + 1];
		ftVec2 tv = mPos - ftVec2(x - 2, y - 1);

		ftRender::transformBegin(x - 2, y - 1);
		if (i == 0) ftRender::useColor(FT_Yellow);
		ftRender::drawCircle(0.025);
		ftRender::transformEnd();

                if (tv.length() < 0.1 && fountain::sysMouse.getState(FT_LButton) == FT_KeyDown) {
                        choose = i;
                }
                if (choose == i) {
			x = mPos.x + 2;
                        y = mPos.y + 1;
                        ftRender::transformBegin(x - 2, y - 1);
                        ftRender::drawCircle(0.05);
                        ftRender::transformEnd();
                }
		tmp.push_back(ftVec2(x, y));
		poly.push_back(vec2(x, y));
	}
	if (fountain::sysMouse.getState(FT_LButton) == FT_KeyUp) {
		choose = -1;
	}
	ori.setData(tmp);
	auto ans = poly.convexDecomposition();
	res.clear();
	for (auto it = ans.begin(); it != ans.end(); ++it) {
                std::vector<ftVec2> vvec;
                for (auto itt = it->begin(); itt != it->end(); ++itt) {
                        vvec.push_back(ftVec2(itt->x, itt->y));
                }
                res.push_back(ftShape(vvec, vvec.size(), true));
	}
	if (fountain::sysKeyboard.getState(FT_R) == FT_KeyDown) {
		ftFile f;
		f.open("in.txt");
		polygon2File(poly, f);
		f.close();
		ftFile ff;
		ff.open("out.txt");
		polygonList2File(ans, ff);
		ff.close();
	}
}

void editorScene::draw()
{
	mainCamera.update();
	ftRender::transformBegin(-2, -1);
	ftRender::drawShapeEdge(ori);
	ftRender::transformEnd();
	ftRender::transformBegin(1, -1);
	for (int i = 0; i < (int)res.size(); i++)
	{
		ftRender::drawShapeEdge(res[i]);
	}
	ftRender::transformEnd();
}
