#include "Victory.h"

void victory::Initialize() {
    state.nextScene = -1;
}

void victory::Update(float deltaTime) {

}

void victory::Render(ShaderProgram* program) {
    GLuint fontTextureID = Util::LoadTexture("font1.png");

    Util::DrawText(program, fontTextureID, "You WIN!!!",
        1.5f, -0.25f, glm::vec3(-3.5, 2, -9));
}