#pragma once

class CE_Engine;

class GameContext
{
public:
	virtual ~GameContext() {};

	virtual void Init(CE_Engine& anEngine) = 0;
	virtual void Update(float aDelta) = 0;
	virtual void Render() = 0;

protected:
	GameContext() {};
};

