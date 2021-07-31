#include "Level1.h"
#define LEVEL1_WIDTH 20
#define LEVEL1_HEIGHT 8
static int LEVEL1_ENEMY_COUNT = 1;
unsigned int level1_data[] =
{
24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,
24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,24,24,
24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,24,24,
28,28,28,24,24,24,24,24,24,24,24,24,24,24,24,24,0,0,0,24,
24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,24,0,0,24,
24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,
24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,24,
24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24
};

void Level1::Initialize() {
    state.nextScene = -1;
	GLuint mapTextureID = Util::LoadTexture("fantasy-tileset.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 8, 26);
	// Move over all of the player and enemy code from initialization.

    //Initialize Enemies

    // Initialize Player
    state.player = new Entity();
    GLuint rightTextID = Util::LoadTexture("right.png");
    GLuint leftTextID = Util::LoadTexture("left.png");

    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, -5, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 3.0f;

    state.player->textureID = rightTextID;
    state.player->JIC = leftTextID;

    state.player->animDown = new int[7]{ 4, 5, 6, 7, 4, 5, 6 };
    state.player->animLeft = new int[7]{ 12, 11, 10, 9, 8, 7, 12 };
    state.player->animUp = new int[7]{ 16, 17, 18, 19, 20, 21, 22 };
    state.player->animRight = new int[7]{ 8, 9, 10, 11, 12, 13, 8 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 7;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 7;
    state.player->animRows = 11;

    state.player->height = 0.75f;
    state.player->width = 0.75f;

    state.player->jumpPOW = 6.5f;

    //Initialize Enemies 
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("hand.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].kind = WAITNGO;
    state.enemies[0].cond = IDLE;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].acceleration = glm::vec3(0, -9.81, 0);
    state.enemies[0].position = glm::vec3(9, -5, 0);
    state.enemies[0].speed = 1.5f;
    state.enemies[0].height = 0.75;
    state.enemies[0].width = 0.75;
    
}

void Level1::Update(float deltaTime) {
	state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    state.enemies[0].Update(deltaTime, state.player, NULL, 0, state.map);
    //if (state.player->hit)
        //Initialize();
}

void Level1::Render(ShaderProgram* program,int rar) {
	state.map->Render(program);
	state.player->Render(program);
    state.enemies[0].Render(program);
    GLuint fontTextureID = Util::LoadTexture("font1.png");
    Util::DrawText(program, fontTextureID, "GOAL!", 0.5f, -0.25f,
        glm::vec3(1, -2.2, 0));
    if (rar >= 3) {
        state.player->isActive = false;
        Util::DrawText(program, fontTextureID, "You Lose!", 0.5, -0.25,
            glm::vec3(state.player->position.x,
                state.player->position.y+2, 0));
    }

}
