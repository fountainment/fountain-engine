#include <fountain/fountaindef.h>

extern const char *str[];
extern const char *strEn[];

class emptyScene : public ftScene::Scene
{

};

class SButton : public ftUI::Button
{
public:
	SButton();
	int id;
	void click();
	void update();
};

class HelloWorld : public ftScene::Scene
{
private:
	container<SButton, 10> butCon;

public:
	void init();
	void update();
	void draw();
};

class TextureScene : public ftScene::Scene
{
private:
	SButton button;
public:
	void init();
	void update();
	void draw();
};
