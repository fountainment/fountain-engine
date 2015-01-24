#include <fountain/fountaindef.h>

#include <cstdio>

using ftPhysics::Body;
using ftPhysics::World;

static b2BodyDef defaultBodyDef;
static b2FixtureDef defaultFixtureDef;
static float defaultTimeStep = 1.0f / 60.0f;
static ftVec2 defaultGravity(0.0f, -10.0f);

static int defaultVelocityIterations = 8;
static int defaultPositionIterations = 3;

static float ratio = 1.0f;

class DebugDraw : public b2Draw
{
private:
public:
	~DebugDraw()
	{

	}

	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		ftVec2 a, b;
		ftRender::useColor(ftColor(color.r, color.g, color.b, color.a));
		for (int i = 0; i < vertexCount; i++) {
			a = ftVec2(vertices[i].x, vertices[i].y);
			if (i == vertexCount - 1)
				b = ftVec2(vertices[0].x, vertices[0].y);
			else
				b = ftVec2(vertices[i + 1].x, vertices[i + 1].y);
			a = a * ratio;
			b = b * ratio;
			ftRender::drawLine(a, b);
		}
	}

	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		std::vector<ftVec2> v;
		for (int i = 0; i < vertexCount; i++) {
			v.push_back(ftVec2(vertices[i].x * ratio, vertices[i].y * ratio));
		}
		ftShape poly(v, vertexCount, true);
		ftRender::useColor(ftColor(color.r, color.g, color.b, color.a));
		ftRender::drawShape(poly);
		DrawPolygon(vertices, vertexCount, b2Color(color.r + 0.2, color.g + 0.2, color.b + 0.2));
	}

	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
	{
		ftRender::useColor(ftColor(color.r, color.g, color.b, color.a));
		ftRender::transformBegin();
		ftRender::ftTranslate(ftVec2(center.x, center.y) * ratio);
		ftRender::drawCircleEdge(radius * ratio);
		ftRender::transformEnd();
	}

	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
	{
		ftRender::useColor(ftColor(color.r, color.g, color.b, color.a));
		ftRender::transformBegin();
		ftRender::ftTranslate(ftVec2(center.x, center.y) * ratio);
		ftRender::drawCircle(radius * ratio);
		ftRender::useColor(FT_White);
		ftRender::drawLine(ftVec2(0, 0), ftVec2(axis.x, axis.y) * (radius * ratio));
		ftRender::transformEnd();
		DrawCircle(center, radius, b2Color(color.r + 0.2, color.g + 0.2, color.b + 0.2));
	}

	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
		ftVec2 a(p1.x, p1.y);
		ftVec2 b(p2.x, p2.y);
		a = a * ratio;
		b = b * ratio;
		ftRender::useColor(ftColor(color.r, color.g, color.b, color.a));
		ftRender::drawLine(a, b);
	}

	void DrawTransform(const b2Transform& xf)
	{
		b2Vec2 p1 = xf.p, p2;
		const float32 k_axisScale = 0.2f;
		p2 = p1 + k_axisScale * xf.q.GetXAxis();
		DrawSegment(p1, p2, b2Color(1, 0, 0));

		p2 = p1 + k_axisScale * xf.q.GetYAxis();
		DrawSegment(p1, p2, b2Color(0, 1, 0));
	}
};

static DebugDraw debugDraw;

bool ftPhysics::init()
{
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	//flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	//flags += b2Draw::e_centerOfMassBit;
	debugDraw.SetFlags(flags);
	return true;
}

void ftPhysics::close()
{
}

void ftPhysics::setRatio(float rt)
{
	ratio = rt;
}

float ftPhysics::getRatio()
{
	return ratio;
}

void ftPhysics::setIterations(int vIter, int pIter)
{
	defaultVelocityIterations = vIter;
	defaultPositionIterations = pIter;
}

ftVec2 ftPhysics::render2Physics(ftVec2 v)
{
	return v / ratio;
}

ftVec2 ftPhysics::physics2Render(ftVec2 v)
{
	return v * ratio;
}

