#include "Game.h"

Game::Game() {
	Init();

	done = false;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;

	gravityX = 0.0f;
	gravityY = -9.81f;

	bulletIndex = 0;
	shootTimer = 0.1f;

	score = 0;
	life = 5;

	levelTexture = LoadTexture("arne_sprites.png");
	chracterTexture = LoadTexture("characters_3.png");
	fontSheetTexture = LoadTexture("pixel_font.png");

	buildLevel();
	
	player = new Player(chracterTexture);
	player->yPos = -0.5f;
	player->width = 0.25f;
	player->height = 0.25f;
	player->frictionX = 3.0f;
	gameObjs.push_back(player);
	
}
//destructor
Game::~Game() {

	SDL_Quit();
}
//initializer
void Game::Init() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Wizard", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}

void Game::Update(float elapsed) {
	if (enemySpawnTimer > 1.0f && enemyIndex < MAX_ENEMIES) {
		SpriteSheet enemySprite = SpriteSheet(chracterTexture, 8, 4, 4);
		enemies[enemyIndex].sprite = enemySprite;
		enemies[enemyIndex].yPos = 0.85f;
		enemies[enemyIndex].xPos = 0.65f;
		enemies[enemyIndex].width = 0.25f;
		enemies[enemyIndex].height = 0.25f;
		enemies[enemyIndex].accelerationX = -2.0f;
		gameObjs.push_back(&enemies[enemyIndex]);
		enemyIndex++;
		enemySpawnTimer = 0.0f;
	}

	for (size_t i = 0; i < gameObjs.size(); i++) {
		gameObjs[i]->Update(elapsed);
	}

	for (int i = 0; i < MAX_BULLETS; i++) {
		bullets[i].Update(elapsed);
	}

	shootTimer += elapsed;
	enemySpawnTimer += elapsed * 0.5; //spawm faster as time passes
}

void Game::FixedUpdate() {

	for (size_t i = 0; i < gameObjs.size(); i++) {
		gameObjs[i]->FixedUpdate();

		if (gameObjs[i]->collidedBottom) {
			gameObjs[i]->offGround = false;
			gameObjs[i]->velocityY = 0.0f;
		}
		if (gameObjs[i]->collidedTop)
			gameObjs[i]->velocityY = 0.0f;
		if (gameObjs[i]->collidedLeft)
			gameObjs[i]->velocityX = 0.0f;
		if (gameObjs[i]->collidedRight)
			gameObjs[i]->velocityX = 0.0f;

		gameObjs[i]->collidedBottom = false;
		gameObjs[i]->collidedTop = false;
		gameObjs[i]->collidedLeft = false;
		gameObjs[i]->collidedRight = false;

		if (!gameObjs[i]->isStatic) {
			gameObjs[i]->velocityX += gravityX * FIXED_TIMESTEP;
			gameObjs[i]->velocityY += gravityY * FIXED_TIMESTEP;
		}

		gameObjs[i]->velocityX = lerp(gameObjs[i]->velocityX, 0.0f, FIXED_TIMESTEP * gameObjs[i]->frictionX);
		gameObjs[i]->velocityY = lerp(gameObjs[i]->velocityY, 0.0f, FIXED_TIMESTEP * gameObjs[i]->frictionY);

		gameObjs[i]->velocityX += gameObjs[i]->accelerationX * FIXED_TIMESTEP;
		gameObjs[i]->velocityY += gameObjs[i]->accelerationY * FIXED_TIMESTEP;

		gameObjs[i]->yPos += gameObjs[i]->velocityY * FIXED_TIMESTEP;
		
		if (!gameObjs[i]->isStatic) {
			for (size_t j = 0; j < gameObjs.size(); j++) {
				if (gameObjs[i]->collidesWith(gameObjs[j]) && gameObjs[i] != gameObjs[j]) {
					float yPenetration = fabs(fabs(gameObjs[j]->yPos - gameObjs[i]->yPos) - gameObjs[i]->height / 2.0f - gameObjs[j]->height / 2.0f);
					if (gameObjs[i]->yPos > gameObjs[j]->yPos) {
						gameObjs[i]->yPos += yPenetration + 0.001f;
						gameObjs[i]->collidedBottom = true;
					}
					else if (gameObjs[i]->yPos < gameObjs[j]->yPos) {
						gameObjs[i]->yPos -= yPenetration + 0.001f;
						gameObjs[i]->collidedTop = true;
					}
				}
			}
		}
		//xpos according to velocity
		gameObjs[i]->xPos += gameObjs[i]->velocityX * FIXED_TIMESTEP;
		
		if (!gameObjs[i]->isStatic) {
			for (size_t j = 0; j < gameObjs.size(); j++) {
				if (gameObjs[i]->collidesWith(gameObjs[j]) && gameObjs[i] != gameObjs[j]) {
					float xPenetration = fabs(fabs(gameObjs[j]->xPos - gameObjs[i]->xPos) - gameObjs[i]->width / 2.0f - gameObjs[j]->width / 2.0f);
					if (gameObjs[i]->xPos > gameObjs[j]->xPos) {
						gameObjs[i]->xPos += xPenetration + 0.001f;
						gameObjs[i]->collidedLeft = true;
					}
					else if (gameObjs[i]->xPos < gameObjs[j]->xPos) {
						gameObjs[i]->xPos -= xPenetration + 0.001f;
						gameObjs[i]->collidedRight = true;
					}
				}
			}
		}
	}

	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemies[i].collidedRight) {
			enemies[i].accelerationX = -1.0f;
		}

		if (enemies[i].collidedLeft) {
			enemies[i].accelerationX = 1.0f;
		}
		//if enemy got shot by bullet
		for (int j = 0; j < MAX_BULLETS; j++) {
			if (enemies[i].collidesWith(&bullets[j])) {
				bullets[j].visible = false;
				enemies[i].yPos = 1.5f; //move enemy back to top
				enemies[i].xPos = 0.0f;
				score++;
			}
		}
		
		if (enemies[i].xPos < -1.33 || enemies[i].xPos > 1.33 || enemies[i].yPos < -1) {
			enemies[i].yPos = 1.5;
			enemies[i].xPos = 0;
		}		
	}
}

