#include "Game.h"

//DEFAULT CONSTRUCTOR
Game::Game() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	done = false;
	lastFrameTicks = 0.0f;
	state = STATE_MAIN_MENU;

	score = 0;
	Init();
}

//SHOOT BULLETS
void Game::shootBullet() {
	//<SubTexture name="laserRed13.png" x="856" y="812" width="9" height="57"/>
	SpriteSheet bulletSprite = SpriteSheet(spriteSheetTexture, 856.0f / 1024.0f, 812.0f / 1024.0f, 9.0f / 1024.0f, 57.0f / 1024.0f);
	bullets[bulletIndex].sprite = bulletSprite;
	bullets[bulletIndex].visible = true;
	bullets[bulletIndex].x = gameObjs[0]->getX(); //players x pos
	bullets[bulletIndex].y = gameObjs[0]->getY(); //players y pos
	bullets[bulletIndex].scale = 0.8f;
	bullets[bulletIndex].rotation = 0.0f;
	bullets[bulletIndex].speed = 2.5f;
	bulletIndex++;
	if (bulletIndex > MAX_BULLETS - 1) {
		bulletIndex = 0;
	}
}
//Load requested texture
GLuint Game::LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);

	//If unable to load texture
	if (surface == NULL)
	{
		char directory[1024];
		GetCurrentDirectory(1024, directory);
		OutputDebugString(directory);
		OutputDebugString(IMG_GetError());
		OutputDebugString("error loading the image!\n");
		exit(2);
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//repeat for scrolling background
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	SDL_FreeSurface(surface);

	return textureID;
}
//Draws the requested text on screen
void Game::DrawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a) {
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float texture_size = 1.0 / 16.0f;

	vector<float> vertexData;
	vector<float> texCoordData;
	vector<float> colorData;

	for (size_t i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;

		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });
		vertexData.insert(vertexData.end(), { ((size + spacing) * i) + (-0.5f* size), 0.5f* size, ((size + spacing) * i) + (-0.5f* size), -0.5f* size, 
			((size + spacing) * i) + (0.5f* size), -0.5f* size, ((size + spacing) * i) + (0.5f* size), 0.5f * size });
		texCoordData.insert(texCoordData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x + texture_size, 
			texture_y + texture_size, texture_x + texture_size, texture_y });
	}

	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, text.size() * 4);
	glDisableClientState(GL_COLOR_ARRAY);
}
//Initializer 
void Game::Init() {
	bulletIndex = 0;

	for (size_t i = 0; i < MAX_BULLETS; i++) {
		bullets[i].visible = FALSE;
	}
	//empty out before inserting
	for (size_t i = 0; i < gameObjs.size(); i++) {
		delete gameObjs[i];
	}
	gameObjs.clear();

	spriteSheetTexture = LoadTexture("sheet.png");
	fontSheetTexture = LoadTexture("font2.png");
	//<SubTexture name = "playerShip3_red.png" x = "325" y = "0" width = "98" height = "75" / >
	SpriteSheet playerSprite = SpriteSheet(spriteSheetTexture, 325.0f / 1024.0f, 0.0f / 1024.0f, 98.0f / 1024.0f, 75.0f / 1024.0f);
	Player* player = new Player(playerSprite, 0.9f, 0.0f, -0.85f, 0.0f, 1.0f);
	gameObjs.push_back(player);

	//<SubTexture name="enemyBlue5.png" x="421" y="814" width="97" height="84"/>
	SpriteSheet enemySprite = SpriteSheet(spriteSheetTexture, 421.0f / 1024.0f, 814.0f / 1024.0f, 97.0f / 1024.0f, 84.0f / 1024.0f);

	for (float i = -1.0f; i < 1.0f; i = i + 0.2f) {
		for (float j = 0.5f; j > 0.0f; j = j - 0.2f) {
			GameObject* enemy = new GameObject(enemySprite, 0.7f, i, j, 0.0f, 1.0f, 0.2f, -0.03f);
			gameObjs.push_back(enemy);
		}
	}

}

//Destructor
Game::~Game() {
	//clear out pointers
	for (size_t i = 0; i < gameObjs.size(); i++) {
		delete gameObjs[i];
	}
	gameObjs.clear();

	SDL_Quit();
}

//Check for collision between enemy and bullet
bool Game::CollisionDetect(const GameObject& enemy, const Bullet& bullet) {
	float enemyTop = enemy.yPos + enemy.sprite.height / 2;
	float enemyBottom = enemy.yPos - enemy.sprite.height / 2;
	float enemyRight = enemy.xPos + enemy.sprite.width / 2;
	float enemyLeft = enemy.xPos - enemy.sprite.width / 2;

	float bulletTop = bullet.y + bullet.sprite.height / 2;
	float bulletBottom = bullet.y - bullet.sprite.height / 2;
	float bulletRight = bullet.x + bullet.sprite.width / 2;
	float bulletLeft = bullet.x - bullet.sprite.width / 2;

	if (enemyBottom > bulletTop)
		return false;
	if (enemyTop < bulletBottom)
		return false;
	if (enemyLeft > bulletRight)
		return false;
	if (enemyRight < bulletLeft)
		return false;

	return true;
}

