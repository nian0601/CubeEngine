#pragma once
#include "GameContext.h"

class CE_Input;

class CUI_NodeEditor;

class NodeEditorContext : public GameContext
{
public:
	void Init(CE_Engine& anEngine) override;
	void Update(float aDelta) override;
	void Render() override;

private:
	CE_Input* myInput;

	CUI_NodeEditor* myNodeEditor;
};

