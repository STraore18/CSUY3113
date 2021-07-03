#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"

static int block_n= 25;

struct Gamestate {
	Entity* player;
	Entity* blocks;
};

Gamestate state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

GLuint LoadTexture(const char* filePath) {
	int w, h, n;
	unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

	if (image == NULL) {
		std::cout << "Unable to load image. Make sure the path is correct\n";
		assert(false);
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(image);
	return textureID;
}


void Initialize() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Moving!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 480);

	program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

	viewMatrix = glm::mat4(1.0f);
	modelMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(-5.0f, 5.0f, -4.0f, 4.0f, -1.0f, 1.0f);

	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);
	//program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);

	glUseProgram(program.programID);

	glClearColor(0.5f, 0.5f, 0.2f, 1.0f);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	state.player = new Entity();
	state.player->position = glm::vec3(0, 3.5f, 0);
	state.player->movement = glm::vec3(0, 0, 0);
	state.player->acceleration = glm::vec3(0, -1.0f, 0);
	state.player->speed = 1.0f;
	state.player->textureID = LoadTexture("playerShip3_blue.png");

	/*state.player->animDown = new int[4]{ 0, 4, 8, 12 };
	state.player->animLeft = new int[4]{ 1, 5, 9, 13 };
	state.player->animUp = new int[4]{ 2, 6, 10, 14 };
	state.player->animRight = new int[4]{ 3, 7, 11, 15 };
	state.player->animIndices = state.player->animRight;

	state.player->animFrames = 4;
	state.player->animIndex = 0;
	state.player->animTime = 0;
	state.player->animCols = 4;
	state.player->animRows = 4;*/

	state.blocks = new Entity[block_n];

	float x = -4.5f;
	float y = 3.5f;
	for (int i = 0; i < block_n; i++) {
		state.blocks[i].textureID = LoadTexture("platformPack_tile020.png");
		state.blocks[i].position = glm::vec3(x, y, 0.0f);
		if (x <= -4.5 && y == 3.5) {
			y -= 1.0f;
		}
		else if (y == -3.5f) {
			x += 1.0f;
		}
		else
			y += 1.0f;
	}
	
	//for (int j=0;i<)
	/*state.blocks[0].position = glm::vec3(-4.5f, -3.5f, 0.0f);
	state.blocks[1].position = glm::vec3(-3.5f, -3.5f, 0.0f);
	state.blocks[2].position = glm::vec3(-2.5f, -3.5f, 0.0f);
	state.blocks[3].position = glm::vec3(-1.5f, -3.5f, 0.0f);
	state.blocks[4].position = glm::vec3(-0.5f, -3.5f, 0.0f);
	state.blocks[5].position = glm::vec3(0.5f, -3.5f, 0.0f);
	state.blocks[6].position = glm::vec3(1.5f, -3.5f, 0.0f);
	state.blocks[7].position = glm::vec3(2.5f, -3.5f, 0.0f);
	state.blocks[8].position = glm::vec3(3.5f, -3.5f, 0.0f);
	state.blocks[9].position = glm::vec3(4.5f, -3.5f, 0.0f);
	state.blocks[10].position = glm::vec3(5.5f, -3.5f, 0.0f);
	*/

}

void ProcessInput() {

	state.player->movement = glm::vec3(0);

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
		case SDL_WINDOWEVENT_CLOSE:
			gameIsRunning = false;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_LEFT:
				// Move the player left
				break;

			case SDLK_RIGHT:
				// Move the player right
				break;

			case SDLK_SPACE:
				// Some sort of action
				break;
			}
			break; // SDL_KEYDOWN
		}
	}

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_A]) {
		state.player->movement.x = -1.0f;
		//state.player->animIndices = state.player->animLeft;
	}
	else if (keys[SDL_SCANCODE_D]) {
		state.player->movement.x = 1.0f;
		//state.player->animIndices = state.player->animRight;
	}


	if (glm::length(state.player->movement) > 1.0f) {
		state.player->movement = glm::normalize(state.player->movement);
	}

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;
void Update() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float deltaTime = ticks - lastTicks;
	lastTicks = ticks;

	deltaTime += accumulator;
	if (deltaTime < FIXED_TIMESTEP) {
		accumulator = deltaTime;
		return;
	}

	while (deltaTime >= FIXED_TIMESTEP) {
		// Update. Notice it's FIXED_TIMESTEP. Not deltaTime
		state.player->Update(FIXED_TIMESTEP);
		for (int i = 0; i < block_n; i++) {
			state.blocks[i].Update(FIXED_TIMESTEP);
		}

		deltaTime -= FIXED_TIMESTEP;
	}

	accumulator = deltaTime;
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < block_n; i++) {
		state.blocks[i].Render(&program);
	}

	state.player->Render(&program);

	SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	Initialize();

	while (gameIsRunning) {
		ProcessInput();
		Update();
		Render();
	}

	Shutdown();
	return 0;
}