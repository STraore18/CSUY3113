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


SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, blockMatrix, block2Matrix, projectionMatrix, ballMatrix;

glm::vec3 player1_position = glm::vec3(-19.0, 0, 0);
glm::vec3 player1_movement = glm::vec3(0, 0, 0);
float player1_speed = 3.0f;

glm::vec3 player2_position = glm::vec3(19.0, 0, 0);
glm::vec3 player2_movement = glm::vec3(0, 0, 0);
float player2_speed = 3.0f;

glm::vec3 ball_position = glm::vec3(0, 0, 0);
glm::vec3 ball_movement = glm::vec3(0, 0, 0);
float ball_speed = 5.0f;

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
    displayWindow = SDL_CreateWindow("Pong!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");

    viewMatrix = glm::mat4(1.0f);
    blockMatrix = glm::mat4(1.0f);
    block2Matrix = glm::mat4(1.0f);
    ballMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(0.0f, 0.0f, 0.0f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void ProcessInput() {

    player1_movement = glm::vec3(0);
    player2_movement = glm::vec3(0);


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
                ball_position = glm::vec3(0);
                ball_movement = glm::vec3(0);
                ball_movement.x += 1.0f;
                ball_movement.y += 1.0f;
                break;
            }
            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_S]) {
        player1_movement.y = -1.0f;
    }

    else if (keys[SDL_SCANCODE_W]) {
        player1_movement.y = 1.0f;
    }

    if (keys[SDL_SCANCODE_DOWN]) {
        player2_movement.y = -1.0f;
    }

    else if (keys[SDL_SCANCODE_UP]) {
        player2_movement.y = 1.0f;
    }

    if (ball_position.y >= 14) {
        ball_movement.y -= 1.0f;
    }
    else if (ball_position.y <= -14) {
        ball_movement.y += 1.0f;
    }

    if (ball_position.x >= 19) {
        player1_movement = glm::vec3(0);
        player2_movement = glm::vec3(0);
        ball_movement = glm::vec3(0);
    }
    else if (ball_position.x <= -19) {
        player1_movement = glm::vec3(0);
        player2_movement = glm::vec3(0);
        ball_movement = glm::vec3(0);
    }
    if (player1_position.y <= -4.1875f) {
        //player1_movement = glm::vec3(0);
    }

    if (player1_position.y >= 4.1875f) {
        player1_movement = glm::vec3(0);
    }

    //Collison Detection
    float player2_dist = fabs(player1_position.x - 0.25);
    if (ball_position.x == player2_dist) {
        ball_movement.x -= 1.0f;
    }
}

float lastTicks = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    player1_position += player1_movement * player1_speed * deltaTime;
    player2_position += player2_movement * player2_speed * deltaTime;
    ball_position += ball_movement * ball_speed * deltaTime;



    blockMatrix = glm::mat4(1.0f);
    block2Matrix = glm::mat4(1.0f);
    ballMatrix = glm::mat4(1.0f);
    blockMatrix = glm::scale(blockMatrix, glm::vec3(0.25, 0.8, 1.0));
    block2Matrix = glm::scale(block2Matrix, glm::vec3(0.25, 0.8, 1.0));
    ballMatrix = glm::scale(ballMatrix, glm::vec3(0.25, 0.25, 1.0));
    blockMatrix = glm::translate(blockMatrix, player1_position);
    block2Matrix = glm::translate(block2Matrix, player2_position);
    ballMatrix = glm::translate(ballMatrix, ball_position);


}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);


    float vertices[] = { -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,-0.5f,-0.5f,0.5f,0.5f,-0.5f,0.5f };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    program.SetModelMatrix(blockMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    program.SetModelMatrix(ballMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    program.SetModelMatrix(block2Matrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);

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