void Game::Render() {
	//set color to teal
	glClearColor(0.0f, 0.7f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (size_t i = 0; i < gameObjs.size(); i++) {
		gameObjs[i]->Render();
	}

	for (size_t i = 0; i < MAX_BULLETS; i++) {
		bullets[i].Render();
	}
	//draw score on screen
	glLoadIdentity();
	glTranslatef(-1.0f, 0.9f, 0.0f);
	DrawText(fontSheetTexture, "Score: " + to_string(score), 0.08, 0.0, 1.0, 1.0, 1.0, 1.0);

	//draw number of life on screen
	glLoadIdentity();
	glTranslatef(0.5f, 0.9f, 0.0f);
	DrawText(fontSheetTexture, "Life: " + to_string(life), 0.08, 0.0, 1.0, 1.0, 1.0, 1.0);

	SDL_GL_SwapWindow(displayWindow);
}

bool Game::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				if (shootTimer > 0.1f)
					shootBullet();
			}
		}
	}
	
	//shoot bullet
	if (keys[SDL_SCANCODE_SPACE]) {
		if (shootTimer > 0.1f) {
			shootTimer = 0.0f;
			shootBullet();
		}
	}

	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP* MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP* MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		FixedUpdate();
	}
	timeLeftOver = fixedElapsed;

	Update(elapsed);
	Render();
	return done;
}

//build the game level, sets sprite and x y position, and width and height
//static to true so it doesnt move
void Game::buildLevel() {
	brickIndex = 0;
	SpriteSheet brickSprite;

	//Base
	for (float i = -1.33f; i <= 1.33f; i = i + 0.2f) {
		brickSprite = SpriteSheet(levelTexture, 16, 8, 3  );
		bricks[brickIndex].sprite = brickSprite;
		bricks[brickIndex].isStatic = true;
		bricks[brickIndex].yPos = -0.9f;
		bricks[brickIndex].xPos = i;
		bricks[brickIndex].width = 0.2f;
		bricks[brickIndex].height = 0.2f;
		gameObjs.push_back(&bricks[brickIndex]);
		brickIndex++;
	}

	brickSprite = SpriteSheet(levelTexture, 16, 8, 6);
	//left wall
	for (float i = -0.4f; i <= 1.0f; i = i + 0.2f) {
		bricks[brickIndex].sprite = brickSprite;
		bricks[brickIndex].isStatic = true;
		bricks[brickIndex].yPos = i;
		bricks[brickIndex].xPos = -1.25f;
		bricks[brickIndex].width = 0.2f;
		bricks[brickIndex].height = 0.2f;
		gameObjs.push_back(&bricks[brickIndex]);
		brickIndex++;
	}
	//right wall
	for (float i = -0.4f; i <= 1.0f; i = i + 0.2f) {
		bricks[brickIndex].sprite = brickSprite;
		bricks[brickIndex].isStatic = true;
		bricks[brickIndex].yPos = i;
		bricks[brickIndex].xPos = 1.25f;
		bricks[brickIndex].width = 0.2f;
		bricks[brickIndex].height = 0.2f;
		gameObjs.push_back(&bricks[brickIndex]);
		brickIndex++;
	}
	//middle platform
	for (float i = -0.4f; i <= 0.4f; i = i + 0.2f) {
		bricks[brickIndex].sprite = brickSprite;
		bricks[brickIndex].isStatic = true;
		bricks[brickIndex].yPos = 0.0f;
		bricks[brickIndex].xPos = i;
		bricks[brickIndex].width = 0.2f;
		bricks[brickIndex].height = 0.2f;
		gameObjs.push_back(&bricks[brickIndex]);
		brickIndex++;
	}

}
//shoots bullet
void Game::shootBullet() {
	SpriteSheet bulletSprite = SpriteSheet(levelTexture, 16, 8, 23);
	bullets[bulletIndex].sprite = bulletSprite;
	bullets[bulletIndex].visible = true;
	bullets[bulletIndex].width = 0.15;
	bullets[bulletIndex].xPos = player->xPos;
	bullets[bulletIndex].yPos = player->yPos;
	bullets[bulletIndex].rotation = 0.0f;
	bullets[bulletIndex].velocityX = 3.5f * player->direction;
	bulletIndex++;
	if (bulletIndex > MAX_BULLETS - 1) {
		bulletIndex = 0;
	}
	shootTimer = 0;
}

//lerp takes where to start, and exact where to end, and t is between 0 or 1
float lerp(float start, float end, float t) {
	return (1.0f - t)*start + t*end;
}
