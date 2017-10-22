#pragma once

#include <CE_BaseComponent.h>
#include <CE_BehaviorTree.h>

struct BehaviorComponent : public CE_BaseComponent
{
	CE_BehaviorTree* myBehaviorTree;
};
