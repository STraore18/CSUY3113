#include "Intro.h"

//GLuint fontTextureID;

void intro::Initialize() {
    state.nextScene = -1;
}

void intro::Update(float deltaTime) {

}

void intro::Render(ShaderProgram* program) {
    GLuint fontTextureID = Util::LoadTexture("font1.png");

    Util::DrawText(program, fontTextureID, "Rules and Instructions:",
        0.75f, -0.25f, glm::vec3(-5, 3, -9));

    Util::DrawText(program, fontTextureID, "W - Move forward",
        0.5f, -0.25f, glm::vec3(-5, 2, -9));

    Util::DrawText(program, fontTextureID, "S - Move backward",
        0.5f, -0.25f, glm::vec3(-5, 1, -9));

    Util::DrawText(program, fontTextureID, "A - Turn Left",
        0.5f, -0.25f, glm::vec3(-5, 0, -9));

    Util::DrawText(program, fontTextureID, "D - Turn Right",
        0.5f, -0.25f, glm::vec3(-5, -1, -9));

    Util::DrawText(program, fontTextureID, "Collect all the sweets to WIN!",
        0.5f, -0.25f, glm::vec3(-5, -2.0, -9));

    Util::DrawText(program, fontTextureID, "Let the skulls touch you and you LOSE!!",
        0.5f, -0.25f, glm::vec3(-5, -2.5, -9));

    Util::DrawText(program, fontTextureID, "Press Space to Start!",
        0.45f, -0.25f, glm::vec3(-1.5, -3.5, -10));


}