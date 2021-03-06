#pragma once
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

//i added this too, just for fun/show BHN
class CBall
{
private:
	b2Body * m_body;
	b2BodyDef m_bodyDef;
	b2CircleShape m_bodyShape;
	b2FixtureDef m_bodyFix;
	sf::Sprite m_sprite; //SFML sprite
	sf::Texture texture;
	static int n;
	static int x;
	int number;
public:
	
	CBall(b2World & world);
	CBall();
	void SetWorld(b2World & world);
	void SetInfo(int number) {
	this->number = number;
	texture.loadFromFile("Bile/"+std::to_string(this->number)+".png");
	texture.setSmooth(true);
	m_sprite.setTexture(texture);
	}
	int getInfo() { return this->number; }
	int getTeam() {
		if (number < 8) {
			return 1;
		}
		if (number > 8 && number < 16) {
			return 2;
		}
		if (number > 16) {
			return 3;
		}
		else return 0;
	}
	b2Body* GetBody(); // Get Box2d body
	sf::Sprite & GetSprite(){ return m_sprite; }
	void update(); // Get SFML shape
	void ResetPosition(b2World & world); 
	void scoredPosition(b2World & world);
	void CBall::whiteReset(b2World & world);
	void CBall::move(float x, float y);
	void setN(int newN) { n = newN; }  //reset n
	void setX(int newX) { x = newX; }
	sf::Sprite getSprite() { return m_sprite; }
	int getN() { return n; }
	~CBall();
};
