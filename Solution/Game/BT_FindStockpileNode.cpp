#include "stdafx.h"
#include "BT_FindStockpileNode.h"
#include <CE_ComponentFilter.h>
#include "InventoryComponent.h"
#include <CE_World.h>
#include "TranslationComponent.h"
#include <CE_BlackBoard.h>

eBTActionState BT_FindStockpileNode::Update(CE_Blackboard& aBlackboard, float /*aDelta*/)
{
	CE_Vector3f selfPosition;

	if (!aBlackboard.Get("selfPosition", selfPosition))
		return FAILED;

	CE_World* world = nullptr;
	if (!aBlackboard.Get("world", world))
		return FAILED;

	CE_GrowingArray<CE_Entity> entities = world->GetEntities(CE_CreateFilter<CE_Requires<InventoryComponent, TranslationComponent>>());

	float bestDistance = FLT_MAX;
	CE_Vector3f bestPos;

	for (CE_Entity entity : entities)
	{
		const TranslationComponent& stockpileTranslate = world->GetComponent<TranslationComponent>(entity);
		
		CE_Vector3f vec = stockpileTranslate.myOrientation.GetPos() - selfPosition;
		float dist = CE_Length2(vec);

		if (dist < bestDistance)
		{
			bestDistance = dist;
			bestPos = stockpileTranslate.myOrientation.GetPos();
		}
	}

	if (bestDistance == FLT_MAX)
		return FAILED;

	aBlackboard.Set("targetPosition", bestPos);
	return FINISHED;
}
