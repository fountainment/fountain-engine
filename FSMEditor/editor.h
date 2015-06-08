#include <fountain/fountaindef.h>

class editorScene : public ftScene::Scene
{
	int aniN;
        ftAnime::FrameAnime anime[100];
        ftAnime::AFSM mainAFSM;
        int mouse;
        float sc;
public:
        void init();
        void update();
        void draw();
};
