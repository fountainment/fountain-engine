#include <fountain/fountaindef.h>

using ftPhysics::World;
using ftPhysics::Body;

class TDAScene : public ftScene::Scene
{
private:
	World world;
	Body fly, ground;
	Body ball;
	float flySpeed;
	container<Body, 100> bodyCon;
public:
	void init();
	void update();
	void draw();
};
