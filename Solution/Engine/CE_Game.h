#pragma once

class CE_Engine;

class CE_Game
{
public:
	virtual ~CE_Game() {};
	virtual void Init(CE_Engine& anEngine) = 0;
	virtual void Update(float aDelta) = 0;
	virtual void Render() = 0;

protected:
	CE_Game() {};
};

