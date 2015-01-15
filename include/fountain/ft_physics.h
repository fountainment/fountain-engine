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
void setIterations(int vIter, int pIter);
ftVec2 render2Physics(ftVec2 v);
ftVec2 physics2Render(ftVec2 v);

class Body : public ftSprite
{
private:
public:
	ftShape shape;
	b2Body* body;
	int bodyType;

	Body();
	Body(float x, float y, int bodyT = FT_Dynamic);
	void setBody(b2Body* b2bd);
	void autoCreateFixtures();
	void update();
	void draw();
	void setShape(const ftShape & shape);
};

class World
{
private:
	container<Body*, BODY_MAXNUM> bodyCon;
	bool doDebugDraw;
public:
	b2World* world;
	World(ftVec2 gravity);
	bool addBody(Body* bd);
	void delHeadBody();
	void update();
	void update(float timeStep);
	void draw();
	bool empty();
	void setDebugDraw(bool dd);
};

}

#endif
