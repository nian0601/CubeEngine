#pragma once

class CE_Engine;
class CE_RendererProxy;

class CE_Game
{
public:
	virtual void Init(CE_Engine& anEngine) = 0;
	virtual void Update(float aDelta) = 0;
	virtual void Render(CE_RendererProxy& anRendererProxy) = 0;

protected:
	CE_Game() {};
	virtual ~CE_Game() {};
};

