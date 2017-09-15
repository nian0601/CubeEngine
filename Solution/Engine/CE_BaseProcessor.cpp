#include "stdafx.h"
#include "CE_BaseProcessor.h"
#include "CE_World.h"


CE_BaseProcessor::CE_BaseProcessor(CE_World& aWorld)
	: myWorld(aWorld)
{
}


CE_BaseProcessor::CE_BaseProcessor(CE_World& aWorld, const CE_ComponentFilter& aFilter)
	: myWorld(aWorld)
	, myComponentFilter(aFilter)
{

}

CE_BaseProcessor::~CE_BaseProcessor()
{
}

const CE_GrowingArray<CE_Entity>& CE_BaseProcessor::GetEntities()
{
	return myWorld.GetEntities(myComponentFilter);
}

void CE_BaseProcessor::OnEvent(const CE_Event&)
{
}