#include "PongGame.h"

PongGame::PongGame() :
		game_active(false), game_paused(false), board(NULL),
		left_paddle(NULL), right_paddle(NULL), ball(NULL),
		left_controller(NULL), right_controller(NULL),
		ball_timer(-1)
{
	Pong* pong = Pong::getInstance();
	width = pong->getViewWidth();
	height = pong->getViewHeight();
	
	reset();
}

PongGame::~PongGame()
{
	clear();
}

void PongGame::step()
{
	if (game_active && game_paused) return;

	if (left_paddle != NULL)
	{
		left_paddle -> step();
	}
	if (right_paddle != NULL)
	{
		right_paddle -> step();
	}
	if( ball != NULL )
	{
		ball -> step();
	}
	if (left_controller != NULL)
	{
		left_controller->step();
	}
	if (right_controller != NULL)
	{
		right_controller->step();
	}
	if (ball_timer == 0 && ball != NULL && ball->x_velocity == 0 && ball->y_velocity == 0)
	{
		serve();
	}
	if (ball_timer > -1)
	{
		ball_timer--;
	}
}

void PongGame::keyDownEvent(unsigned char key)
{
	// Forward event to paddle controllers (aka players)
	if (left_controller != NULL)
		left_controller->keyDown((char) key);
	if (right_controller != NULL)
		right_controller->keyDown((char) key);

	// Adjust ball speed
	if (ball != NULL && !game_paused)
	{
		if (key == '-' || key == '_')
		{
			ball->setSpeedModifier(ball->getSpeedModifier() - 0.25);
		}
		if (key == '=' || key == '+')
		{
			ball->setSpeedModifier(ball->getSpeedModifier() + 0.25);

			// Apply governor to ball speed. Too much, and we may lose it.
			if (ball->getSpeedModifier() > 2.0)
			{
				ball->setSpeedModifier(2.0);
			}
		}
	}
}

void PongGame::keyUpEvent(unsigned char key)
{
	if (left_controller != NULL)
		left_controller->keyUp((char) key);
	if (right_controller != NULL)
		right_controller->keyUp((char) key);
}

void PongGame::keySpecialDownEvent(int key)
{
	if (left_controller != NULL)
		left_controller->keySpecialDown(key);
	if (right_controller != NULL)
		right_controller->keySpecialDown(key);
}

void PongGame::keySpecialUpEvent(int key)
{
	if (left_controller != NULL)
		left_controller->keySpecialUp(key);
	if (right_controller != NULL)
		right_controller->keySpecialUp(key);
}

void PongGame::startGame(bool left_ai, bool right_ai)
{
	if (game_active) return;

	this -> left_ai = left_ai;
	this -> right_ai = right_ai;

	reset();

	game_active = true;
	game_paused = false;

	setBallSpeed( 5 );
	hit_count = 0;
	ball = new (nothrow) Ball(this, 0, 0, 20, 0, 0);
	resetBall();
	Pong::getInstance() -> drawObject( ball, 1 );

	if (left_ai)
	{
		left_controller = new AIController(left_paddle, ball);
	}
	else
	{
		left_controller = new PlayerController(left_paddle, false);
	}

	if (right_ai)
	{
		right_controller = new AIController(right_paddle, ball);
	}
	else
	{
		right_controller = new PlayerController(right_paddle, true);
	}
	
	if (left_ai && right_ai)
	{
		board->setLeftText("");
		board->setRightText("");
	}

	getLeftPaddle() -> change_max_paddle_speed( 2, 3 );
	getRightPaddle() -> change_max_paddle_speed( 2, 3 );

	updateDifficulty();
}

void PongGame::quitGame()
{
	if (!game_active) return;
	game_active = false;
	Pong::getInstance() -> stopDrawingObject( ball );
	if (right_controller != NULL) delete right_controller;
	if (left_controller != NULL) delete left_controller;
	if (ball != NULL) delete ball;

	right_controller = NULL;
	left_controller = NULL;
	ball = NULL;
}

void PongGame::pauseGame()
{
	game_paused = true;
}

void PongGame::resumeGame()
{
	game_paused = false;
}

void PongGame::clear()
{

	if (board != NULL)
	{
		Pong::getInstance()->stopDrawingObject(board);
		delete board;
	}
	if (left_paddle != NULL)
	{
		Pong::getInstance()->stopDrawingObject(left_paddle);
		delete left_paddle;
	}
	if (right_paddle != NULL)
	{
		Pong::getInstance()->stopDrawingObject(right_paddle);
		delete right_paddle;
	}
	if (ball != NULL)
	{
		Pong::getInstance()->stopDrawingObject(ball);
		delete ball;
	}
	if (left_controller != NULL)
	{
		delete left_controller;
	}
	if (right_controller != NULL)
	{
		delete right_controller;
	}
}

