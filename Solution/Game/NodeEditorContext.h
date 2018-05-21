#pragma once
#include "GameContext.h"

class NodeEditorContext : public GameContext
{
public:
	void Init(CE_Engine& anEngine) override;
	void Update(float aDelta) override;
	void Render() override;
};

