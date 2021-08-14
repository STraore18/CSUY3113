#include "Level2.h"
#define LEVEL2_WIDTH 14
#define LEVEL2_HEIGHT 8
static int LEVEL2_OPP_COUNT = 1;
static int LEVEL2_OBJECT_COUNT = 4;
static int LEVEL2_COLLECTIBLES = 4;
static int LEVEL2_THINGS = LEVEL2_OPP_COUNT + LEVEL2_COLLECTIBLES;


void Level2::Initialize() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    state.nextScene = -1;

    state.cont = 0;

    state.nectar = Mix_LoadWAV("honey.wav");

    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(0, 0.25f, -2);
    //state.player->acceleration = glm::vec3(0, -9.81, 0);
    state.player->speed = 1.0f;

    state.objects = new Entity[LEVEL2_OBJECT_COUNT];


    GLuint floorTextureID = Util::LoadTexture("forest.png");
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
    Mesh* crateMesh = new Mesh();
    crateMesh->LoadOBJ("cube.obj", 1);

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


    state.billies = new Entity[LEVEL2_THINGS];

    GLuint enemyTextureID = Util::LoadTexture("skull.gif");
    for (int i = 0; i < LEVEL2_OPP_COUNT; i++) {
        state.billies[i].billboard = true;
        state.billies[i].textureID = enemyTextureID;
        if (i < (LEVEL2_OPP_COUNT / 2)) {
            state.billies[i].position = glm::vec3(rand() % 9, 0.5, -5 - (rand() % 15));
        }
        else {
            state.billies[i].position = glm::vec3((0 - rand() % 9), 0.5, -5 - (rand() % 15));
        }
        state.billies[i].rotation = glm::vec3(0, 0, 0);
        state.billies[i].acceleration = glm::vec3(0, 0, 0);
        state.billies[i].entityType = ENEMY;
    }

    GLuint itemTextureID = Util::LoadTexture("honey.png");

    for (int d = LEVEL2_OPP_COUNT; d < LEVEL2_THINGS; d++) {
        state.billies[d].billboard = true;
        state.billies[d].textureID = itemTextureID;
        if (d < (LEVEL2_OPP_COUNT + (LEVEL2_COLLECTIBLES / 2))) {
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

void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.objects, LEVEL2_OBJECT_COUNT);

    for (int j = 0; j < LEVEL2_OBJECT_COUNT; j++) {
        state.objects[j].Update(deltaTime, state.player, state.objects, LEVEL2_OBJECT_COUNT);
    }

    for (int b = 0; b < LEVEL2_THINGS; b++) {
        state.billies[b].Update(deltaTime, state.player, state.objects, LEVEL2_OBJECT_COUNT);
        if (state.billies[b].touched) {
            state.billies[b].touched = false;
            state.billies[b].isActive = false;
            Mix_PlayChannel(-1, state.nectar, 0);
        }

    }
}

void Level2::Render(ShaderProgram* program) {
    for (int i = 0; i < LEVEL2_OBJECT_COUNT; i++) {
        state.objects[i].Render(program);
    }

    for (int c = 0; c < LEVEL2_THINGS; c++) {
        state.billies[c].Render(program);
    }

    //state.player->Render(&program);
}
