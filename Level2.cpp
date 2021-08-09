#include "Level2.h"
#define LEVEL2_WIDTH 14
#define LEVEL2_HEIGHT 8
static int LEVEL2_ENEMY_COUNT = 1;

unsigned int level2_data[] =
{
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 1,
 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 0, 0, 2,
 3, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 0, 0, 2
};



void Level2::Initialize() {
    state.nextScene = -1;
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 4, 1);
    // Move over all of the player and enemy code from initialization.

    //Initialize Enemies
    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("hand.png");


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

    // Initialize Player
    state.player = new Entity();
    GLuint mainTextID = Util::LoadTexture("george_0.png");

    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, 0, 0);
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

}
void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);
}
void Level2::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);
}