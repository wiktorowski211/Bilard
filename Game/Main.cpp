#include <iostream>
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <ctime>
#include "Ball.h"
#include <stdio.h>
#include <Windows.h>
#include <vector>
const int PPM = 5;

using namespace std;
class Player {
    public:
	int points = 0;
	bool finishHim = false;
	bool win = false;
	int team=-1;
	string name;
	Player() {
		
	}
	void reset() {
		points = 0;
		finishHim = 0;
		win = false;
		team = -1;
		string name = "";
	}
};
int WhiteBallPocket(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();
	int entityA = (int)(fixtureA->GetBody()->GetUserData());
	CBall* entityB = static_cast<CBall*>(fixtureB->GetBody()->GetUserData());
	int y = 5;
	return y;
}
class MyContactListener : public b2ContactListener
{
	public:
		Player player[2];
		bool faul=false;
		bool finish=false;
		bool opentable = true;
		int turn=0;
		bool turnBlockade = false;
		bool noHit = false;
		int ballNumber = 0;
		void changeTurn() {
			if (turn == 0) {
				turn = 1;
			}
			else { turn = 0; }
			turnBlockade = true;
		}
		int otherPlayer() {
			if (turn == 0) {
				return 1;
			}
			else { return 0; }
			}
		void reset() {
			player[0].reset();
			player[1].reset();
			faul = false;
			finish = false;
			opentable= true;
			turn = 0;
			turnBlockade = false;
			noHit = false;
			ballNumber = 0;
		}
	private:
		void BeginContact(b2Contact* contact) {
			b2Fixture* fixtureA = contact->GetFixtureA();
			b2Fixture* fixtureB = contact->GetFixtureB();
			CBall* Sensor1 = static_cast<CBall*>(fixtureA->GetBody()->GetUserData()); //pockets
			CBall* Sensor2 = static_cast<CBall*>(fixtureB->GetBody()->GetUserData()); //balls
			if (Sensor1 != nullptr && Sensor2 != nullptr) {
				
				if (player[turn].points == 7 && Sensor2->getInfo() == 8 && Sensor1->getInfo() > 16)//wygrana 
				{
					if (turnBlockade == false) {
						player[turn].win = true;
						player[turn].points++;
					}
					else {
						player[this->otherPlayer()].win = true;
					}
				}
				if (player[turn].points < 7 && Sensor2->getInfo() == 8 && Sensor1->getInfo() > 16) //wbicie czarnej przed wbiciem wszystkich bil
				{
					player[this->otherPlayer()].win = true;
				}
				if ( Sensor1->getInfo() > 16 && Sensor2->getInfo() == 16)//biala w luzie
				{
					faul = true;
					ballNumber = Sensor2->getInfo();
				}
				if (turnBlockade == false && Sensor2->getInfo() == 16 && Sensor1->getInfo()==8 && noHit==true&& player[turn].points<7)//biala vs czarna
				{
					faul = true;
				}
				
				if (opentable == false) {
					if (Sensor1->getTeam() == 3 && Sensor2->getTeam()>0 && Sensor2->getTeam()<3)//trafienie bila do luzy
					{
						if (Sensor2->getTeam() == player[turn].team) {
							player[turn].points++;
						}
						else {
							player[this->otherPlayer()].points++;
							faul = true;
						}
						ballNumber = Sensor2->getInfo();
					}
					
					if (turnBlockade == false && Sensor2->getInfo() == 16 && player[turn].team == Sensor1->getTeam())//biala vs jego bila
					{
						turnBlockade = true;
					}
					if (player[turn].points<7 && turnBlockade==false && noHit==true && Sensor2->getInfo() == 16 && player[turn].team != Sensor1->getTeam())//biala vs bila przeciwnika lub czarna
					{
						faul = true;
					}
				}

				if (opentable == true && Sensor1->getTeam() == 3 && Sensor2->getTeam() != 0)//otwarcie stolu
				{
					player[turn].team = Sensor2->getTeam();
					player[turn].points++;
					ballNumber = Sensor2->getInfo();
					this->setTeam();
					opentable =false;
				}
				if (Sensor2->getInfo() == 16 && Sensor1->getTeam()>=0 && Sensor1->getTeam()<3)//biala vs ktorakolwiek z bil
				{
					noHit = false;
				}
			}
		

		}
		void setTeam() {
			if (player[0].team == 1) {
				player[1].team = 2;
			}
			else if (player[0].team == 2) {
				player[1].team = 1;
			}
			else if (player[1].team == 2) {
				player[0].team = 1;
			}
			else if (player[1].team == 1) {
				player[0].team = 2;
			}
		}
};
bool isMouseOver(const sf::Sprite &ball, sf::Window & app)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(app);
	float ballRadius = ball.getTexture()->getSize().x/2;
	sf::Vector2f ballPosition= ball.getPosition();
	if (abs((float)mousePosition.x - ballPosition.x) < ballRadius && abs((float)mousePosition.y - ballPosition.y) < ballRadius)
		return true;
	return false;
}
bool isMouseOver(const sf::RectangleShape &button, sf::Window &app) {
	
		sf::Vector2i mousePosition = sf::Mouse::getPosition(app);
		if (abs((float)mousePosition.x - button.getPosition().x) < button.getSize().x/2  && abs((float)mousePosition.y - button.getPosition().y) < button.getSize().y/2 ) {
			return true;
		};
		return false;
}
void resetGame(CBall balls[16], b2World& world) {
	balls->setN(0);
	balls->setX(1);
	for (int i = 0; i < 5; i++) {
		balls[i].ResetPosition(world);
	}
	balls->setN(0);
	balls->setX(2);
	for (int i = 5; i < 9; i++) {
		balls[i].ResetPosition(world);
	}
	balls->setN(0);
	balls->setX(3);
	for (int i = 9; i < 12; i++) {
		balls[i].ResetPosition(world);
	}
	balls->setN(0);
	balls->setX(4);
	for (int i = 12; i < 14; i++) {
		balls[i].ResetPosition(world);
	}
	balls->setN(0);
	balls->setX(5);
	for (int i = 14; i < 15; i++) {
		balls[i].ResetPosition(world);
	}
	balls[15].whiteReset(world);
	balls->setN(0);
	balls->setX(0);
}
float framesPS(sf::Clock &);
int main() {
	sf::RenderWindow app(sf::VideoMode(1920, 1080, 32), "Bilard");
	sf::Image icon;
	icon.loadFromFile("8.png");
	const unsigned char * Icon = icon.getPixelsPtr();
	app.setIcon(40, 40, Icon);
	//world
	b2Vec2 gravity(0, 0);
	b2World world(gravity);
	world.Dump();
	//Texture
	sf::Texture texture1;
	texture1.loadFromFile("Pool.jpg");
	sf::Sprite background(texture1);
	sf::Texture texture2;
	texture2.loadFromFile("av2.jpg");
	sf::Texture menu;
	menu.loadFromFile("menu.jpg");
	menu.setSmooth(true);
	sf::Sprite menuSprite(menu);
	sf::Texture PoolStick;
	PoolStick.loadFromFile("PoolStick.png");
	PoolStick.setSmooth(true);
	sf::Sprite stick(PoolStick);
	stick.setScale(0.5, 0.5);
	stick.setOrigin(sf::Vector2f(17,0));
	sf::Sprite Avatar1(texture2);
	Avatar1.setPosition(392, 30);
	sf::Texture texture3;
	texture3.loadFromFile("av1.jpg");
	sf::Sprite Avatar2(texture3);
	Avatar2.setPosition(1417, 30);
	sf::RectangleShape turnRect(sf::Vector2f(350, 120));
	sf::RectangleShape player1Name;
	player1Name.setOrigin(200, 40);
	player1Name.setPosition(sf::Vector2f(960, 540));
	player1Name.setSize(sf::Vector2f(400, 80));
	player1Name.setOutlineThickness(2.0f);
	player1Name.setOutlineColor(sf::Color::Black);
	player1Name.setFillColor(sf::Color(135, 255, 130));
	sf::RectangleShape player2Name;
	player2Name.setOrigin(200, 40);
	player2Name.setPosition(sf::Vector2f(960, 640));
	player2Name.setSize(sf::Vector2f(400, 80));
	player2Name.setOutlineThickness(2.0f);
	player2Name.setOutlineColor(sf::Color::Black);
	player2Name.setFillColor(sf::Color(135, 255, 130));
	sf::RectangleShape newGameButton;
	newGameButton.setOrigin(200, 40);
	newGameButton.setPosition(sf::Vector2f(960, 740));
	newGameButton.setSize(sf::Vector2f(400, 80));
	newGameButton.setOutlineThickness(2.0f);
	newGameButton.setOutlineColor(sf::Color::Black);
	sf::CircleShape aimCircle;
	aimCircle.setOutlineColor(sf::Color::White);
	aimCircle.setOrigin(20.0f, 20.0f);
	aimCircle.setRadius(19.0f);
	aimCircle.setFillColor(sf::Color(0, 0, 0, 0));
	aimCircle.setOutlineThickness(1.0f);
	turnRect.setFillColor(sf::Color(0, 0, 0, 0));
	turnRect.setOutlineColor(sf::Color::Blue);
	turnRect.setOutlineThickness(4.0f);
	//Pockets
	MyContactListener Referee;
	world.SetContactListener(&Referee);
	//Text
	sf::Font myFont;
	if (!myFont.loadFromFile("BAUHS93.TTF"))
		return EXIT_FAILURE;
	sf::Text Text("FPS", myFont);
	Text.setCharacterSize(20);
	Text.setFillColor(sf::Color(0, 255, 255, 255));
	Text.setPosition(25, 25);
	sf::Text Faul("Faul !",myFont);
	sf::Text mouseY("Mouse Y:", myFont);
	sf::Text mouseX("Mouse X:", myFont);
	sf::Text WhiteSpeed("White speed:", myFont);
	sf::Text Player1("", myFont);
	sf::Text Player2("", myFont);
	sf::Text Points1("Punkty: ", myFont);
	sf::Text Points2("Punkty: ", myFont);
	sf::Text openTable("Stól otwarty", myFont);
	sf::Text newGameText("Graj", myFont);
	sf::Text player1NameText("", myFont);
	sf::Text player2NameText("", myFont);
	sf::Text Win("", myFont);
	Win.setCharacterSize(100);
	Win.setPosition(630, 450);
	player1NameText.setCharacterSize(40);
	player1NameText.setPosition(sf::Vector2f(780, 510));
	player2NameText.setCharacterSize(40);
	player2NameText.setPosition(sf::Vector2f(780, 610));
	newGameText.setCharacterSize(40);
	newGameText.setPosition(sf::Vector2f(780, 710));
	Faul.setPosition(908, 60);
	Faul.setCharacterSize(40);
	Faul.setFillColor(sf::Color::Red);
	Faul.setPosition(908, 60);
	openTable.setCharacterSize(30);
	openTable.setPosition(875, 20);
	mouseX.setCharacterSize(18);
	mouseY.setCharacterSize(18);
	Points1.setCharacterSize(25);
	Points2.setCharacterSize(25);
	Player1.setCharacterSize(40);
	Player2.setCharacterSize(40);
	mouseX.setPosition(25, 70);
	mouseY.setPosition(25, 90);
	Player1.setPosition(517, 40);
	Points1.setPosition(517, 90);
	Points2.setPosition(1299, 90);
	WhiteSpeed.setPosition(25, 110);
	
	float timeStep = 1 / 180.0f;
	sf::Event Event = sf::Event();
	sf::Clock Clock;
	sf::Time counter;
	sf::Clock fpsClock;
	int frameNumber = 0;
	int lastFPSFrame = 0;
	string lastFPSDisplay;

	//balls
	const int count = 16; 
	CBall balls[count];
	int i = 0;
	
	for (i; i < count; i++) {
		balls[i].SetWorld(world);
	}
	balls[0].SetInfo(5); //orange
	balls[1].SetInfo(4); //purple
	balls[2].SetInfo(13);
	balls[3].SetInfo(2); //blue
	balls[4].SetInfo(11);//red
	balls[5].SetInfo(14);//green
	balls[6].SetInfo(3);
	balls[7].SetInfo(10);
	balls[8].SetInfo(6);
	balls[9].SetInfo(1);//yellow
	balls[10].SetInfo(8);//dark
	balls[11].SetInfo(15);//brown
	balls[12].SetInfo(12);
	balls[13].SetInfo(7);
	balls[14].SetInfo(9);
	balls[15].SetInfo(16);//white
	
	//edges
	{
		b2BodyDef myBodyDef;
		b2FixtureDef myFixtureDef;
		myFixtureDef.friction = 0.0f;
		myBodyDef.type = b2_staticBody;
		myBodyDef.position.Set(0.0f / PPM, 0.0f / PPM);
		b2EdgeShape edgeShape;
		
		//left
		edgeShape.Set(b2Vec2(370.0f / PPM, 290.0f / PPM), b2Vec2(370.0f / PPM, 790.0f / PPM));
		b2Body* staticBody = world.CreateBody(&myBodyDef);
		myFixtureDef.shape = &edgeShape;
		staticBody->CreateFixture(&myFixtureDef);

		//right
		edgeShape.Set(b2Vec2(1550.0f / PPM, 290.0f / PPM), b2Vec2(1550.0f / PPM, 790.0f / PPM));
		myFixtureDef.shape = &edgeShape;
		staticBody->CreateFixture(&myFixtureDef);

		//upper-left
		edgeShape.Set(b2Vec2(416.0f / PPM, 246.0f / PPM), b2Vec2(916.0f / PPM, 246.0f / PPM));
		myFixtureDef.shape = &edgeShape;
		staticBody->CreateFixture(&myFixtureDef);

		//upper-right
		edgeShape.Set(b2Vec2(1002.0f / PPM, 246.0f / PPM), b2Vec2(1502.0f / PPM, 246.0f / PPM));
		myFixtureDef.shape = &edgeShape;
		staticBody->CreateFixture(&myFixtureDef);

		//bottom-left
		edgeShape.Set(b2Vec2(416.0f / PPM, 834.0f / PPM), b2Vec2(916.0f / PPM, 834.0f / PPM));
		myFixtureDef.shape = &edgeShape;
		staticBody->CreateFixture(&myFixtureDef);

		//bottom-right
		edgeShape.Set(b2Vec2(1002.0f / PPM, 834.0f / PPM), b2Vec2(1502.0f / PPM, 834.0f / PPM));
		myFixtureDef.shape = &edgeShape;
		staticBody->CreateFixture(&myFixtureDef);
		
		//corners
		
		//right
		//right-upper
		edgeShape.Set(b2Vec2(1550.0f / PPM, 290.0f / PPM), b2Vec2(1573.0f / PPM, 267.0f / PPM));
		myFixtureDef.shape = &edgeShape;
		staticBody->CreateFixture(&myFixtureDef);

		//right-bottom
		edgeShape.Set(b2Vec2(1550.0f / PPM, 790.0f / PPM), b2Vec2(1573.0f / PPM, 814.0f / PPM));
		myFixtureDef.shape = &edgeShape;
		staticBody->CreateFixture(&myFixtureDef);

		//left
		//left-upper
		edgeShape.Set(b2Vec2(370.0f / PPM, 290.0f / PPM), b2Vec2(346.0f / PPM, 267.0f / PPM));
		myFixtureDef.shape = &edgeShape;
		staticBody->CreateFixture(&myFixtureDef);
		
		//left-bottom
		edgeShape.Set(b2Vec2(370.0f / PPM, 790.0f / PPM), b2Vec2(346.0f / PPM, 814.0f / PPM));
		myFixtureDef.shape = &edgeShape;
		staticBody->CreateFixture(&myFixtureDef);

		//bottom corners
		//left
		edgeShape.Set(b2Vec2(416.0f / PPM, 834.0f / PPM), b2Vec2(391.0f / PPM, 857.0f / PPM));
		myFixtureDef.shape = &edgeShape;
		staticBody->CreateFixture(&myFixtureDef);

		//middle-left
		edgeShape.Set(b2Vec2(916.0f / PPM, 834.0f / PPM), b2Vec2(928.0f / PPM, 857.0f / PPM));
		myFixtureDef.shape = &edgeShape;
		staticBody->CreateFixture(&myFixtureDef);

		//middle-right
		edgeShape.Set(b2Vec2(1002.0f / PPM, 834.0f / PPM), b2Vec2(992.0f / PPM, 857.0f / PPM));
		myFixtureDef.shape = &edgeShape;
		staticBody->CreateFixture(&myFixtureDef);

		//right
		edgeShape.Set(b2Vec2(1502.0f / PPM, 834.0f / PPM), b2Vec2(1527.0f / PPM, 857.0f / PPM));
		myFixtureDef.shape = &edgeShape;
		staticBody->CreateFixture(&myFixtureDef);

		//upper corners
		//left
		edgeShape.Set(b2Vec2(416.0f / PPM, 246.0f / PPM), b2Vec2(391.0f / PPM, 221.0f / PPM));
		myFixtureDef.shape = &edgeShape;
		staticBody->CreateFixture(&myFixtureDef);

		//middle-left
		edgeShape.Set(b2Vec2(916.0f / PPM, 246.0f / PPM), b2Vec2(928.0f / PPM, 221.0f / PPM));
		myFixtureDef.shape = &edgeShape;
		staticBody->CreateFixture(&myFixtureDef);

		//middle-right
		edgeShape.Set(b2Vec2(1002.0f / PPM, 246.0f / PPM), b2Vec2(992.0f / PPM, 221.0f / PPM));
		myFixtureDef.shape = &edgeShape;
		staticBody->CreateFixture(&myFixtureDef);

		//right
		edgeShape.Set(b2Vec2(1502.0f / PPM, 246.0f / PPM), b2Vec2(1527.0f / PPM, 221.0f / PPM));
		myFixtureDef.shape = &edgeShape;
		staticBody->CreateFixture(&myFixtureDef);
	}

	//sensors

	b2BodyDef sensorBodyDef;
	b2FixtureDef sensorFixture;
	sensorFixture.friction = 0.0f;
	sensorFixture.isSensor = true;
	sensorBodyDef.type = b2_staticBody;
	sensorBodyDef.position.Set(0.0f / PPM, 0.0f / PPM);
	b2EdgeShape sensorShape;

	//upper-right
	sensorShape.Set(b2Vec2(1527.0f / PPM, 206.0f / PPM), b2Vec2(1588.0f / PPM, 267.0f / PPM));
	b2Body* sensorBody1 = world.CreateBody(&sensorBodyDef);
	sensorFixture.shape = &sensorShape;
	CBall pocket;
	pocket.SetInfo(17);
	sensorBody1->SetUserData(&pocket);
	sensorBody1->CreateFixture(&sensorFixture);

	//upper-middle
	sensorShape.Set(b2Vec2(928.0f / PPM, 211.0f / PPM), b2Vec2(992.0f / PPM, 211.0f / PPM));
	CBall pocket2;
	b2Body* sensorBody2 = world.CreateBody(&sensorBodyDef);
	pocket2.SetInfo(18);
	sensorBody2->SetUserData(&pocket2);
	sensorBody2->CreateFixture(&sensorFixture);

	//upper-left
	sensorShape.Set(b2Vec2(331.0f / PPM, 267.0f / PPM), b2Vec2(391.0f / PPM, 206.0f / PPM));
	CBall pocket3;
	b2Body* sensorBody3 = world.CreateBody(&sensorBodyDef);
	pocket3.SetInfo(19);
	sensorBody3->SetUserData(&pocket3);
	sensorBody3->CreateFixture(&sensorFixture);

	//bottom-left
	sensorShape.Set(b2Vec2(331.0f / PPM, 814.0f / PPM), b2Vec2(391.0f / PPM, 872.0f / PPM));
	CBall pocket4;
	b2Body* sensorBody4 = world.CreateBody(&sensorBodyDef);
	pocket4.SetInfo(20);
	sensorBody4->SetUserData(&pocket4);
	sensorBody4->CreateFixture(&sensorFixture);

	//bottom-middle
	sensorShape.Set(b2Vec2(928.0f / PPM, 867.0f / PPM), b2Vec2(992.0f / PPM, 867.0f / PPM));
	CBall pocket5;
	b2Body* sensorBody5 = world.CreateBody(&sensorBodyDef);
	pocket5.SetInfo(21);
	sensorBody5->SetUserData(&pocket5);
	sensorBody5->CreateFixture(&sensorFixture);

	//bottom-right
	sensorShape.Set(b2Vec2(1527.0f / PPM, 872.0f / PPM), b2Vec2(1588.0f / PPM, 814.0f / PPM));
	CBall pocket6;
	b2Body* sensorBody6 = world.CreateBody(&sensorBodyDef);
	pocket6.SetInfo(22);
	sensorBody6->SetUserData(&pocket6);
	sensorBody6->CreateFixture(&sensorFixture);

	bool shoot=false;
	sf::VertexArray lines(sf::LineStrip, 2);
	sf::Vector2i moveVector1, moveVector2;
	int turn=-1;
	int points = -1;
	int tabIndex = 1;
	bool menuDisplay = true;

	while (app.isOpen())
	{
		if (abs(balls[15].GetBody()->GetLinearVelocity().x) < 0.1f && abs(balls[15].GetBody()->GetLinearVelocity().y) < 0.1f && points == Referee.player[Referee.turn].points && turn == Referee.turn) {
			Referee.changeTurn();
		}//zmiana gracza jesli nic nie wbil 
		if (abs(balls[15].GetBody()->GetLinearVelocity().x) < 0.1f && abs(balls[15].GetBody()->GetLinearVelocity().y) < 0.1f && Referee.noHit==true) {
			Referee.faul = true;
		}
		world.Step(timeStep, 8, 3);
		char buff[32];
		sprintf_s(buff, "%f", framesPS(Clock));
		Text.setString("FPS: " + sf::String(buff));
		sf::Vector2i position = sf::Mouse::getPosition(app);
		mouseX.setString("Mouse X : " + std::to_string(position.x));
		player1NameText.setString(Referee.player[0].name);
		player2NameText.setString(Referee.player[1].name);
		mouseY.setString("Mouse Y : " + std::to_string(position.y));
		Points1.setString("Punkty: " + std::to_string(Referee.player[0].points));
		Points2.setString("Punkty: " + std::to_string(Referee.player[1].points));
		WhiteSpeed.setString("White speed : " + std::to_string(balls[15].GetBody()->GetLinearVelocity().x));
		frameNumber++;
		if (fpsClock.getElapsedTime().asMilliseconds() > 1000) {
			char buf[64];
			sprintf_s(buf, "FPS: %d\n", frameNumber - lastFPSFrame);
			lastFPSDisplay = string(buf);
			lastFPSFrame = frameNumber;
			fpsClock.restart();
			fflush(stdout);

		
		}
		if (Referee.turn == 0) {
			turnRect.setPosition(387, 25);
		}
		else {
			turnRect.setPosition(1182, 25);
		}
		if (Referee.faul == true) {
			if (balls[15].getInfo() == Referee.ballNumber) {
				Referee.faul = false;
				balls[15].whiteReset(world);
				Referee.ballNumber = 0;
			}
			else if (abs(balls[15].GetBody()->GetLinearVelocity().x) < 0.1f && abs(balls[15].GetBody()->GetLinearVelocity().y) < 0.1f) {
				Referee.faul = false;
				balls[15].whiteReset(world);
				Referee.changeTurn();
			}
	}
		if (Referee.ballNumber > 0 && Referee.ballNumber<16) {
				for (int i = 0; i < 15; i++) {
					if (balls[i].getInfo() == Referee.ballNumber)
						balls[i].scoredPosition(world);
				}
			Referee.ballNumber = 0;
		}


		sf::Event events;
		while (app.pollEvent(events)) {
			switch (events.type) {
			case sf::Event::MouseButtonPressed:
				if (isMouseOver(balls[15].getSprite(), app) && (abs(balls[15].GetBody()->GetLinearVelocity().x) < 0.1f && abs(balls[15].GetBody()->GetLinearVelocity().y) < 0.1f)) {
					moveVector1 = static_cast<sf::Vector2i>(balls[15].GetSprite().getPosition());
					shoot = true;
				}
				if (menuDisplay == true) {
					if(isMouseOver(newGameButton, app)) {
						Referee.reset();
						resetGame(balls, world);
						menuDisplay = false;
					}
					if (isMouseOver(player1Name, app)) {
						tabIndex = 1;
					}
					if (isMouseOver(player2Name, app)) {
						tabIndex = 2;
					}
				}
				break;
			case sf::Event::MouseButtonReleased:
				if (shoot == true) {
					moveVector2 = sf::Mouse::getPosition(app);
					balls[15].move(moveVector1.x - moveVector2.x, moveVector1.y - moveVector2.y);
					turn = Referee.turn;
					points = Referee.player[Referee.turn].points;
					Referee.turnBlockade = false;
					Referee.noHit = true;

				}
				shoot = false;
				
				break;
			case sf::Event::Closed:
				app.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
					Referee.reset();
					resetGame(balls, world);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && menuDisplay==true){
				tabIndex++;
				if (menuDisplay == true&&tabIndex==3) {
						resetGame(balls, world);
						menuDisplay = false;
					}
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					app.close();
				 else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
					 balls[15].move(50,50);
				 }
				 break;

			case sf::Event::TextEntered:
				 {
				if ((events.text.unicode >= 65 && events.text.unicode <= 90)|| (events.text.unicode >= 97 && events.text.unicode <= 122)) {
					if (tabIndex == 1) {
						Referee.player[0].name += (events.text.unicode);
					}
					if (tabIndex == 2) {
						Referee.player[1].name += (events.text.unicode);
					}
				}
				
					}
			}
		}
		if (menuDisplay == true) {
			if (isMouseOver(newGameButton, app)) {
				newGameButton.setFillColor(sf::Color(8, 216, 0));
			}
			else {
				newGameButton.setFillColor(sf::Color(7, 192, 0));
			}
		}
		for (int i = 0; i < count; i++) {
			balls[i].update();
		}

		app.clear();
		app.draw(background);
		app.draw(Avatar1);
		app.draw(Avatar2);
		app.draw(Text);
		app.draw(mouseY);
		app.draw(mouseX);
		app.draw(WhiteSpeed);
		app.draw(Player1);
		app.draw(Player2);
		app.draw(Points1);
		app.draw(Points2);
		app.draw(turnRect);
		if (Referee.opentable == true) {
			app.draw(openTable);
		}
		if (Referee.faul == true) {
			app.draw(Faul);
		}
		if (shoot == true) {
			float angle = atan2(position.y - balls[15].GetSprite().getPosition().y, position.x - balls[15].GetSprite().getPosition().x);
			stick.setRotation(angle*180/ 3.14159265359-90.0);
			stick.setPosition(sf::Vector2f(position.x, position.y));
			app.draw(stick);
			lines[0].position = balls[15].GetSprite().getPosition();
			b2RayCastInput input;
			b2Vec2 p1, p2;
			p1.x = lines[0].position.x/PPM;
			p1.y = lines[0].position.y / PPM;
			p2.x = (lines[0].position.x + 20 * (lines[0].position.x - position.x)) / PPM;
			p2.y = (lines[0].position.y + 20 * (lines[0].position.y - position.y)) / PPM;
			input.p1 = p1;
			input.p2 = p2;
			input.maxFraction = 10;
			//check every fixture of every body to find closest
			float closestFraction = 10; //start with end of line as p2
			b2Vec2 intersectionNormal(0, 0);
			for (b2Body* b = world.GetBodyList(); b; b = b->GetNext()) {
				for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) {

					b2RayCastOutput output;
					if (!f->RayCast(&output, input,0))
						continue;
					if (output.fraction < closestFraction) {
 						closestFraction = output.fraction;
						intersectionNormal = output.normal;
					}
				}
			}
			b2Vec2 intersectionPoint = p1 + closestFraction * (p2 - p1);
			int x = lines[0].position.x - intersectionPoint.x*PPM;
			int y = lines[0].position.y - intersectionPoint.y*PPM;
			float distance = sqrt(x*x + y*y);
			intersectionPoint = p1 + (1 - 20.0f / distance)*closestFraction * (p2 - p1);
			b2Vec2 normalEnd = intersectionPoint + intersectionNormal;
			aimCircle.setPosition(sf::Vector2f(intersectionPoint.x*PPM, intersectionPoint.y*PPM));
			intersectionPoint = p1 + (1 - 40.0f / distance)*closestFraction * (p2 - p1);
			lines[1].position = sf::Vector2f(intersectionPoint.x*PPM, intersectionPoint.y*PPM);
			app.draw(aimCircle);
			app.draw(lines);
		}
		for (int i = 0; i < count; i++) {
			app.draw(balls[i].GetSprite());
		}

		if (menuDisplay == true) {
			Player1.setString(Referee.player[0].name);
			Player2.setString(Referee.player[1].name);
			Player2.setPosition(1400 - Player2.getString().getSize() * 22, 40);
			app.draw(menuSprite);
			app.draw(player1Name);
			app.draw(player2Name);
			app.draw(player1NameText);
			app.draw(player2NameText);
			app.draw(newGameButton);
			app.draw(newGameText);
		}
		for (int i = 0; i < 2; i++) {
			if (Referee.player[i].win == true) {
				Win.setString(Referee.player[i].name+ " wygral");
				resetGame(balls, world);
				app.draw(Win);

			}
		}
		app.display();
	}

	return 0;
}

float framesPS(sf::Clock & clock)
{
	bool sec = false;
	int count = 0;
	count++;
	if (clock.getElapsedTime().asMilliseconds() >= 1000)
	{
		sec = true;
		clock.restart();
		count = 0;
	}
	float t = (float)clock.getElapsedTime().asMilliseconds();
	float ret = 0;
	if (t == 0) return 0;
	else { ret = count / (float)clock.getElapsedTime().asMilliseconds(); }
	if (sec) {
		char buf[64];
		sprintf_s(buf, "%f\n", ret);
		fflush(stdout);
	}
	return ret;
}