void PongGame::reset()
{
	int u = Pong::unit;
	clear();

	// Reset game variables
	left_score = 0;
	right_score = 0;
	left_paddle_size = 8;
	right_paddle_size = 8;

	// Construct game elements
	board = new (nothrow) Board(0, 0, width, height, u);
	left_paddle = new (nothrow) Paddle(this,
		(int) (u * 2.5),
		height/2,
		u,
		left_paddle_size * u,
		board->getX() + board->getWidth() / 4,
		board->getX(),
		board->getY() + board->getHeight(),
		board->getY());
	right_paddle = new (nothrow) Paddle(this,
		width - (int) (u * 2.5),
		height/2,
		u,
		right_paddle_size * u,
		board->getX() + board->getWidth(),
		board->getX() + board->getWidth() / 4 * 3,
		board->getY() + board->getHeight(),
		board->getY());

	board->setRightText(to_string(left_score));
	board->setLeftText(to_string(left_score));

	// Register game elements with drawing system
	Pong::getInstance()->drawObject(board, 0);
	Pong::getInstance()->drawObject(left_paddle, 2);
	Pong::getInstance()->drawObject(right_paddle, 2);
}

Board* PongGame::getBoard()
{
	return board;
}

Paddle* PongGame::getLeftPaddle()
{
	return left_paddle;
}

Paddle* PongGame::getRightPaddle()
{
	return right_paddle;
}

Ball* PongGame::getBall()
{
	return ball;
}

bool PongGame::isPaused()
{
	return game_paused;
}

bool PongGame::isRunning()
{
	return game_active;
}

void PongGame::scoreLeft()
{
	// If a player is playing, then keep track of score.
	if (!left_ai || !right_ai)
	{
		left_score++;
		board->setLeftText(to_string(left_score));
	}

	// Reset variables and paddle status
	hit_count = 0;
	right_paddle_size = 8;
	left_paddle_size = 8;
	right_paddle->setHeight(right_paddle_size * Pong::unit);
	left_paddle->setHeight(left_paddle_size * Pong::unit);

	// Since paddles could be outside border, move 0 px to fix position
	right_paddle->verticalMotion(0);
	left_paddle->verticalMotion(0);

	// Check if we have a winner
	if ( left_score == 10 )
	{
		leftWins();
		quitGame();
	}
	else
	{
		updateDifficulty();
		resetBall();
	}
}

void PongGame::scoreRight()
{
	// If a player is playing, then keep track of score.
	if (!left_ai || !right_ai)
	{
		right_score++;
		board->setRightText(to_string(right_score));
	}

	// Reset variables and paddle status
	hit_count = 0;
	right_paddle_size = 8;
	left_paddle_size = 8;
	right_paddle->setHeight(right_paddle_size * Pong::unit);
	left_paddle->setHeight(left_paddle_size * Pong::unit);

	// Since paddles could be outside border, move 0 px to fix position
	right_paddle->verticalMotion(0);
	left_paddle->verticalMotion(0);

	// Check if we have a winner
	if ( right_score == 10 )
	{
		rightWins();
		quitGame();
	}
	else
	{
		updateDifficulty();
		resetBall();
	}
}

void PongGame::leftWins()
{
	board -> setCenterText("Left side wins!");
}

void PongGame::rightWins( )
{
	board -> setCenterText("Right side wins!");
}

void PongGame::ballHit(bool right)
{
	hit_count++;
	if (hit_count > 16 - (left_score + right_score) / 2)
	{
		if (right)
		{
			right_paddle_size--;
		}
		else
		{
			left_paddle_size--;
		}
	}
	if (right_paddle_size < 3)
	{
		right_paddle_size = 3;
	}
	if (left_paddle_size < 3)
	{
		left_paddle_size = 3;
	}
	right_paddle->setHeight(right_paddle_size * Pong::unit);
	left_paddle->setHeight(left_paddle_size * Pong::unit);

	if (right) {
		right_controller->ballHit();
	}
	else {
		left_controller->ballHit();
	}
}

void PongGame::updateDifficulty()
{
	if (left_ai)
	{
		left_paddle->change_max_paddle_speed(2, 3 + (right_score - left_score) * 0.125);
	}
	if (right_ai)
	{
		right_paddle->change_max_paddle_speed(2, 3 + (left_score - right_score) * 0.125);
	}
}

void PongGame::resetBall( )
{
	ball -> center_x = board -> getWidth() / 2;
	ball -> center_y = board -> getHeight() / 2;
	ball -> x_velocity = 0;
	ball -> y_velocity = 0;
	ball_timer = 60;
}

void PongGame::serve()
{
	if( (left_score + right_score) % 2 )
	{
		ball -> x_velocity = -ball_speed;
	}
	else
	{
		ball -> x_velocity = ball_speed;
	}
	ball -> y_velocity = (rand() % 11 - 5) / 2.0;
}

void PongGame::setBallSpeed( double speed )
{
	ball_speed = speed;
}
