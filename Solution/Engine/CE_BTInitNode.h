#pragma once
#include "CE_BTNode.h"

class CE_BTInitNode : public CE_BTNode
{
public:
	CE_BTInitNode();
	~CE_BTInitNode();

	virtual eBTActionState Update() override;

private:
	CE_BTNode* myChildNode;
};

