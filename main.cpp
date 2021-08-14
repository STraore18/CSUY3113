#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include <SDL_mixer.h>
#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"
#include "Entity.h"
#include "Scene.h"
#include "MainMenu.h"
#include "Intro.h"
#include "Level1.h"
#include "Level2.h"
#include "Victory.h"
#include "Defeat.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene* currentScene;
Scene* sceneList[6];


void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}

Mix_Music* music;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("3D!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 1280, 720);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("Nonstop.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::perspective(glm::radians(45.0f), 1.777f, 0.1f, 100.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.5294f, 0.8078f, 0.9216f, 1.0f);

    
    
    sceneList[0] = new MainM();
    sceneList[1] = new intro();
    sceneList[2] = new Level1();
    sceneList[3] = new Level2();
    sceneList[4] = new victory();
    sceneList[5] = new defeat();
    SwitchToScene(sceneList[0]);

}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        // Some sort of action
                        break;
                        
                }
                break;
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if ((keys[SDL_SCANCODE_RETURN]) && (currentScene == sceneList[0])) {
        currentScene->state.nextScene = 1;
    }

    else if ((keys[SDL_SCANCODE_SPACE]) && (currentScene == sceneList[1]))
        currentScene->state.nextScene = 2;

    else if ((currentScene != sceneList[0]) && (currentScene != sceneList[1])
        && (currentScene != sceneList[4]) && (currentScene != sceneList[5])) {

        if (keys[SDL_SCANCODE_A]) {
            currentScene->state.player->rotation.y += 2.0f;
        }
        else if (keys[SDL_SCANCODE_D]) {
            currentScene->state.player->rotation.y -= 2.0f;
        }

        else if (keys[SDL_SCANCODE_SPACE]) {
            //currentScene->state.player->velocity.y += 5;
        }

        currentScene->state.player->velocity.x = 0;
        currentScene->state.player->velocity.z = 0;

        if (keys[SDL_SCANCODE_W]) {
            currentScene->state.player->velocity.z = cos(glm::radians(currentScene->state.player->rotation.y)) * -4.0f;
            currentScene->state.player->velocity.x = sin(glm::radians(currentScene->state.player->rotation.y)) * -4.0f;
        }
        else if (keys[SDL_SCANCODE_S]) {
            currentScene->state.player->velocity.z = cos(glm::radians(currentScene->state.player->rotation.y)) * 4.0f;
            currentScene->state.player->velocity.x = sin(glm::radians(currentScene->state.player->rotation.y)) * 4.0f;
        }
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
        
        currentScene->Update(FIXED_TIMESTEP);

        deltaTime -= FIXED_TIMESTEP;
    }
    
    accumulator = deltaTime;

    /*
    if ((currentScene==sceneList[2])&&(currentScene->state.cont >= 6)) {
        currentScene->state.nextScene = 4;
    }
    */

    if ((currentScene != sceneList[0]) && (currentScene != sceneList[1])
        && (currentScene != sceneList[4]) && (currentScene != sceneList[5])) {
        viewMatrix = glm::mat4(1.0f);
        viewMatrix = glm::rotate(viewMatrix,
            glm::radians(currentScene->state.player->rotation.y), glm::vec3(0, -1.0f, 0));
        viewMatrix = glm::translate(viewMatrix, -currentScene->state.player->position);
    }
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program.SetViewMatrix(viewMatrix);

    currentScene->Render(&program);
    
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    //Mix_FreeMusic(twomusic);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();

        if (currentScene->state.nextScene >= 0) {
            SwitchToScene(sceneList[currentScene->state.nextScene]);
        }

        Render();
    }
    
    Shutdown();
    return 0;
}
