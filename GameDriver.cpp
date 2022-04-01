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
	ballSoundBuffer.loadFromFile("./resources/ball.wav");
	//return false;
	ballSound.setBuffer(ballSoundBuffer);

	// Load font
	//if (!
	font.loadFromFile("./resources/sansation.ttf");
	//) return false;

	// Setup menu text
	menuText.setFont(font);
	menuText.setCharacterSize(60);
	menuText.setPosition(windowWidth / 8, windowHeight / 4);
	menuText.setFillColor(sf::Color::Black);
	menuText.setString("BreakOut Clone - ByFrost Games");
}

void GameDriver::init_Game()
{
	reset_Game();

	// Setup paddle size, color, and position
	sf::Vector2f paddleSize(paddleWidth, paddleHeight);
	paddle.setSize(paddleSize - sf::Vector2f(3, 3));
	paddle.setOutlineThickness(3);
	paddle.setOutlineColor(sf::Color::Black);
	paddle.setFillColor(paddleColor);
	paddle.setOrigin(paddleSize / 2.f);
	paddle.setPosition(windowWidth / 2, windowHeight - 2 * paddleHeight);

	// Setup ball color, size, and position
	ball.setRadius(ballRadius - 3);
	ball.setOutlineThickness(3);
	ball.setOutlineColor(sf::Color::Black);
	ball.setFillColor(ballColor);
	ball.setOrigin(ballRadius / 2, ballRadius / 2);
	ball.setPosition(paddle.getPosition().x, paddle.getPosition().y - paddleHeight - ball.getRadius()); // should be a little above the paddle

	// Setup ball state
	state.prevBallPosition = { ball.getPosition().x, ball.getPosition().y };
	state.ballVector = { (float)cos((double)225 * (pi / 180)), (float)sin((double)225 * (pi / 180)) };

	// Setup paddle state
	state.prevPaddlePosition = { paddle.getPosition().x, paddle.getPosition().y };
	state.paddleVector = { paddle.getPosition().x, paddle.getPosition().y }; // paddle starts stationary with no vector - updates on user input

	// Create brick 2D vectors for render on screen
	buildLayout();
}

void GameDriver::reset_Game()
{
	bricks.clear();
}

void GameDriver::buildLayout()
{
	// Initial implementation will be simply grid - TODO: variable design implementation
	int numRows = windowWidth / brickWidth - 2;
	int numCols = windowHeight / 3 / brickHeight - 2;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(0, 255);

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
			brick.setFillColor(sf::Color(distr(gen), distr(gen), distr(gen)));
			brick.setOrigin(brickSize / 2.f);
			brick.setPosition((brickWidth + 5) * row, (brickHeight + 5) * col);
			brickLayer.push_back(brick);
		}
		bricks.push_back(brickLayer);
	}
}

void GameDriver::renderScreen()
{
	window.clear(windowColor);

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
		menuText.setString("Game PAUSED!\n Press P to resume or ESCAPE to exit the game.");
		window.draw(menuText);
	}
	else if (state.screen == menuWin)
	{
		menuText.setString("You WIN!\n Press SPACE to play again or ESCAPE to exit the game.");
		window.draw(menuText);
	}
	else if (state.screen == menuLose)
	{
		menuText.setString("You LOSE!\n Press SPACE to play again or ESCAPE to exit the game.");
		window.draw(menuText);
	}

	window.display();
}

bool GameDriver::checkIntersection(sf::CircleShape ball, sf::RectangleShape brick)
{
	float ballDistance_x = abs(ball.getPosition().x - brick.getPosition().x);
	float ballDistance_y = abs(ball.getPosition().y - brick.getPosition().y);

	if (ballDistance_x > (brickWidth / 2 + ballRadius)) return false;
	if (ballDistance_y > (brickHeight / 2 + ballRadius)) return false;

	if (ballDistance_x <= brickWidth / 2) return true;
	if (ballDistance_x <= brickHeight / 2) return true;
	float cornerDistance_sq = pow((ballDistance_x - brickWidth / 2), 2) + pow((ballDistance_x - brickHeight / 2), 2);

	return (cornerDistance_sq <= pow(ballRadius, 2));
}

bool GameDriver::checkIntersectionPaddle(sf::CircleShape ball)
{
	float ballDistance_x = abs(ball.getPosition().x - state.prevPaddlePosition.x);
	float ballDistance_y = abs(ball.getPosition().y - state.prevPaddlePosition.y);

	if (ballDistance_x > (paddleWidth / 2 + ballRadius)) return false;
	if (ballDistance_y > (paddleHeight / 2 + ballRadius)) return false;

	if (ballDistance_x <= paddleWidth / 2) return true;
	if (ballDistance_x <= paddleHeight / 2) return true;
	float cornerDistance_sq = pow((ballDistance_x - paddleWidth / 2), 2) + pow((ballDistance_x - paddleHeight / 2), 2);

	return (cornerDistance_sq <= pow(ballRadius, 2));
}

