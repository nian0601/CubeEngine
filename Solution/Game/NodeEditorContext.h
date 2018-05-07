#pragma once
#include "GameContext.h"

class NodeEditorContext : public GameContext
{
public:
	NodeEditorContext();
	~NodeEditorContext();

	void Init(CE_Engine& anEngine) override;
	void Update(float aDelta) override;
	void Render() override;
};

