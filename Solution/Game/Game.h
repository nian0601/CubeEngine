#pragma once

#include <CE_Game.h>


class CE_Shader;
class CE_Model;
class CE_Camera;

class Game : public CE_Game
{
public:
	Game();
	~Game();

	void Init(CE_Engine& anEngine) override;
	void Update() override;
	void Render(const CE_GPUContext& anGPUContext) override;

private:
	CE_Shader* myShader;
	CE_Model* myCube1;
	CE_Model* myCube2;
	CE_Camera* myCamera;
};