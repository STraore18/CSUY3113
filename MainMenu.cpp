#include "MainMenu.h"

#define MainM_WIDTH 20
#define MainM_HEIGHT 8
static int MainM_ENEMY_COUNT = 1;

unsigned int MainM_data[] =
{
 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24
};

GLuint fontTextureID;

void MainM::Initialize() {
    state.nextScene = -1;
    GLuint mapTextureID = Util::LoadTexture("fantasy-tileset.png");
    state.map = new Map(MainM_WIDTH, MainM_HEIGHT, MainM_data, mapTextureID, 1.0f, 8, 26);
    // Move over all of the player and enemy code from initialization.

    // Initialize Player
    state.player = new Entity();
    GLuint mainTextID = Util::LoadTexture("george_0.png");

    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(9.5, 0, 0);
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

    state.player->height = 0.75f;
    state.player->width = 0.75f;

    state.player->jumpPOW = 7.5f;
    state.player->isActive = false;

}

void MainM::Update(float deltaTime) {
    
}

void MainM::Render(ShaderProgram* program,int rar) {
    //state.map->Render(program);
    state.player->Render(program);
    GLuint fontTextureID = Util::LoadTexture("font1.png");

    Util::DrawText(program, fontTextureID, "Dungeon Crawler",
        0.75f, -0.25f, glm::vec3(1, -3, 0));

    Util::DrawText(program, fontTextureID, "Press Enter to Start!",
        0.45f, -0.25f, glm::vec3(2.5, -5, 0));


}