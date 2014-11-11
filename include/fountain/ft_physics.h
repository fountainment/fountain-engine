#ifndef _FT_PHYSICS_H_
#define _FT_PHYSICS_H_

#include <fountain/ft_data.h>
#include <Box2D/Box2D.h>

#define BODY_MAXNUM 2000

namespace ftPhysics {

void init();

class Body : public Sprite
{
private:
	b2Body* body;
public:
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
	b2World* world;
	container<Body*, BODY_MAXNUM> bodyCon;
public:
	World(ftVec2 gravity);
	void addBody(Body* bd);
	void update();
	void update(float timeStep);
	void draw();
}; 

};

namespace fountain {

extern ftPhysics::World mainWorld;

};

#endif
