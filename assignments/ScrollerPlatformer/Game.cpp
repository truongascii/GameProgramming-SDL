#include "Game.h"

Game::Game() {
	Init();


	done = false;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;

	gravityX = 0.0f;
	gravityY = -9.8f;

	bulletIndex = 0;
	shootTimer = 0.1f;

	levelTexture = LoadTexture("arne_sprites.png");
	chracterTexture = LoadTexture("characters_3.png");
	buildLevel();

	gunshot = Mix_LoadWAV("fire.wav");
	jump = Mix_LoadWAV("jump.wav");
	music = Mix_LoadMUS("tapcat.wav");

	if (Mix_PlayMusic(music, -1) < 0) {
		cout << "Error";
	}
}

Game::~Game() {


	SDL_Quit();
}

void Game::Init() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-2.66, 2.66, -2.0, 2.0, -2.0, 2.0);
}

void Game::Update(float elapsed) {
	player->Update(elapsed);

	//if (enemySpawnTimer > 1.0f && enemyIndex < 8) {
	//	SpriteSheet enemySprite = SpriteSheet(chracterTexture, 8, 4, 26);
	//	enemies[enemyIndex].sprite = enemySprite;
	//	enemies[enemyIndex].yPos = 0.85f;
	//	enemies[enemyIndex].xPos = 0.65f;
	//	enemies[enemyIndex].width = 0.2f;
	//	enemies[enemyIndex].height = 0.2f;
	//	enemies[enemyIndex].accelerationX = -2.0f;
	//	gameObjs.push_back(&enemies[enemyIndex]);
	//	enemyIndex++;
	//	enemySpawnTimer = 0.0f;
	//}

	for (size_t i = 0; i < MAX_BULLETS; i++) {
		bullets[i].Update(elapsed);
	}

	shootTimer += elapsed;
	enemySpawnTimer += elapsed;
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

		gameObjs[i]->xPos += gameObjs[i]->velocityX * FIXED_TIMESTEP;
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
		doLevelCollisionY(gameObjs[i]);
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
		doLevelCollisionX(gameObjs[i]);
	}


	for (int i = 0; i < MAX_ENEMIES; i++) {
		//enemy gets hit
		for (int k = 0; k < MAX_BULLETS; k++) {
			if (enemies[i].collidesWith(&bullets[k])) {
				bullets[k].visible = false;
			}
		}
	}
}

void Game::Render() {
	glClearColor(0.0f, 0.7f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	float translateX = player->xPos;
	float translateY = player->yPos;

	if (translateY > 1.5)
		translateY = 1.5;
	if (translateX > 8.0)
		translateX = 8.0;
	if (translateX < -8.0)
		translateX = -8.0;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(translateX, translateY, 0.0f);

	for (size_t i = 0; i < gameObjs.size(); i++) {
		gameObjs[i]->Render();
	}

	for (size_t i = 0; i < MAX_BULLETS; i++) {
		bullets[i].Render();
	}
	levelRenderer();

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

	if (keys[SDL_SCANCODE_UP]) {
		if (!player->offGround) {
			Mix_PlayChannel(-1, jump, 0);
		}
	}

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

void Game::buildLevel() {
	ifstream infile("rakoon.txt");
	string line;
	while (getline(infile, line)) {
		if (line == "[header]") {
			if (!readHeader(infile)) {
				return;
			}
		}
		else if (line == "[layer]") {
			readLayerData(infile);
		}
		else if (line == "[Object Layer 1]") {
			readEntityData(infile);
		}
	}
}

bool Game::readHeader(ifstream& stream) {
	string line;
	mapWidth = -1;
	mapHeight = -1;
	while (getline(stream, line)) {
		if (line == "") { break; }

		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);

		if (key == "width") {
			mapWidth = atoi(value.c_str());
		}
		else if (key == "height"){
			mapHeight = atoi(value.c_str());
		}
	}

	if (mapWidth == -1 || mapHeight == -1) {
		return false;
	}
	else {
		levelData = new unsigned char*[mapHeight];
		for (int i = 0; i < mapHeight; ++i) {
			levelData[i] = new unsigned char[mapWidth];
		}
		return true;
	}
}
bool Game::readLayerData(ifstream& stream) {
	string line;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "data") {
			for (int y = 0; y < mapHeight; y++) {
				getline(stream, line);
				istringstream lineStream(line);
				string tile;

				for (int x = 0; x < mapWidth; x++) {
					getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						levelData[y][x] = val - 1;
					}
					else {
						levelData[y][x] = 0;
					}
				}

			}
		}
	}
	return true;
}
bool Game::readEntityData(ifstream& stream) {
	string line;
	string type;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "type") {
			type = value;
		}
		else if (key == "location") {
			istringstream lineStream(value);
			string xPosition, yPosition;
			getline(lineStream, xPosition, ',');
			getline(lineStream, yPosition, ',');

			float placeX = (atoi(xPosition.c_str()) + 8.0f - 64 * 16.0f / 2.0f) / 16.0f * TILE_SIZE;
			float placeY = (atoi(yPosition.c_str()) - 8.0f - 32.0f * 16.0f / 2.0f) / 16.0f * -TILE_SIZE;

			mapObject(type, placeX, placeY);
		}
	}
	return true;
}

