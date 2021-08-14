#include "MainMenu.h"


GLuint fontTextureID;

void MainM::Initialize() {
    state.nextScene = -1;
}

void MainM::Update(float deltaTime) {

}

void MainM::Render(ShaderProgram* program) {
    GLuint fontTextureID = Util::LoadTexture("font1.png");

    Util::DrawText(program, fontTextureID, "Hector",
        1.5f, -0.25f, glm::vec3(-2.5, 2, -9));

    Util::DrawText(program, fontTextureID, "The Nectar Collector",
        0.75f, -0.25f, glm::vec3(-4.5, 1, -9));

    Util::DrawText(program, fontTextureID, "Press Enter to Start!",
        0.45f, -0.25f, glm::vec3(-1.5, -2, -10));


}