//updates according to game state
void Game::Update(float elapsed) {
	switch (state) {
	case STATE_MAIN_MENU:
		UpdateMenuScene(elapsed);
		break;
	case STATE_GAME_LEVEL:
		UpdateGameScene(elapsed);
		break;
	case STATE_GAME_OVER:
		UpdateEndScene(elapsed);
		break;
	}
}

//Renders according to game state
void Game::Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	switch (state) {
	case STATE_MAIN_MENU:
		RenderMenuScene();
		break;
	case STATE_GAME_LEVEL:
		RenderGameScene();
		break;
	case STATE_GAME_OVER:
		RenderEndScene();
		break;
	}
	SDL_GL_SwapWindow(displayWindow);
}

//Updates menu scene
void Game::UpdateMenuScene(float elapsed) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
				state = STATE_GAME_LEVEL;
			}
		}
	}

}
//Updates game scene
void Game::UpdateGameScene(float elapsed) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				shootBullet();
			}
		}
	}

	for (size_t i = 1; i < gameObjs.size(); i++) {
		//enemy reached the bottom
		if (gameObjs[i]->getY() < -0.75) {
			state = STATE_GAME_OVER;
			Init();
		}

		if (gameObjs[i]->getX() > 1.2 || gameObjs[i]->getX() < -1.2) {
			for (size_t j = 1; j < gameObjs.size(); j++) {
				gameObjs[j]->directionX = -gameObjs[j]->directionX;
			}
			break;
		}

		for (size_t j = 0; j < MAX_BULLETS; j++) {
			if (bullets[j].visible && CollisionDetect(*gameObjs[i], bullets[j])) {
				bullets[j].visible = false;
				delete gameObjs[i];
				gameObjs.erase(gameObjs.begin() + i);
				score++;
				break;
			}
		}
	}

	for (size_t i = 0; i < gameObjs.size(); i++) {
		gameObjs[i]->Update(elapsed);
	}

	for (size_t i = 0; i < MAX_BULLETS; i++) {
		bullets[i].Update(elapsed);
	}
	//all enemy dead
	if (gameObjs.size() <= 1){
		state = STATE_GAME_OVER;
		Init();
	}
}
//Updates game over scene
void Game::UpdateEndScene(float elapsed) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
				score = 0;
				state = STATE_MAIN_MENU;
			}
		}
	}
}
//Renders menu scene
void Game::RenderMenuScene() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-0.95f, 0.2f, 0.0f);
	DrawText(fontSheetTexture, "Space Invaders", 0.15, 0.0, 0.0, 1.0, 0.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.7f, -0.1f, 0.0f);
	DrawText(fontSheetTexture, "Controlls: LEFT RIGHT SPACE ", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.5f, -0.3f, 0.0f);
	DrawText(fontSheetTexture, "Press ENTER to Play", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
}
//Renders game scene
void Game::RenderGameScene() {
	glLoadIdentity();
	glTranslatef(-0.3f, 0.9f, 0.0f);
	DrawText(fontSheetTexture, "Score: " + to_string(score), 0.08, 0.0, 1.0, 0.5, 1.0, 1.0);

	for (size_t i = 0; i < gameObjs.size(); i++) {
		gameObjs[i]->Render();
	}

	for (size_t i = 0; i < MAX_BULLETS; i++) {
		bullets[i].Render();
	}
}
//Renders gameover scene
void Game::RenderEndScene() {
	glMatrixMode(GL_MODELVIEW);
	if (score == 33){
		glLoadIdentity();
		glTranslatef(-0.3f, 0.2f, 0.0f);
		DrawText(fontSheetTexture, "YOU WIN", 0.1, 0.0, 0.0, 0.0, 1.0, 1.0);
	}
	else{
		glLoadIdentity();
		glTranslatef(-0.35f, 0.2f, 0.0f);
		DrawText(fontSheetTexture, "YOU LOSE", 0.1, 0.0, 0.0, 0.0, 1.0, 1.0);
	}
	glLoadIdentity();
	glTranslatef(-0.3f, 0.0f, 0.0f);
	DrawText(fontSheetTexture, "Your SCORE: ", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
	glTranslatef(0.6f, 0.0f, 0.0f);
	DrawText(fontSheetTexture, to_string(score), 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
	glLoadIdentity();
	glTranslatef(-0.85f, -0.5f, 0.0f);
	DrawText(fontSheetTexture, "Press ENTER to return to main menu", 0.05, 0.0, 1.0, 1.0, 1.0, 1.0);
}
//Updates and renders
bool Game::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	Update(elapsed);
	Render();

	return done;
}