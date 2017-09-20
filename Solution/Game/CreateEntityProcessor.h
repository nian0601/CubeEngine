#pragma once

#include <CE_BaseProcessor.h>

class EntityFactory;
class CreateEntityProcessor : public CE_BaseProcessor
{
public:
	CreateEntityProcessor(CE_World& aWorld, EntityFactory& aEntityFactory);

	void Update(float aDelta) override;

private:
	EntityFactory& myEntityFactory;
};

