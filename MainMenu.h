#include "Scene.h"
class MainM : public Scene {

public:
	void Initialize() override;
	void Update(float deltaTime) override;
	void Render(ShaderProgram* program,int rar) override;
};
