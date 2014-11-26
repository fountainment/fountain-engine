#ifndef _FT_PHYSICS_H_
#define _FT_PHYSICS_H_

#include <fountain/ft_data.h>
#include <Box2D/Box2D.h>

#define BODY_MAXNUM 2000

namespace ftPhysics {

void init();

class Body : public ftSprite
{
private:
public:
	b2Body* body;
	bool isDynamic;

	Body();
	Body(float x, float y, bool dynamic = true);
	void setBody(b2Body* b2bd);
	void autoCreateFixtures();
	void update();
};

class World
{
private:
	container<Body*, BODY_MAXNUM> bodyCon;
public:
	b2World* world;
	World(ftVec2 gravity);
	bool addBody(Body* bd);
	void delHeadBody();
	void update();
	void update(float timeStep);
	void draw();
	bool empty();
};

};

#endif
