const char *str[] = {"图片读取渲染", "3D模型读取渲染", "物理引擎", "字体系统", "音频系统", "UI系统", "着色器", "输入检测", "时间检测"};
const char *strEn[] = {"Texture", "3DModel", "Physics", "Type", "Audio", "UI", "Shader", "Input", "Time"};

class emptyScene : public ftScene::Scene
{

};

class SButton : public ftUI::Button
{
public:
	int id;

	void click()
	{
		std::printf("Button %d click!\n", id);
		switch (id) {
		case 0:
			fountain::sceneSelector.gotoScene(new emptyScene());
			break;
		case 1:
			fountain::sceneSelector.gotoScene(new emptyScene());
			break;
		case 2:
			fountain::sceneSelector.gotoScene(new emptyScene());
			break;
		case 3:
			fountain::sceneSelector.gotoScene(new emptyScene());
			break;
		case 4:
			fountain::sceneSelector.gotoScene(new emptyScene());
			break;
		case 5:
			fountain::sceneSelector.gotoScene(new emptyScene());
			break;
		case 6:
			fountain::sceneSelector.gotoScene(new emptyScene());
			break;
		case 7:
			fountain::sceneSelector.gotoScene(new emptyScene());
			break;
		case 8:
			fountain::sceneSelector.gotoScene(new emptyScene());
			break;
		};
	}

        void update()
        {
                ftUI::Button::update();
                int st = getState();
                if (st == FT_isOn) {
                        setForeColor(FT_White);
                        setBackColor(FT_Orange);
                } else if (st == FT_isDown || st == FT_ButtonDown || st == FT_ButtonUp) {
			setForeColor(FT_Yellow);
                        setBackColor(FT_Orange);
                } else {
			setForeColor(ftColor("#333"));
                        setBackColor(ftColor("#CCC"));
		}
        }
};

class HelloWorld : public ftScene::Scene
{
private:
	ftType::FontMan fm;
	container<SButton, 10> butCon;

public:
	void init()
	{
		ftFile lData("resources/data/letters.dat");
		fm.loadFont("resources/font/test.ttc");
		fm.genStringTable(lData.getStr(), 32);
		ftUI::setDefaultFont(&fm);
		for (int i = 0; i < 9; i++) {
			SButton t;
			t.setPosition(ftVec2(0, 240 - i * 60));
			t.setRectSize(ftVec2(300, 40));
			t.setCaption(str[i]);
			t.id = i;
			butCon.add(t);
		}
		mainCamera.setViewport(fountain::getWinRect());
	}

	void update()
	{
		mainCamera.update();
		butCon.update();
	}

	void draw()
	{
		butCon.draw();
	}
};
