#include <fountain/fountaindef.h>

class AnimeSprite : public ftSprite
{
	ftAnime::Anime* ap;
public:
	void setAnime(ftAnime::Anime* ani);
	void update();
	void draw();
};

class editorScene : public ftScene::Scene
{
	int aniN;
	ftAnime::FrameAnime animeBackUp[100];
	ftAnime::FrameAnime anime[100];
	AnimeSprite animeS[100];
	ftAnime::AFSM mainAFSM;
	int mouse;
	int selectAS;
	int pa, pb;
	int signal;
	bool showAll;
	int lastSelect;
public:
	void init();
	void update();
	void draw();
};
