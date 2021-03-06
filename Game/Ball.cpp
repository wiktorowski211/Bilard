#include "Ball.h"
const int PPM = 5;
int CBall::n = 0;
int CBall::x = 0;

CBall::CBall(b2World & world) {
	
}

void CBall::SetWorld(b2World & world) {
	m_body = world.CreateBody(&m_bodyDef);
	m_body->CreateFixture(&m_bodyFix);
	m_body->SetUserData(this);
}

CBall::CBall() {
	float size = 4.0f;
	m_sprite.setOrigin(size*PPM, size*PPM);
	m_bodyDef.position.Set(640.0f / PPM + n * 50.0f / PPM, 955.0f / PPM);
	n++;
	m_bodyDef.linearDamping = 0.8f; // op�r powietrza (domy�lnie 0.0f)
	m_bodyDef.angularDamping = 1.0f;;
	m_bodyDef.type = b2_dynamicBody;
	m_bodyShape.m_radius = size;
	m_bodyFix.shape = &m_bodyShape;
	m_bodyFix.density = 100.0f;
	m_bodyFix.friction = 0.0f;
	m_bodyFix.restitution = 0.95f;
}

b2Body* CBall::GetBody() {
	return m_body;
}


void CBall::update() {
	//m_shape.setRotation(m_body->GetAngle());
	m_sprite.setPosition(m_body->GetPosition().x*PPM, m_body->GetPosition().y*PPM);
}

void CBall::ResetPosition(b2World & world) {
	//destroy stuff (memory management, otherwise they build up)
	m_body->DestroyFixture(m_body->GetFixtureList());
	world.DestroyBody(m_body);
	float offset1, offset2;
	n++;
	offset1 = (x * 38.0f) / PPM;
	offset2 = (n * 46.0f) / PPM;
	switch (x) {
	case 1:
		m_bodyDef.position.Set(520.0f / PPM + offset1, 684.0f / PPM - offset2);
		break;
	case 2:
		m_bodyDef.position.Set(520.0f / PPM + offset1, 661.0f / PPM - offset2);
		break;
	case 3:
		m_bodyDef.position.Set(520.0f / PPM + offset1, 636.0f / PPM - offset2);
		break;
	case 4:
		m_bodyDef.position.Set(520.0f / PPM + offset1, 611.0f / PPM - offset2);
		break;
	case 5:
		m_bodyDef.position.Set(520.0f / PPM + offset1, 584.0f / PPM - offset2);
		break;
	}
	m_bodyDef.angularVelocity = 0;
	m_bodyDef.linearVelocity.Set(0, 0);
	SetWorld(world);
	update();
}
void CBall::scoredPosition(b2World & world) {
	m_body->DestroyFixture(m_body->GetFixtureList());
	world.DestroyBody(m_body);
	m_bodyDef.position.Set(640.0f / PPM + x * 50.0f / PPM, 955.0f / PPM);
	x++;
	m_bodyDef.angularVelocity = 0;
	m_bodyDef.linearVelocity.Set(0, 0);
	SetWorld(world);
	update();
}
void CBall::whiteReset(b2World & world) {
	m_body->DestroyFixture(m_body->GetFixtureList());
	world.DestroyBody(m_body);	
	m_bodyDef.position.Set(1300.0f / PPM, 540.0f / PPM);
	m_bodyDef.angularVelocity = 0;
	m_bodyDef.linearVelocity.Set(0, 0);
	SetWorld(world);
	update();
}
void CBall::move(float x,float y) {
	b2Vec2 velocity;
	velocity.y = 10*y;
	velocity.x= 10*x;
	m_body->SetLinearVelocity(velocity);
}

CBall::~CBall()
{
}

