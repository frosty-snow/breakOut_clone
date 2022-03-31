#include "GameDriver.h"

GameDriver::GameDriver()
{
	init_Menu();
}

void GameDriver::init_Menu()
{
	// Set default starting state
	state.screen = menuTitle;
	
	// Create the window of the application
	window.create(sf::VideoMode(), "Breakout Clone", sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);

	// Get width and height of current screen application is running on
	windowWidth = window.getSize().x;
	windowHeight = window.getSize().y;

	// TODO: choose sound to use and add it here
	/*
	if(!ballSoundBuffer.loadFromFile("filepath")) return false;
	ballSound(ballSoundBuffer);
	*/

	// Load font
	//if (!
	font.loadFromFile("./resources/sansation.ttf");
	//) return false;

	// Setup menu text
	menuText.setFont(font);
	menuText.setCharacterSize(40);
	menuText.setPosition(windowWidth / 2, windowHeight / 4);
	menuText.setFillColor(sf::Color::White);
	menuText.setString("BreakOut Clone - BiFrost Games");
}

void GameDriver::init_Game()
{
	// Setup paddle
	sf::Vector2f paddleSize(paddleWidth, paddleHeight);
	paddle.setSize(paddleSize - sf::Vector2f(3, 3));
	paddle.setOutlineThickness(3);
	paddle.setOutlineColor(sf::Color::Black);
	paddle.setFillColor(paddleColor);
	paddle.setOrigin(paddleSize / 2.f);
	paddle.setPosition(windowWidth / 2, windowHeight - paddleHeight);

	// Setup ball
	ball.setRadius(ballRadius - 3);
	ball.setOutlineThickness(3);
	ball.setOutlineColor(sf::Color::Black);
	ball.setFillColor(ballColor);
	ball.setOrigin(ballRadius / 2, ballRadius / 2);
	ball.setPosition(paddle.getPosition().x, paddle.getPosition().y - paddleHeight / 2 - ball.getRadius() - 1);

	// Setup bricks
	buildLayout();
}

void GameDriver::buildLayout()
{
	// Initial implementation will be simply grid - TODO: variable design implementation
	int numRows = windowWidth / brickWidth - 2;
	int numCols = windowHeight / 3 / brickHeight - 2;

	// Create all the bricks
	for (int row = 1; row <= numRows; row++)
	{
		std::vector<sf::RectangleShape> brickLayer;
		for (int col = 1; col <= numCols; col++)
		{
			sf::RectangleShape brick;
			sf::Vector2f brickSize(brickWidth, brickHeight);
			brick.setSize(brickSize - sf::Vector2f(3, 3));
			brick.setOutlineThickness(3);
			brick.setOutlineColor(sf::Color::Black);
			brick.setFillColor(brickColor);
			brick.setOrigin(brickSize / 2.f);
			brick.setPosition((brickWidth + 5) * row, (brickHeight + 5) * col);
			brickLayer.push_back(brick);
		}
		bricks.push_back(brickLayer);
	}
}

void GameDriver::drawScreen()
{
	window.clear(sf::Color(50, 200, 50));

	if (state.screen == menuTitle) {
		window.draw(menuText);
	} else if (state.screen == playing) {
		window.draw(paddle);
		window.draw(ball);
		for (auto brickLayer : bricks)
		{
			for (auto brick : brickLayer)
			{
				window.draw(brick);
			}
		}
	}

	window.display();
}