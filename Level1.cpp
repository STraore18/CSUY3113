#include "Level1.h"
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8
static int LEVEL1_OPP_COUNT = 4;
static int LEVEL1_OBJECT_COUNT = 5;
static int LEVEL1_COLLECTIBLES = 13;
static int LEVEL1_THINGS = 10;

void Level1::Initialize() {
    state.nextScene = -1;
    state.vic = false;

    state.cont = LEVEL1_COLLECTIBLES;

    state.nectar = Mix_LoadWAV("honey.wav");

    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(0, 0.25f, -2);
    //state.player->acceleration = glm::vec3(0, -9.81, 0);
    state.player->speed = 1.0f;

    state.objects = new Entity[LEVEL1_OBJECT_COUNT];


    GLuint floorTextureID = Util::LoadTexture("floor_01.png");
    Mesh* cubeMesh = new Mesh();
    cubeMesh->LoadOBJ("cube.obj", 20);

    state.objects[0].textureID = floorTextureID;
    state.objects[0].mesh = cubeMesh;
    state.objects[0].position = glm::vec3(0, -0.5, -10);
    state.objects[0].rotation = glm::vec3(0, 0, 0);
    state.objects[0].acceleration = glm::vec3(0, 0, 0);
    state.objects[0].depth = 20;
    state.objects[0].width = 20;
    state.objects[0].height = 0.5;
    state.objects[0].scale = glm::vec3(20, 0.5f, 20);
    state.objects[0].entityType = FLOOR;

    GLuint wallTextureID = Util::LoadTexture("wall.png");

    state.objects[1].textureID = wallTextureID;
    state.objects[1].mesh = cubeMesh;
    state.objects[1].position = glm::vec3(10, -0.5, -10);
    state.objects[1].rotation = glm::vec3(0, 0, 0);
    state.objects[1].acceleration = glm::vec3(0, 0, 0);
    state.objects[1].depth = 20;
    state.objects[1].width = 0.5;
    state.objects[1].height = 20;
    state.objects[1].scale = glm::vec3(0.5f, 20, 20);
    state.objects[1].entityType = WALL;

    state.objects[2].textureID = wallTextureID;
    state.objects[2].mesh = cubeMesh;
    state.objects[2].position = glm::vec3(-10, -0.5, -10);
    state.objects[2].rotation = glm::vec3(0, 0, 0);
    state.objects[2].acceleration = glm::vec3(0, 0, 0);
    state.objects[2].depth = 20;
    state.objects[2].width = 0.5;
    state.objects[2].height = 20;
    state.objects[2].scale = glm::vec3(0.5f, 20, 20);
    state.objects[2].entityType = WALL;

    state.objects[3].textureID = wallTextureID;
    state.objects[3].mesh = cubeMesh;
    state.objects[3].position = glm::vec3(0, -0.5, 0);
    state.objects[3].rotation = glm::vec3(0, 0, 0);
    state.objects[3].acceleration = glm::vec3(0, 0, 0);
    state.objects[3].depth = 0.5;
    state.objects[3].width = 20;
    state.objects[3].height = 20;
    state.objects[3].scale = glm::vec3(20, 20, 0.5f);
    state.objects[3].entityType = WALL;

    state.objects[4].textureID = wallTextureID;
    state.objects[4].mesh = cubeMesh;
    state.objects[4].position = glm::vec3(0, -0.5, -20);
    state.objects[4].rotation = glm::vec3(0, 0, 0);
    state.objects[4].acceleration = glm::vec3(0, 0, 0);
    state.objects[4].depth = 0.5;
    state.objects[4].width = 20;
    state.objects[4].height = 20;
    state.objects[4].scale = glm::vec3(20, 20, 0.5f);
    state.objects[4].entityType = WALL;


    state.billies = new Entity[LEVEL1_THINGS];

    GLuint enemyTextureID = Util::LoadTexture("skull.gif");
    for (int i = 0; i < LEVEL1_OPP_COUNT; i++) {
        state.billies[i].billboard = true;
        state.billies[i].textureID = enemyTextureID;
        if (i < (LEVEL1_OPP_COUNT / 2)){
            state.billies[i].position = glm::vec3(rand() % 9, 0.5, -5 - (rand() % 15));
        }
        else {
            state.billies[i].position = glm::vec3((0 - rand() % 9), 0.5, -5 - (rand() % 15));
        }
        state.billies[i].rotation = glm::vec3(0, 0, 0);
        state.billies[i].acceleration = glm::vec3(0, 0, 0);
        state.billies[i].entityType = ENEMY;
    }

    GLuint itemTextureID = Util::LoadTexture("please.png");

    for (int d = LEVEL1_OPP_COUNT; d < LEVEL1_THINGS; d++) {
        state.billies[d].billboard = true;
        state.billies[d].textureID = itemTextureID;
        if (d < (LEVEL1_OPP_COUNT+(LEVEL1_COLLECTIBLES / 2))) {
            state.billies[d].position = glm::vec3(rand() % 9, 0.25, -5 - (rand() % 15));
        }
        else {
            state.billies[d].position = glm::vec3((0 - rand() % 9), 0.25, -5 - (rand() % 15));
        }
        state.billies[d].rotation = glm::vec3(0, 0, 0);
        state.billies[d].scale = glm::vec3(0.5f, 0.5f, 0.5f);
        state.billies[d].depth = 0.5;
        state.billies[d].width = 0.5;
        state.billies[d].height = 0.5;
        state.billies[d].acceleration = glm::vec3(0, 0, 0);
        state.billies[d].entityType = COLLECT;
    }

}

void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.objects, LEVEL1_OBJECT_COUNT);

    for (int j = 0; j < LEVEL1_OBJECT_COUNT; j++) {
        state.objects[j].Update(deltaTime, state.player, state.objects, LEVEL1_OBJECT_COUNT);
    }

    for (int b = 0; b < LEVEL1_THINGS; b++) {
        state.billies[b].Update(deltaTime, state.player, state.objects, LEVEL1_OBJECT_COUNT);
        if (state.billies[b].touched) {
            state.billies[b].touched = false;
            state.billies[b].isActive = false;
            Mix_PlayChannel(-1, state.nectar, 0);
        }
    }

    for (int n = LEVEL1_OPP_COUNT; n < LEVEL1_THINGS; n++) {
        if (!state.billies[n].isActive)
            state.cont -= 1.0f;
    }
    if (state.cont <= 0) {
        state.vic = true;
        return;
    }
}

void Level1::Render(ShaderProgram* program) {
    GLuint fontTextureID = Util::LoadTexture("font1.png");


    for (int i = 0; i < LEVEL1_OBJECT_COUNT; i++) {
        state.objects[i].Render(program);
    }

    for (int c = 0; c < LEVEL1_THINGS; c++) {
        state.billies[c].Render(program);
        if (state.billies[c].faneto)
            state.nextScene = 5;
    }

    //state.player->Render(&program);
}