b2Shape* ftPhysics::createb2ShapeWithFtShape(ftShape & shape)
{
	b2Shape *b2shape;
	b2PolygonShape *pShape;
	b2CircleShape *cShape;
	b2ChainShape *chShape;
	int type = shape.getType();
	const float * v;
	b2Vec2 bv[16];
	int n;
	switch (type) {
	case FT_Circle:
		cShape = new b2CircleShape;
		cShape->m_radius = shape.getR() / ratio;
		b2shape = cShape;
		break;

	case FT_Polygon:
		v = shape.getData();
		n = shape.getN();
		for (int i = 0; i < n; i++) {
			bv[i].Set(v[i * 2] / ratio, v[i * 2 + 1] / ratio);
		}
		pShape = new b2PolygonShape;
		pShape->Set(bv, n);
		b2shape = pShape;
		break;

	//TODO: complete FT_LINE shape support(loop)
	case FT_Line:
		v = shape.getData();
		n = shape.getN();
		for (int i = 0; i < n; i++) {
			bv[i].Set(v[i * 2] / ratio, v[i * 2 + 1] / ratio);
		}
		chShape = new b2ChainShape;
		chShape->CreateChain(bv, n);
		b2shape = chShape;
		break;

	case FT_Rect:
		v = shape.getData();
		pShape = new b2PolygonShape;
		pShape->SetAsBox(v[0] / 2.0f / ratio, v[1] / 2.0f / ratio);
		b2shape = pShape;
		break;

	default:
		b2shape = NULL;
		break;
	}
	return b2shape;
}

b2Body* ftPhysics::createBodyInWorld(b2World* world, float x, float y, int type)
{
	defaultBodyDef.position.Set(x / ratio, y / ratio);
	switch (type) {
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
	return world->CreateBody(&defaultBodyDef);
}

//class ftPhysics::Body
Body::Body()
{
	body = NULL;
	setPosition(0.0f, 0.0f);
	setRectSize(ftVec2(1.0f, 1.0f));
	bodyType = FT_Dynamic;
}

Body::~Body()
{
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
	body->SetUserData(this);
}

void Body::setImage(const ftRender::SubImage & image)
{
	im = image;
}

void Body::autoCreateFixtures()
{
	//Test Code
	b2Shape *b2shape = ftPhysics::createb2ShapeWithFtShape(shape);

	switch (bodyType) {
	case FT_Dynamic:
	case FT_Kinematic:/* not tested */
		defaultFixtureDef.shape = b2shape;
		defaultFixtureDef.density = 1.0f;
		defaultFixtureDef.friction = 0.3f;
		body->CreateFixture(&defaultFixtureDef);
		break;

	case FT_Static:
		body->CreateFixture(b2shape, 0.0f);
		break;
	}

	delete b2shape;
}

void Body::update()
{
	b2Vec2 bv = body->GetPosition();
	float angle = body->GetAngle();
	setPosition(bv.x * ratio, bv.y * ratio);
	setAngle(angle);
}

void Body::draw()
{
	ftVec2 pos = getPosition();
	ftRender::transformBegin(pos.x, pos.y, getAngle(), 1.0f, getColor());
	ftRender::drawShape(shape);
	ftRender::transformEnd();
}

void Body::setShape(const ftShape & shape)
{
	this->shape = shape;
}

void Body::setVelocity(ftVec2 v)
{
	v = v / ratio;
	body->SetLinearVelocity(b2Vec2(v.x, v.y));
}

//class ftPhysics::World
World::World(ftVec2 gravity)
{
	b2Vec2 g(gravity.x, gravity.y);
	world = new b2World(g);
	world->SetAllowSleeping(true);
	world->SetDebugDraw(&debugDraw);
	doDebugDraw = false;
}

World::~World()
{
	delete world;
	world = NULL;
}

bool World::addBody(Body* bd)
{
	ftVec2 pos = bd->getPosition();
	if (World::bodyCon.add(bd) == true) {
		bd->setBody(ftPhysics::createBodyInWorld(world, pos.x, pos.y, bd->bodyType));
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

void World::update(float timeStep)
{
	world->Step(timeStep, defaultVelocityIterations,
	            defaultPositionIterations);
	bodyCon.update();
}

void World::update()
{
	update(defaultTimeStep);
}

void World::draw()
{
	bodyCon.draw();
	if (doDebugDraw == true)
		world->DrawDebugData();
}

void World::setDebugDraw(bool dd)
{
	doDebugDraw = dd;
}
