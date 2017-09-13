#pragma once

class CE_Engine;
class CE_GPUContext;

class CE_Game
{
public:
	virtual void Init(CE_Engine& anEngine) = 0;
	virtual void Update() = 0;
	virtual void Render(const CE_GPUContext& anGPUContext) = 0;

protected:
	CE_Game();
	virtual ~CE_Game();
};

