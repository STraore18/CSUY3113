#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity=glm::vec3(0);
    speed = 0;
    
    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity* other) {
    if (isActive == false || other->isActive == false)return false;
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);

    if (xdist < 0 && ydist < 0) {
        return true;
    }
    return false;
}

void Entity::CheckCollisionsY(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CheckCollision(object))
        {
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                if ((entityType == PLAYER) && (object->entityType == ENEMY)) { isActive = false; }
                collidedTop = true;
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                if ((entityType == PLAYER) && (object->entityType == ENEMY)) object->isActive = false;
                collidedBottom = true;
            }
        }
    }
}

void Entity::CheckCollisionsX(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CheckCollision(object))
        {
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) {
                position.x -= penetrationX;
                velocity.x = 0;
                if ((entityType == PLAYER) && (object->entityType == ENEMY)) isActive = false;
                collidedRight = true;
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                if ((entityType == PLAYER) && (object->entityType == ENEMY)) isActive = false;
                collidedLeft = true;
            }
        }
    }
}

void Entity::Update(float deltaTime, Entity* player, Entity* platforms,Entity* enemies,
    int platformCount,int enemyCount)
{
    if (isActive == false)return;

    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;

    if (entityType == ENEMY) {
        AI(player);
    }

    if (animIndices != NULL) {
        if (glm::length(movement) != 0) {
            animTime += deltaTime;

            if (animTime >= 0.25f)
            {
                animTime = 0.0f;
                animIndex++;
                if (animIndex >= animFrames)
                {
                    animIndex = 0;
                }
            }
        } else {
            animIndex = 0;
        }
    }

    if (jump) {
        jump = false;
        velocity.y += jumpPOW;
   }
    
    velocity.x = movement.x * speed;
    velocity += acceleration * deltaTime;
    
    position.y += velocity.y * deltaTime; // Move on Y
    CheckCollisionsY(platforms, platformCount);// Fix if needed
    if (entityType == PLAYER)CheckCollisionsY(enemies, enemyCount);

    position.x += velocity.x * deltaTime; // Move on X
    CheckCollisionsX(platforms, platformCount);// Fix if needed
    if (entityType == PLAYER)CheckCollisionsX(enemies, enemyCount);


    if (kind == GHOST&&collidedBottom) 
        cond = RISE;

    if (kind == WALKER) 
        if (position.x >= 9.5f) {
            position.x=-9.5f;
            cond = RESET;
        }

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);


}

void Entity::AIWalker(Entity* player) {
    switch (cond) {
        case IDLE:
            movement.x = 1.0f;
        case RESET:
            cond = IDLE;
    }
}

void Entity::AIWaitngo(Entity* player) {
    switch (cond) {
    case IDLE:
        if (glm::distance(position,player->position)<5.0f) {
            cond = WALKING;
        }
        break;
    case WALKING:
        if (player->position.x < position.x)
            movement = glm::vec3(-1, 0, 0);
        else
            movement = glm::vec3(1, 0, 0);
        break;
    }
}

void Entity::AIGhost(Entity* player) {
    switch (cond) {
    case IDLE:
        velocity = glm::vec3(0);
        acceleration = glm::vec3(0);
        if (glm::distance(position, player->position) < 7.0f) {
            cond = DROP;
        }
        break;
    case DROP:
        acceleration = glm::vec3(0, -17, 0);
        break;
    case RISE:
        acceleration = glm::vec3(0, 7, 0);
        if (position.y >= 6.6) {
            cond = IDLE;
            position.y -= 0.1;
        }
        break;
    }
};


void Entity::AI(Entity* player) {
    switch (kind) {
    case WALKER:
        AIWalker(player);
        break;
    case WAITNGO:
        AIWaitngo(player);
        break;
    case GHOST:
        AIGhost(player);
        break;
    }
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;
    
    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;
    
    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v};
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram *program) {
    program->SetModelMatrix(modelMatrix);
    if (isActive == false)return;
    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
