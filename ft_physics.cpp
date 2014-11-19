#include <fountain/ft_physics.h>

using ftPhysics::Body;
using ftPhysics::World;

static b2BodyDef defaultBodyDef;
static b2FixtureDef defaultFixtureDef;
static float defaultTimeStep = 1.0f / 60.0f;
static ftVec2 defaultGravity(0.0f, -10.0f);

void ftPhysics::init()
{
}

Body::Body()
{
	Body::body = NULL;
	Body::setPosition(0.0f, 0.0f);
	Body::setRectSize(ftVec2(1.0f, 1.0f));
	Body::isDynamic = true;
}

Body::Body(float x, float y, bool dynamic)
{
	Body::body = NULL;
	Body::setPosition(x, y);
	Body::setRectSize(ftVec2(1.0f, 1.0f));
	Body::isDynamic = dynamic;
}

void Body::setBody(b2Body* b2bd)
{
	Body::body = b2bd;
	Body::body->SetUserData(Body::body);
}

void Body::autoCreateFixtures()
{
	//Test Code
	b2PolygonShape aBox;
	aBox.SetAsBox(Body::getRectSize().x / 2.0f, Body::getRectSize().y / 2.0f);
	if (Body::isDynamic) {
		defaultFixtureDef.shape = &aBox;
		defaultFixtureDef.density = 1.0f;
		defaultFixtureDef.friction = 0.3f;
		Body::body->CreateFixture(&defaultFixtureDef);
	} else {
		Body::body->CreateFixture(&aBox, 0.0f);
	}
}

void Body::update()
{
	b2Vec2 bv = Body::body->GetPosition();
	float angle = Body::body->GetAngle();
	//TODO: add sprite class
	Body::setPosition(bv.x, bv.y);
	Body::setAngle(angle);
}

World::World(ftVec2 gravity)
{
	b2Vec2 g(gravity.x, gravity.y);
	World::world = new b2World(g);
}

bool World::addBody(Body* bd)
{
	//TODO: set defaultBodyDef
	ftVec2 pos = bd->getPosition();
	defaultBodyDef.position.Set(pos.x, pos.y);
	if (bd->isDynamic) {
		defaultBodyDef.type = b2_dynamicBody;
	} else {
		defaultBodyDef.type = b2_staticBody;
	}
	if (World::bodyCon.add(bd) == true) {
		bd->setBody(World::world->CreateBody(&defaultBodyDef));
		bd->autoCreateFixtures();
		return true;
	} else {
		return false;
	}
}

void World::delHeadBody()
{
	Body* bdPoint = World::bodyCon.getHead();
	World::world->DestroyBody(bdPoint->body);
	delete bdPoint;
	World::bodyCon.delHead();
}

void bodyUpdate(Body* bd)
{
	bd->update();
}

void bodyDraw(Body* bd)
{
	bd->draw();
}

void World::update(float timeStep)
{
	World::world->Step(timeStep, 8, 3);
	World::bodyCon.doWith(bodyUpdate);
}

void World::update()
{
	World::update(defaultTimeStep);
}

void World::draw()
{
	World::bodyCon.doWith(bodyDraw);
}

namespace fountain {

World mainWorld(defaultGravity);

};
