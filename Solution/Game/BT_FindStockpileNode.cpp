#include "stdafx.h"
#include "BT_FindStockpileNode.h"
#include <CE_ComponentFilter.h>
#include "InventoryComponent.h"
#include <CE_World.h>
#include "TranslationComponent.h"
#include <CE_BlackBoard.h>


BT_FindStockpileNode::BT_FindStockpileNode(CE_World& aWorld)
	: myWorld(aWorld)
{
}


eBTActionState BT_FindStockpileNode::Update(CE_Blackboard& aBlackboard, float /*aDelta*/)
{
	CE_GrowingArray<CE_Entity> entities = myWorld.GetEntities(CE_CreateFilter<CE_Requires<InventoryComponent, TranslationComponent>>());

	float bestDistance = FLT_MAX;
	CE_Vector3f bestPos;

	for (CE_Entity entity : entities)
	{
		const TranslationComponent& stockpileTranslate = myWorld.GetComponent<TranslationComponent>(entity);

		CE_Vector3f vec = stockpileTranslate.myOrientation.GetPos() - aBlackboard.myPosition;
		float dist = CE_Length2(vec);

		if (dist < bestDistance)
		{
			bestDistance = dist;
			bestPos = stockpileTranslate.myOrientation.GetPos();
		}
	}

	if (bestDistance == FLT_MAX)
		return FAILED;

	aBlackboard.myTargetPosition = bestPos;
	return FINISHED;
}
