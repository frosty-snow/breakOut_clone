#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

enum gameScreen
{
	menuTitle, menuMain, menuWin, menuLose, menuPause, playing
};

struct State
{
	gameScreen screen;  // default value set on initialization of the game;
	std::vector<float> prevBallPosition; // this is saved to use against current position to calc ballVector
	std::vector<float> ballVector;

	std::vector<float> prevPaddlePosition;
	std::vector<float> paddleVector;

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
	const float paddleSpeed = 400;

	const float ballRadius = 10;
	const float brickWidth = 100;
	const float brickHeight = 30;
	const float ballSpeed = 200; // leave this for now
	float ballAngle = 0;

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
	void drawScreen();

};

