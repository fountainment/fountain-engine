#include <fountain/fountaindef.h>
#include "polygon.h"

using namespace fim;

class editorScene : public ftScene::Scene
{
public:
	void init();
	void update();
	void draw();
private:
	ftShape ori;
	std::vector<ftShape> res;
	int choose;
};
