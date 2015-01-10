#ifndef _FT_PHYSICS_H_
#define _FT_PHYSICS_H_

#include <fountain/ft_data.h>
#include <Box2D/Box2D.h>

#define BODY_MAXNUM 2000

//TODO: complete Kinematic type support
#define FT_Static 1
#define FT_Dynamic 2
#define FT_Kinematic 3

namespace ftPhysics {

bool init();
void close();

void setRatio(float rt);
ftVec2 render2Physics(ftVec2 v);
ftVec2 physics2Render(ftVec2 v);

class Body : public ftSprite
{
private:
public:
	b2Body* body;
	int bodyType;

	Body();
	Body(float x, float y, int bodyT = FT_Dynamic);
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
