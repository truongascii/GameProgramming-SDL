#include "game.h"

Game::Game() {
	displayWindow = 0;
	renderr = 0;
}

Game::~Game() {

}

bool Game::Init() {
	//Initializer SDL video
	SDL_Init(SDL_INIT_VIDEO);

	//Creates wgame indow
	displayWindow = SDL_CreateWindow("Pong",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (!displayWindow) {
		return false;
	}

	//creates renderer
	renderr = SDL_CreateRenderer(displayWindow, -1, SDL_RENDERER_ACCELERATED);
	if (!renderr) {
		return false;
	}

	//animation timing
	lasttick = SDL_GetTicks();
	fpstick = lasttick;
	fps = 0;
	framecount = 0;

	return true;
}

void Game::Clean() {
	//SDL clean up (renderer and game window)
	SDL_DestroyRenderer(renderr);
	SDL_DestroyWindow(displayWindow);
}

void Game::Run() {
	table = new Table(renderr); //creates play table
	ball = new Ball(renderr); //creates ball

	//creates 2 player and add them to vector of players
	players.push_back(new Player(renderr, "paddleBlu.png"));
	players.push_back(new Player(renderr, "paddleRed.png"));

	NewGame();

	bool isRunning = true;
	SDL_Event event;

	//while game is running
	while (isRunning) {
		//event handler
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				isRunning = false;
			}
		}

		//change and fps
		unsigned int curtick = SDL_GetTicks();
		float change = (curtick - lasttick) / 1000.0f;
		if (curtick - fpstick >= FPS_DELAY) {
			fps = framecount * (1000.0f / (curtick - fpstick));
			fpstick = curtick;
			framecount = 0;
		}
		else {
			framecount++;
		}
		lasttick = curtick;

		//updates and renders the game
		Update(change);
		Render(change);
	}
	//delete game objects when game is over
	delete table;
	delete ball;

	//remove all the player objects
	for (unsigned int i = 0; i<players.size(); i++) {
		delete players[i];
	}
	players.clear();

	Clean();

	SDL_Quit();
}

void Game::NewGame() {
	//centers the player paddle vertically
	SetPlayerY(0, table->height / 2 - players[0]->height / 2);
	SetPlayerY(1, table->height / 2 - players[1]->height / 2);

	//positions the player paddles 
	players[0]->x = 12;
	players[1]->x = table->width - players[1]->width - 12;

	//centers the ball horizontally and vertically
	ball->x = table->width / 2 - ball->width / 2;
	ball->y = table->height / 2 - ball->height / 2;

	//initializes the ball's direction
	ball->SetDirection(1, -1);
}

void Game::Update(float change) {
	//gets mouse position
	int mx, my;
	SDL_GetMouseState(&mx, &my);

	//move the player's paddle with mouse
	SetPlayerY(0, my - players[0]->height / 2.0f);

	//checks for collision and runs AI
	CheckCollision();
	AI(change);

	//updates the ball and tale
	table->Update(change);
	ball->Update(change);

	//updates the player's paddle
	for (unsigned int i = 0; i<players.size(); i++) {
		players[i]->Update(change);
	}
}

void Game::SetPlayerY(int index, float y) {
	float newy;
	if (y < table->top) {
		// top boundary
		newy = table->top;
	}
	else if (y + players[index]->height > table->bottom) {
		//bottom boundary
		newy = table->bottom - players[index]->height;
	}
	else {
		//in boundary
		newy = y;
	}

	//sets players y position
	players[index]->y = newy;
}

void Game::CheckCollision() {
	//checks collision with top or bottom
	if (ball->y < table->top) {
		ball->y = table->top;
		ball->SetDirection(ball->direction_x, -1 * ball->direction_y);
	}
	else if (ball->y + ball->height > table->bottom) {
		ball->y = table->bottom - ball->height;
		ball->SetDirection(ball->direction_x, -1 * ball->direction_y);
	}

	//checks collision with left player paddle
	if (ball->Collision(players[0])) {
		ball->x = players[0]->x + players[0]->width;
		ball->SetDirection(1, GetReflection(0, (ball->y + ball->height / 2) - players[0]->y));
	}

	//checks collision with right player paddle
	if (ball->Collision(players[1])) {
		ball->x = players[1]->x - ball->width;
		ball->SetDirection(-1, GetReflection(1, (ball->y + ball->height / 2) - players[1]->y));
	}

	//checks if ball went out of screen
	if (ball->x + ball->width <= table->x) {
		//player2 scored & resetball
		ball->x = table->width / 2 - ball->width / 2 + 250;
		ball->y = table->height / 2 - ball->height / 2;

		ball->SetDirection(-1, -1);
	}
	else if (ball->x >= table->x + table->width) {
		//player1 scored & resetball

		ball->x = table->width / 2 - ball->width / 2 - 250;
		ball->y = table->height / 2 - ball->height / 2;

		ball->SetDirection(1, -1);
	}
}

float Game::GetReflection(int index, float hity) {
	// Make sure the hity variable is within the height of the paddle
	if (hity < 0) {
		hity = 0;
	}
	else if (hity > players[index]->height) {
		hity = players[index]->height;
	}

	// Everything above the center of the paddle is reflected upward
	// while everything below the center is reflected downward
	hity -= players[index]->height / 2.0f;

	// Scale the reflection, making it fall in the range -2.0f to 2.0f
	return 2.0f * (hity / (players[index]->height / 2.0f));
}

void Game::AI(float change) {
	// Calculate howmuch AI paddle can move in this frame
	float dy = AI_SPEED * change;

	if (ball->x > table->width / 2 && ball->direction_x > 0) {
		// Lag behind the ball, when ball is in sight
		if (players[1]->y + players[1]->height / 2 < ball->y + ball->height / 2 - dy) {
			SetPlayerY(1, players[1]->y + dy);
		}
		else if (players[1]->y + players[1]->height / 2 > ball->y + ball->height / 2 + dy) {
			SetPlayerY(1, players[1]->y - dy);
		}
	}
	else {
		// Move to the center, when ball is far away
		if (players[1]->y + players[1]->height / 2 < table->height / 2 - dy) {
			SetPlayerY(1, players[1]->y + dy);
		}
		else if (players[1]->y + players[1]->height / 2 > table->height / 2 + dy) {
			SetPlayerY(1, players[1]->y - dy);
		}
	}
}

void Game::Render(float change) {
	// Clear the screen to cyan
	SDL_SetRenderDrawColor(renderr, 0, 200, 255, 255);
	SDL_RenderClear(renderr);

	//renders the table and ball
	table->Render(change);
	ball->Render(change);

	//renders the player paddle
	for (unsigned int i = 0; i<players.size(); i++) {
		players[i]->Render(change);
	}

	//updates the screen
	SDL_RenderPresent(renderr);
}