void Game::mapObject(string& type, float placeX, float placeY) {
	if (type == "player") {
		player = new Player(chracterTexture);
		player->xPos = placeX;
		player->yPos = placeY;
		player->width = 0.2f;
		player->height = 0.2f;
		player->frictionX = 3.0f;
		gameObjs.push_back(player);
	}
}

void Game::levelRenderer() {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, levelTexture);

	vector<float> vertexData;
	vector<float> texCoordData;

	int numVertices = 0;

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (levelData[y][x] != 0) {
				float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
				float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
				float spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
				float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;
				vertexData.insert(vertexData.end(), {
					TILE_SIZE* x, -TILE_SIZE* y,
					TILE_SIZE* x, (-TILE_SIZE* y) - TILE_SIZE,
					(TILE_SIZE* x) + TILE_SIZE, (-TILE_SIZE* y) - TILE_SIZE,
					(TILE_SIZE* x) + TILE_SIZE, -TILE_SIZE* y
				});
				texCoordData.insert(texCoordData.end(), { u, v,
					u, v + (spriteHeight),
					u + spriteWidth, v + (spriteHeight),
					u + spriteWidth, v
				});

				numVertices += 4;
			}
		}
	}

	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(-TILE_SIZE* mapWidth / 2, TILE_SIZE* mapHeight / 2, 0.0f);

	glDrawArrays(GL_QUADS, 0, numVertices);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}

void Game::shootBullet() {
	Mix_PlayChannel(-1, gunshot, 0);
	SpriteSheet bulletSprite = SpriteSheet(levelTexture, 16, 8, 23);
	bullets[bulletIndex].sprite = bulletSprite;
	bullets[bulletIndex].visible = true;
	bullets[bulletIndex].xPos = player->xPos;
	bullets[bulletIndex].yPos = player->yPos;
	bullets[bulletIndex].rotation = 0.0f;
	bullets[bulletIndex].velocityX = -3.5f * player->direction;
	
	bulletIndex++;
	if (bulletIndex > MAX_BULLETS - 1) {
		bulletIndex = 0;
	}
	shootTimer = 0;
}

//CHECK IF SOLID OR EMPY SPACE
bool Game::isSolid(unsigned char tile) {
	switch (tile) {
	case 0:
		return false;
		break;
	default:
		return true;
		break;
	}
}

void Game::worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY) {
	*gridX = (int)(((worldX * TILE_SIZE) / 16.0f) + (64.0f / 2.0f));
	*gridY = (int)(((worldY * -TILE_SIZE) / 16.0f) + (32.0f / 2.0f));
}

float Game::checkPointForGridCollisionX(float x, float y) {
	int gridX, gridY;
	worldToTileCoordinates(x, y, &gridX, &gridY);
	if (gridX < 0 || gridX > 128 || gridY < 0 || gridY > 32) {
		return 0.0f;
	}

	if (isSolid(levelData[gridY][gridX])) {
		return 0.004f;
	}
	return 0.0f;
}

float Game::checkPointForGridCollisionY(float x, float y) {
	int gridX, gridY;
	worldToTileCoordinates(x, y, &gridX, &gridY);
	if (gridX < 0 || gridX > 128 || gridY < 0 || gridY > 32) {
		return 0.0f;
	}

	if (isSolid(levelData[gridY][gridX])) {

		float yCoordinate = (gridY * TILE_SIZE) - (TILE_SIZE*16.0);
		return -y - yCoordinate;

	}
	return 0.0f;
}

void Game::doLevelCollisionX(GameObject *gameObj) {
	float adjust = checkPointForGridCollisionX(gameObj->xPos + gameObj->width*0.5, gameObj->yPos);
	if (adjust != 0.0f) {
		gameObj->xPos -= adjust;
		gameObj->velocityX = 0.0f;
		gameObj->collidedRight = true;
	}

	adjust = checkPointForGridCollisionX(gameObj->xPos - gameObj->width*0.5, gameObj->yPos);
	if (adjust != 0.0f) {
		gameObj->xPos += adjust;
		gameObj->velocityX = 0.0f;
		gameObj->collidedLeft = true;
	}

}

void Game::doLevelCollisionY(GameObject *gameObj) {
	float adjust = checkPointForGridCollisionY(gameObj->xPos, gameObj->yPos - gameObj->height*0.5);
	if (adjust != 0.0f) {
		gameObj->yPos += adjust;
		gameObj->velocityY = 0.0f;
		gameObj->collidedBottom = true;
	}
}

float lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}