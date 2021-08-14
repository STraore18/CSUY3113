#include "Defeat.h"

void defeat::Initialize() {

}

void defeat::Update(float deltaTime) {

}

void defeat::Render(ShaderProgram* program) {
    GLuint fontTextureID = Util::LoadTexture("font1.png");

    Util::DrawText(program, fontTextureID, "You LOSE!!!",
        1.5f, -0.25f, glm::vec3(-2.5, 2, -9));
}