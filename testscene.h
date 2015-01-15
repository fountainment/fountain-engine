class HelloWorld : public ftScene::Scene
{
private:
	float x;
	ftType::FontMan fm;
	container<ftUI::Button, 10> butCon;
public:
	void init()
	{
		x = 0;
		ftFile lData("resources/data/letters.dat");
		fm.loadFont("resources/font/test.ttc");
		fm.genStringTable(lData.getStr(), 32);
		ftUI::setDefaultFont(&fm);
		ftUI::Button t;
		t.setPosition(ftVec2(0, 0));
		t.setRectSize(ftVec2(100, 100));
		t.setCaption("你好");
		butCon.add(t);
		mainCamera.setViewport(fountain::getWinRect());
	}

	void update()
	{
		mainCamera.update();
		x += 0.1;
		if (fountain::sysKeyboard.getState(FT_F) == FT_KeyDown) {
			fountain::sceneSelector.gotoScene(new HelloWorld());
		}
		butCon.update();
	}

	void draw()
	{
		butCon.draw();
	}
};
