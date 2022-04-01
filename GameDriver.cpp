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

	//if(!
	ballSoundBuffer.loadFromFile("filepath");
	//return false;
	ballSound.setBuffer(ballSoundBuffer);

	// Load font
	//if (!
	font.loadFromFile("./resources/sansation.ttf");
	//) return false;

	// Setup menu text
	menuText.setFont(font);
	menuText.setCharacterSize(40);
	menuText.setPosition(windowWidth / 3, windowHeight / 4);
	menuText.setFillColor(sf::Color::White);
	menuText.setString("BreakOut Clone - BiFrost Games");
}

void GameDriver::init_Game()
{
	// Setup paddle size, color, and position
	sf::Vector2f paddleSize(paddleWidth, paddleHeight);
	paddle.setSize(paddleSize - sf::Vector2f(3, 3));
	paddle.setOutlineThickness(3);
	paddle.setOutlineColor(sf::Color::Black);
	paddle.setFillColor(paddleColor);
	paddle.setOrigin(paddleSize / 2.f);
	paddle.setPosition(windowWidth / 2, windowHeight - paddleHeight);

	// Setup ball color, size, and position
	ball.setRadius(ballRadius - 3);
	ball.setOutlineThickness(3);
	ball.setOutlineColor(sf::Color::Black);
	ball.setFillColor(ballColor);
	ball.setOrigin(ballRadius / 2, ballRadius / 2);
	ball.setPosition(paddle.getPosition().x, paddle.getPosition().y - paddleHeight / 2 - ball.getRadius() - 1);

	// Setup ball state
	state.prevBallPosition = { ball.getPosition().x, ball.getPosition().y };
	state.ballVector = { (float)cos((double)225 * (pi / 180)), (float)sin((double)225 * (pi / 180)) };

	// Setup paddle state
	state.prevPaddlePosition = { paddle.getPosition().x, paddle.getPosition().y };
	state.paddleVector = { paddle.getPosition().x, paddle.getPosition().y }; // paddle starts stationary with no vector - updates on user input

	// Create brick 2D vectors for render on screen
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

void GameDriver::renderScreen()
{
	window.clear(sf::Color(50, 200, 50));

	if (state.screen == menuTitle) 
	{
		window.draw(menuText);
	} 
	else if (state.screen == playing) 
	{
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
	else if (state.screen == menuMain) 
	{
		// use this section to draw the main menu - maybe leaderboards?
	}
	else if (state.screen == menuPause) {
		// use this section to draw the pause screen
	}
	else if (state.screen == menuWin)
	{
		// use this section to draw the win screen
	}
	else if (state.screen == menuLose)
	{
		// use this section to draw to the lose screen
	}

	window.display();
}

void GameDriver::simulatePhysics(float deltaTime) 
{
	state.prevBallPosition = { ball.getPosition().x, ball.getPosition().y };
	ball.move(state.ballVector.x * state.ballSpeed * deltaTime, state.ballVector.y * state.ballSpeed * deltaTime);

	// Handle wall collisions
	if (ball.getPosition().x - ballRadius < 0)
	{
		// hitting left wall
		state.ballVector.x *= -1;
		ballSound.play();
	}
	if (ball.getPosition().x + ballRadius > windowWidth)
	{
		// hitting right wall
		state.ballVector.x *= -1;
		ballSound.play();
	}
	if (ball.getPosition().y - ballRadius < 0)
	{
		// hitting top wall
		state.ballVector.y *= -1;
		ballSound.play();
	}
	if (ball.getPosition().y + ballRadius > windowHeight)
	{
		// hitting bottom wall
		state.ballVector.y *= -1;
		ballSound.play();
		/* Once game is ready this will be a losing action
		* state.screen = menuLose;
		*/
	}

	// Handle paddle collisions
	if (state.ballVector.x - ballRadius >= state.paddleVector.x - paddleWidth / 2 &&
		state.ballVector.x + ballRadius <= state.paddleVector.x + paddleWidth / 2 &&
		state.ballVector.y + ballRadius > state.paddleVector.y - paddleHeight / 2 &&
		state.ballVector.y - ballRadius < state.paddleVector.y + paddleHeight / 2)
	{
		state.ballVector.y *= -1;
		ballSound.play();
	}
}

void GameDriver::handleInput(float deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && (paddle.getPosition().x - paddleWidth / 2 > 0.5))
	{
		// store the current position to use as prev in the next loop
		state.prevPaddlePosition = { paddle.getPosition().x, paddle.getPosition().y };
		// Move the paddle left horizontal
		paddle.move(-state.paddleSpeed * deltaTime, 0);
		 
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && (paddle.getPosition().x + paddleWidth / 2 < windowWidth - 0.5))
	{
		// store the current position to use as prev in the next loop
		state.prevPaddlePosition = { paddle.getPosition().x, paddle.getPosition().y };
		// Move the paddle right horizontal
		paddle.move(state.paddleSpeed * deltaTime, 0);
		
	}
}