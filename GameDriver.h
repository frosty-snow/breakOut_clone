#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <math.h>

enum gameScreen
{
	menuTitle, menuMain, menuWin, menuLose, menuPause, playing
};

struct ObjectVector
{
	float x;
	float y;
};

struct State
{
	gameScreen screen;  // default value set on initialization of the game;
	std::vector<float> prevBallPosition; 
	ObjectVector ballVector; 

	ObjectVector prevPaddlePosition;
	ObjectVector paddleVector; // don't need this until i implement variable speeds


	// Values can be adjusted here for testing to find good game speed
	const float ballSpeed = 200;
	const float paddleSpeed = 400;

};

class GameDriver
{
public:
	State state;
public:
	// Constants defined here
	const float pi = 3.14159;

	const float paddleWidth = 150;
	const float paddleHeight = 25;

	const float ballRadius = 10;
	const float brickWidth = 100;
	const float brickHeight = 30;

	// Shapes defined here
	sf::RectangleShape paddle;
	sf::CircleShape ball;
	std::vector<std::vector<sf::RectangleShape>> bricks;

	// Shape colors
	sf::Color ballColor = sf::Color::White;
	sf::Color paddleColor = sf::Color(10, 20, 0);
	sf::Color brickColor = sf::Color(10, 200, 200);

	// Window
	sf::RenderWindow window;

	// Window measurements - updated during init_game
	int windowWidth = 0;
	int windowHeight = 0;

	// Audio
	sf::SoundBuffer ballSoundBuffer;
	sf::Sound ballSound;

	// Font
	sf::Font font;

	// Menu Text
	sf::Text menuText;

public:
	GameDriver();
	~GameDriver();

	void init_Menu();
	void init_Game();
	void buildLayout();
	void renderScreen();
	void simulatePhysics(float deltaTime);

	// User input
	void handleInput(float deltaTime);

};

