#include <fountain/ft_physics.h>

using ftPhysics::Body;
using ftPhysics::World;

static b2BodyDef defaultBodyDef;
static b2FixtureDef defaultFixtureDef;
static float defaultTimeStep = 1.0f / 60.0f;
static ftVec2 defaultGravity(0.0f, -10.0f);

static int defaultVelocityIterations = 6;
static int defaultPositionIterations = 2;

static float ratio = 1.0f;

bool ftPhysics::init()
{
	return true;
}

void ftPhysics::close()
{
}

void ftPhysics::setRatio(float rt)
{
	ratio = rt;
}

ftVec2 ftPhysics::render2Physics(ftVec2 v)
{
	return v / ratio;
}

ftVec2 ftPhysics::physics2Render(ftVec2 v)
{
	return v * ratio;
}

Body::Body()
{
	body = NULL;
	setPosition(0.0f, 0.0f);
	setRectSize(ftVec2(1.0f, 1.0f));
	bodyType = FT_Dynamic;
}

Body::Body(float x, float y, int bodyT)
{
	body = NULL;
	setPosition(x, y);
	setRectSize(ftVec2(1.0f, 1.0f));
	bodyType = bodyT;
}

void Body::setBody(b2Body* b2bd)
{
	body = b2bd;
	body->SetUserData(body);
}

void Body::autoCreateFixtures()
{
	//Test Code
	b2Shape *b2shape;
	b2PolygonShape pShape;
	b2CircleShape cShape;
	b2ChainShape chShape;
	int type = shape.getType();
	const float * v;
	b2Vec2 bv[16];
	int n;
	switch (type) {
	case FT_Circle:
		cShape.m_radius = shape.getR() / ratio;
		b2shape = &cShape;
		break;

	case FT_Polygon:
		v = shape.getData();
		n = shape.getN();
		for (int i = 0; i < n; i++) {
			bv[i].Set(v[i * 2] / ratio, v[i * 2 + 1] / ratio);
		}
		pShape.Set(bv, n);
		b2shape = &pShape;
		break;

	//TODO: complete FT_LINE shape support(loop)
	case FT_Line:
		v = shape.getData();
		n = shape.getN();
		for (int i = 0; i < n; i++) {
			bv[i].Set(v[i * 2] / ratio, v[i * 2 + 1] / ratio);
		}
		chShape.CreateChain(bv, n);
		b2shape = &chShape;
		break;

	case FT_Rect:
		v = shape.getData();
		pShape.SetAsBox(v[0] / 2.0f / ratio, v[1] / 2.0f / ratio);
		b2shape = &pShape;
		break;

	default:
		b2shape = NULL;
		return;
		break;
	}

	switch (bodyType) {
	case FT_Dynamic:
		defaultFixtureDef.shape = b2shape;
		defaultFixtureDef.density = 1.0f;
		defaultFixtureDef.friction = 0.3f;
		body->CreateFixture(&defaultFixtureDef);
		break;

	case FT_Static:
		body->CreateFixture(b2shape, 0.0f);
		break;

	case FT_Kinematic:
		//TODO: createFixture for kinematicBody
		break;
	}
}

void Body::update()
{
	b2Vec2 bv = body->GetPosition();
	float angle = body->GetAngle();
	setPosition(bv.x * ratio, bv.y * ratio);
	setAngle(angle);
}

World::World(ftVec2 gravity)
{
	b2Vec2 g(gravity.x, gravity.y);
	world = new b2World(g);
}

bool World::addBody(Body* bd)
{
	ftVec2 pos = bd->getPosition();
	defaultBodyDef.position.Set(pos.x / ratio, pos.y / ratio);

	switch (bd->bodyType) {
	case FT_Dynamic:
		defaultBodyDef.type = b2_dynamicBody;
		break;

	case FT_Static:
		defaultBodyDef.type = b2_staticBody;
		break;

	case FT_Kinematic:
		defaultBodyDef.type = b2_kinematicBody;
		break;
	}

	if (World::bodyCon.add(bd) == true) {
		bd->setBody(world->CreateBody(&defaultBodyDef));
		bd->autoCreateFixtures();
		return true;
	} else {
		return false;
	}
}

void World::delHeadBody()
{
	Body* bdPoint = bodyCon.getHead();
	world->DestroyBody(bdPoint->body);
	delete bdPoint;
	bodyCon.delHead();
}

bool World::empty()
{
	return bodyCon.empty();
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
	world->Step(timeStep, defaultVelocityIterations,
			defaultPositionIterations);
	bodyCon.doWith(bodyUpdate);
}

void World::update()
{
	update(defaultTimeStep);
}

void World::draw()
{
	bodyCon.doWith(bodyDraw);
}
