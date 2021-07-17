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

#include <SDL_mixer.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"
#include <vector>
#include <iostream>

#define PLATFORM_COUNT 3;
static int plats = 42;
static int opps = 3;


struct GameState {
    Entity* player;
    Entity* platforms;
    Entity* enemies;
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Mix_Music* music;
Mix_Chunk* bounce;


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

GLuint LeftTextureID,RightTextureID,fontTextureID,enemyTextureID;

void DrawText(ShaderProgram* program, GLuint fontTextureID, std::string text,
    float size, float spacing, glm::vec3 position)
    ;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Textured!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-10.0f, 10.0f, -7.0f, 7.0f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint platfromTextureID = LoadTexture("Tile_1.png");
    GLuint LeftTextureID = LoadTexture("HeavyBandit.png");
    GLuint RightTextureID = LoadTexture("HeavyBandit_Right.png");
    GLuint fontTextureID = LoadTexture("font1.png");
    GLuint enemyTextureID = LoadTexture("hand.png");
    GLuint mainTextID = LoadTexture("george_0.png");

    //Audio
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("Ethernight_CLub.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    bounce = Mix_LoadWAV("bounce.wav");

    

    // Initialize Game Objects
     //Initialize platforms
    state.platforms = new Entity[plats];

    int a = 0;
    for (int i = 0; i < 21; i++) {
        state.platforms[i].entityType = PLATFORM;
        state.platforms[i].textureID = platfromTextureID;
        state.platforms[i].position = glm::vec3(-10 + i, -6.5f, 0);
        state.platforms[i].Update(0, NULL, NULL,NULL,0, 0);
    }
    for (int j = 21; j < 28; j++) {
        state.platforms[j].entityType = PLATFORM;
        state.platforms[j].textureID = platfromTextureID;
        state.platforms[j].position = glm::vec3(-10.0 + a, -2.5f, 0);
        state.platforms[j].Update(0, NULL, NULL,NULL,0, 0);
        a++;
    }
    a = 0;
    for (int k = 28; k < 40; k++) {
        state.platforms[k].entityType = PLATFORM;
        state.platforms[k].textureID = platfromTextureID;
        state.platforms[k].position = glm::vec3(10.0 - a, 2.5f, 0);
        state.platforms[k].Update(0, NULL, NULL, NULL, 0, 0);
        a++;
    }

    state.platforms[40].entityType = PLATFORM;
    state.platforms[40].textureID = platfromTextureID;
    state.platforms[40].position = glm::vec3(-2, -4.5f, 0);
    state.platforms[40].Update(0, NULL, NULL,NULL,0, 0);

    state.platforms[41].entityType = PLATFORM;
    state.platforms[41].textureID = platfromTextureID;
    state.platforms[41].position = glm::vec3(-3, 0.0f, 0);
    state.platforms[41].Update(0, NULL, NULL, NULL, 0, 0);




    //Initialize Enemies
    state.enemies = new Entity[opps];

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].kind = GHOST;
    state.enemies[0].cond = IDLE;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(-7, 6.5, 0);
    state.enemies[0].speed = 5.0f;


    state.enemies[1].entityType = ENEMY;
    state.enemies[1].kind = WAITNGO;
    state.enemies[1].cond = IDLE;
    state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].acceleration = glm::vec3(0, -9.81, 0);
    state.enemies[1].position = glm::vec3(4, 6.5, 0);
    state.enemies[1].speed = 1.5f;


    state.enemies[2].entityType = ENEMY;
    state.enemies[2].kind = WALKER;
    state.enemies[2].cond = IDLE;
    state.enemies[2].textureID = enemyTextureID;
    state.enemies[2].position = glm::vec3(8, -5.5, 0);
    state.enemies[2].speed = 2.0f;

    fontTextureID = LoadTexture("font1.png");

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = EntityType::PLAYER;
    state.player->position = glm::vec3(0, -1, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 3.0f;
    
    state.player->textureID = mainTextID;

    state.player->animDown = new int[4]{ 0,4,8,12 };
    state.player->animLeft = new int[4]{ 1,5,9,13 };
    state.player->animUp = new int[4]{ 2,6,10,14 };
    state.player->animRight = new int[4]{ 3,7,11,15 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;

    state.player->height = 1.0f;
    state.player->width = 1.0f;

    state.player->jumpPOW = 7.5f;

   
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
                if (state.player->collidedBottom) {
                    state.player->jump = true;
                    Mix_PlayChannel(-1, bounce, 0);
                }
                break;
            }
            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_A]) {
        state.player->movement.x = -1.0f;
        state.player->animIndices = state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_D]) {
        state.player->movement.x = 1.0f;
        state.player->animIndices = state.player->animRight;
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

    if (state.player->isActive == false) {
        DrawText(&program, fontTextureID, "You Lose!", 1.0f, 1.0f, glm::vec3(0));
    }

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    int isFin = 0;

    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        state.player->Update(FIXED_TIMESTEP,state.player,state.platforms,state.enemies,
            plats,opps);
        for (int i = 0; i < opps; i++) {
            state.enemies[i].Update(FIXED_TIMESTEP, state.player,state.platforms, state.enemies,
                plats, opps);
            if (state.enemies[i].isActive == false)
                isFin++;
        }
        
        if (isFin >= 3) {
            DrawText(&program, fontTextureID, "You Win!", 1.0f, 1.0f, glm::vec3(0, 0, 0));
        }

        deltaTime -= FIXED_TIMESTEP;
    }

    DrawText(&program, fontTextureID, "Please ffucking work Im tired", 1.0f, 1.0f, glm::vec3(0, 0, 0));


    accumulator = deltaTime;
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    state.player->Render(&program);

    for (int i = 0; i < plats; i++) {
        state.platforms[i].Render(&program);
    }

    for (int i = 0; i < opps; i++) {
        state.enemies[i].Render(&program);
    }

    DrawText(&program, fontTextureID, "Please ffucking work Im tired", 1.0f, 1.0f, glm::vec3(0, 0, 0));


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

void DrawText(ShaderProgram* program, GLuint fontTextureID, std::string text,
    float size, float spacing, glm::vec3 position) {
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;

    std::vector<float> vertices;
    std::vector<float> texCoords;

    for (unsigned int i = 0; i < (text.size()); i++) {

        int index = (int)text[i];
        float offset = (size + spacing) * i;
        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;




        vertices.insert(vertices.end(), {
         offset + (-0.5f * size), 0.5f * size,
         offset + (-0.5f * size), -0.5f * size,
         offset + (0.5f * size), 0.5f * size,
         offset + (0.5f * size), -0.5f * size,
         offset + (0.5f * size), 0.5f * size,
         offset + (-0.5f * size), -0.5f * size,
            });
        texCoords.insert(texCoords.end(), {
        u, v,
        u, v + height,
        u + width, v,
        u + width, v + height,
        u + width, v,
        u, v + height,
            });

    }
}

