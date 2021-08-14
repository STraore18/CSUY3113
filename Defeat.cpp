#include "Defeat.h"

void defeat::Initialize() {
    state.nextScene = -1;
}

void defeat::Update(float deltaTime) {

}

void defeat::Render(ShaderProgram* program) {
    GLuint fontTextureID = Util::LoadTexture("font1.png");

    Util::DrawText(program, fontTextureID, "You LOSE!!!",
        0.75f, -0.25f, glm::vec3(-1.5, 0, -9));
}