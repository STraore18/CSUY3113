#pragma once
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
#include "Map.h"

enum EntityType { PLAYER, PLATFORM, ENEMY };
enum AIType { WALKER, WAITNGO, GHOST };
enum AIState { IDLE, WALKING, DROP, RISE, RESET };

class Entity {
public:
    EntityType entityType; 
    AIType kind;
    AIState cond;

    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;

    float width = 1;
    float height = 1;

    bool jump = false;
    float jumpPOW = 0;

    float speed;
    
    GLuint textureID;

    
    glm::mat4 modelMatrix;
    
    int *animDown = NULL;
    int *animLeft = NULL;
    int* animUp = NULL;
    int* animRight = NULL;

    int *animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;

    bool isActive = true;

    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedRight = false;
    bool collidedLeft = false;


    
    Entity();
    
    bool CheckCollision(Entity* other);
    void CheckCollisionsY(Entity* objects, int objectCount);
    void CheckCollisionsX(Entity* objects, int objectCount);
    void CheckCollisionsY(Map* map);
    void CheckCollisionsX(Map* map);

    void Update(float deltaTime, Entity* player, Entity* objects, int objectCount, Map* map);
    void Render(ShaderProgram *program);
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index);

    void AI(Entity* player);
    void AIWalker(Entity* player);
    void AIWaitngo(Entity* player);
    void AIGhost(Entity* player);
};