void GameDriver::simulatePhysics(float deltaTime) 
{
	state.prevBallPosition = { ball.getPosition().x, ball.getPosition().y }; // for future functionality with velocity
	ball.move(state.ballVector.x * state.ballSpeed * deltaTime, state.ballVector.y * state.ballSpeed * deltaTime);

	// Handle wall collisions
	if (ball.getPosition().x - ballRadius < 0)
	{
		// hitting left wall
		state.ballVector.x *= -1;
		ballSound.play();
		ball.setPosition(ball.getPosition().x + ballRadius + 0.5, ball.getPosition().y);
	}
	if (ball.getPosition().x + ballRadius > windowWidth)
	{
		// hitting right wall
		state.ballVector.x *= -1;
		ballSound.play();
		ball.setPosition(ball.getPosition().x - ballRadius - 0.5, ball.getPosition().y);
	}
	if (ball.getPosition().y - ballRadius < 0)
	{
		// hitting top wall
		state.ballVector.y *= -1;
		ballSound.play();
		ball.setPosition(ball.getPosition().x, ball.getPosition().y + ballRadius + 0.5);
	}
	if (ball.getPosition().y + ballRadius > windowHeight)
	{
		// hitting bottom wall for debugging
		/*state.ballVector.y *= -1;
		ballSound.play();
		ball.setPosition(ball.getPosition().x, ball.getPosition().y - ballRadius - 0.5);
		*/
		state.screen = menuLose;
	}

	/************************
	* So i can get the ball stuck inside the paddle when moving from the side into the ball and it then moves through the
	* middle and bounces out the other side - unsuccessful in addressing this so far - going to finish everything else
	* I can make it look good enough for a screen cap and just avoid showing that happening - enough to finish off
	* the resume and start job applying again.
	*/
	// Handle paddle collisions
	if (checkIntersectionPaddle(ball))
	{
		// hit off the top
		if (ball.getPosition().y + ballRadius > state.paddleVector.y - paddleHeight / 2 - 0.5)
		{
			state.ballVector.y *= -1;
			ball.setPosition(ball.getPosition().x, ball.getPosition().y - 0.5);
		} 
		// hit off the bottom (not possible but might as well include it)
		else if (ball.getPosition().y - ballRadius < state.paddleVector.y + paddleHeight / 2 + 0.5)
		{
			state.ballVector.y *= -1;
			ball.setPosition(ball.getPosition().x, ball.getPosition().y + 0.5);
		}
		// hit off the right side
		else if (ball.getPosition().x - ballRadius < state.paddleVector.x + paddleWidth / 2 + 1)
		{
			state.ballVector.y *= -1;
			state.ballVector.x = abs(state.ballVector.x); // always want ball to move off in the right direction
			ball.setPosition(state.paddleVector.x + paddleWidth / 2 + 1, ball.getPosition().y - ballRadius - 0.1);
		}
		// hit off the left side
		else if (ball.getPosition().x + ballRadius > state.paddleVector.x - paddleWidth / 2 - 1)
		{
			state.ballVector.y *= -1;
			state.ballVector.x = -1 * abs(state.ballVector.x); // always want ball to move off in the left direction
			ball.setPosition(state.paddleVector.x - paddleWidth / 2 - 1, ball.getPosition().y - ballRadius - 0.1);
		}

		ballSound.play();
	}

	// Handle brick collisions
	for (int row = bricks.size()-1; row >= 0; row--)
	{
		for (int col = 0; col < bricks[row].size(); col++)
		{
			if (checkIntersection(ball, bricks[row][col]))
			{
				// hit off the top
				if (ball.getPosition().y + ballRadius < bricks[row][col].getPosition().y - brickHeight / 4)
				{
					state.ballVector.y *= -1;
					ball.setPosition(ball.getPosition().x, ball.getPosition().y - 0.5);
				}
				// hit off the bottom (not possible but might as well include it)
				else if (ball.getPosition().y - ballRadius > bricks[row][col].getPosition().y + brickHeight / 4)
				{
					state.ballVector.y *= -1;
					ball.setPosition(ball.getPosition().x, ball.getPosition().y + 0.5);
				}
				// hit off the right side
				else if (ball.getPosition().x - ballRadius > bricks[row][col].getPosition().x + brickWidth / 4)
				{
					// TODO: add some randomness to the bounciness of the edge hits
					state.ballVector.y *= -1;
					state.ballVector.x = abs(state.ballVector.x);
					ball.setPosition(ball.getPosition().x + 0.5, ball.getPosition().y);
				}
				// hit off the left side
				else if (ball.getPosition().x + ballRadius < bricks[row][col].getPosition().x - brickWidth / 4)
				{
					state.ballVector.y *= -1;
					state.ballVector.x = -1 * abs(state.ballVector.x);
					ball.setPosition(ball.getPosition().x - 0.5, ball.getPosition().y);
				}

				ballSound.play();

				// Remove bricks - if end of a row - delete the row and break the inner for loop
				bricks[row].erase(bricks[row].begin() + col);
				--col;
				if (bricks[row].size() == 0)
				{
					bricks.erase(bricks.begin() + row);
					--row;
					break;
				}
			}
		}
	}
}

void GameDriver::handleInput(float deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && (paddle.getPosition().x - paddleWidth / 2 > 0.5))
	{
		// store the current position to use as prev (for future functionality with velocity)
		state.prevPaddlePosition = { paddle.getPosition().x, paddle.getPosition().y };
		// Move the paddle left horizontal
		paddle.move(-state.paddleSpeed * deltaTime, 0);
		state.paddleVector = { paddle.getPosition().x, paddle.getPosition().y };
		
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && (paddle.getPosition().x + paddleWidth / 2 < windowWidth - 0.5))
	{
		// store the current position to use as prev (for future functionality with velocity)
		state.prevPaddlePosition = { paddle.getPosition().x, paddle.getPosition().y };
		// Move the paddle right horizontal
		paddle.move(state.paddleSpeed * deltaTime, 0);
		state.paddleVector = { paddle.getPosition().x, paddle.getPosition().y };

	}
}