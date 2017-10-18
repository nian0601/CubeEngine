#include "stdafx.h"
#include "EntityFactory.h"
#include "TranslationComponent.h"
#include "RenderComponent.h"
#include "MovementComponent.h"
#include "InventoryComponent.h"
#include "PickUpComponent.h"
#include "MoverComponent.h"
#include "AABBComponent.h"
#include <CE_FileParser.h>


EntityFactory::EntityFactory(CE_World& anRealWorld, CE_World& anTemplateWorld)
	: myRealWorld(anRealWorld)
	, myTemplateWorld(anTemplateWorld)
{
	LoadTemplateEntities();
}


EntityFactory::~EntityFactory()
{
}

void EntityFactory::LoadTemplateEntities()
{
	myTemplateEntityMap[static_cast<int>(eEntityTypes::GROUND)] = LoadFromDisk("Data/Entities/ground.ce_entity");
	myTemplateEntityMap[static_cast<int>(eEntityTypes::PLAYER)] = LoadFromDisk("Data/Entities/player.ce_entity");
	myTemplateEntityMap[static_cast<int>(eEntityTypes::PICK_UP)] = LoadFromDisk("Data/Entities/pickup.ce_entity");
	myTemplateEntityMap[static_cast<int>(eEntityTypes::MOVER)] = LoadFromDisk("Data/Entities/mover.ce_entity");
}

CE_Entity EntityFactory::InstansiateEntity(eEntityTypes anIdentifier)
{
	int intID = static_cast<int>(anIdentifier);
	CE_ASSERT(myTemplateEntityMap.KeyExists(intID), "Couldnt find Entity %i to Instansiate");

	CE_Entity templateEntity = myTemplateEntityMap[intID];
	CE_Entity newEntity = myRealWorld.CreateEmptyEntity();

	CopyComponent<MovementComponent>(templateEntity, newEntity);
	CopyComponent<InventoryComponent>(templateEntity, newEntity);
	CopyComponent<PickUpComponent>(templateEntity, newEntity);
	CopyComponent<RenderComponent>(templateEntity, newEntity);
	CopyComponent<TranslationComponent>(templateEntity, newEntity);
	CopyComponent<MoverComponent>(templateEntity, newEntity);
	CopyComponent<AABBComponent>(templateEntity, newEntity);

	return newEntity;
}

CE_Entity EntityFactory::LoadFromDisk(const char* aFilePath)
{
	CE_Entity entity = myTemplateWorld.CreateEmptyEntity();

	CE_FileParser parser(aFilePath);

	CE_String line;
	CE_GrowingArray<CE_String> words;

	while (parser.ReadLine(line))
	{
		parser.TrimBeginAndEnd(line);
		parser.SplitLine(line, words);

		if (words[0] == "#components")
			LoadComponents(entity, parser);
	}
	
	return entity;
}

void EntityFactory::LoadComponents(CE_Entity anEntity, CE_FileParser& aFileParser)
{
	CE_String line;
	CE_GrowingArray<CE_String> words;

	while (aFileParser.ReadLine(line))
	{
		aFileParser.TrimBeginAndEnd(line);
		aFileParser.SplitLine(line, words);

		if (words[0] == "#render")
			LoadRenderComponent(anEntity, aFileParser);
		if (words[0] == "#movement")
			LoadMovementComponent(anEntity, aFileParser);
		if (words[0] == "#translate")
			LoadTranslateComponent(anEntity, aFileParser);
		if (words[0] == "#inventory")
			LoadInventoryComponent(anEntity, aFileParser);
		if (words[0] == "#aabb")
			LoadAABBComponent(anEntity, aFileParser);
		if (words[0] == "#pickup")
			LoadPickupComponent(anEntity, aFileParser);
		if (words[0] == "#mover")
			LoadMoverComponent(anEntity, aFileParser);
	}

}

void EntityFactory::LoadRenderComponent(CE_Entity anEntity, CE_FileParser& aFileParser)
{
	CE_GrowingArray<RenderComponent::Entry> entries;
	bool openEntry = false;

	CE_String line;
	CE_GrowingArray<CE_String> words;

	while (aFileParser.ReadLine(line))
	{
		aFileParser.TrimBeginAndEnd(line);
		aFileParser.SplitLine(line, words);

		if (words[0] == "#color")
		{
			CE_ASSERT(openEntry == true, "Need '#entry' before you can specify a color");

			RenderComponent::Entry& entry = entries.GetLast();
			entry.myColor.x = aFileParser.GetFloat(words[1]);
			entry.myColor.y = aFileParser.GetFloat(words[2]);
			entry.myColor.z = aFileParser.GetFloat(words[3]);
			entry.myColor.w = aFileParser.GetFloat(words[4]);
		}
		else if (words[0] == "#scale")
		{
			CE_ASSERT(openEntry == true, "Need '#entry' before you can specify a scale");

			RenderComponent::Entry& entry = entries.GetLast();
			entry.myScale.x = aFileParser.GetFloat(words[1]);
			entry.myScale.y = aFileParser.GetFloat(words[2]);
			entry.myScale.z = aFileParser.GetFloat(words[3]);
		}
		else if (words[0] == "#entry")
		{
			CE_ASSERT(openEntry == false, "Dont support nested '#entry' yet");
			openEntry = true;
			entries.Add();
		}
		else if (words[0] == "#end")
		{
			if (openEntry)
			{
				openEntry = false;
				continue;
			}
			break;
		}
		else
		{
			CE_ASSERT_ALWAYS("Unsupported Component-data");
		}
	}

	RenderComponent& render = myTemplateWorld.AddComponent<RenderComponent>(anEntity);
	render.myEntries = entries;
}

void EntityFactory::LoadMovementComponent(CE_Entity anEntity, CE_FileParser& aFileParser)
{
	float speed = 0.f;;

	CE_String line;
	CE_GrowingArray<CE_String> words;

	while (aFileParser.ReadLine(line))
	{
		aFileParser.TrimBeginAndEnd(line);
		aFileParser.SplitLine(line, words);

		if (words[0] == "#speed")
		{
			speed = aFileParser.GetFloat(words[1]);
		}
		else if (words[0] == "#end")
		{
			break;
		}
		else
		{
			CE_ASSERT_ALWAYS("Unsupported Component-data");
		}
	}

	MovementComponent& movement = myTemplateWorld.AddComponent<MovementComponent>(anEntity);
	movement.mySpeed = speed;
}

void EntityFactory::LoadTranslateComponent(CE_Entity anEntity, CE_FileParser& aFileParser)
{
	CE_String line;
	CE_GrowingArray<CE_String> words;

	CE_Vector3f scale(1.f);
	while (aFileParser.ReadLine(line))
	{
		aFileParser.TrimBeginAndEnd(line);
		aFileParser.SplitLine(line, words);

		if (words[0] == "#scale")
		{
			scale.x = aFileParser.GetFloat(words[1]);
			scale.y = aFileParser.GetFloat(words[2]);
			scale.z = aFileParser.GetFloat(words[3]);
		}
		else if (words[0] == "#end")
		{
			break;
		}
		else
		{
			CE_ASSERT_ALWAYS("Unsupported Component-data");
		}
	}

	TranslationComponent& translation = myTemplateWorld.AddComponent<TranslationComponent>(anEntity);
	translation.myScale = scale;
}

void EntityFactory::LoadInventoryComponent(CE_Entity anEntity, CE_FileParser& aFileParser)
{
	LoadEmptyComponent(aFileParser);

	myTemplateWorld.AddComponent<InventoryComponent>(anEntity);
}

void EntityFactory::LoadAABBComponent(CE_Entity anEntity, CE_FileParser& aFileParser)
{
	eCollisionLayer layers = eCollisionLayer::NONE;
	eCollisionLayer collidesWith = eCollisionLayer::NONE;

	CE_String line;
	CE_GrowingArray<CE_String> words;

	while (aFileParser.ReadLine(line))
	{
		aFileParser.TrimBeginAndEnd(line);
		aFileParser.SplitLine(line, words);

		if (words[0] == "#layer")
		{
			layers = ConvertStringToCollisionLayer(words[1]);
		}
		else if (words[0] == "#collideswith")
		{
			collidesWith = ConvertStringToCollisionLayer(words[1]);
		}
		else if (words[0] == "#end")
		{
			break;
		}
		else
		{
			CE_ASSERT_ALWAYS("Unsupported Component-data");
		}
	}

	AABBComponent& aabb = myTemplateWorld.AddComponent<AABBComponent>(anEntity);
	aabb.myCollisionLayers = layers;
	aabb.myCollidesWith = collidesWith;
}

void EntityFactory::LoadPickupComponent(CE_Entity anEntity, CE_FileParser& aFileParser)
{
	LoadEmptyComponent(aFileParser);

	myTemplateWorld.AddComponent<PickUpComponent>(anEntity);
}

void EntityFactory::LoadMoverComponent(CE_Entity anEntity, CE_FileParser& aFileParser)
{
	float speed = 0.f;
	CE_Vector3f direction;

	CE_String line;
	CE_GrowingArray<CE_String> words;

	while (aFileParser.ReadLine(line))
	{
		aFileParser.TrimBeginAndEnd(line);
		aFileParser.SplitLine(line, words);

		if (words[0] == "#speed")
		{
			speed = aFileParser.GetFloat(words[1]);
		}
		else if (words[0] == "#direction")
		{
			direction.x = aFileParser.GetFloat(words[1]);
			direction.y = aFileParser.GetFloat(words[2]);
			direction.z = aFileParser.GetFloat(words[3]);
		}
		else if (words[0] == "#end")
		{
			break;
		}
		else
		{
			CE_ASSERT_ALWAYS("Unsupported Component-data");
		}
	}

	MoverComponent& mover = myTemplateWorld.AddComponent<MoverComponent>(anEntity);
	mover.mySpeed = speed;
	mover.myDirection = direction;
}

void EntityFactory::LoadEmptyComponent(CE_FileParser& aFileParser)
{
	CE_String line;
	CE_GrowingArray<CE_String> words;

	while (aFileParser.ReadLine(line))
	{
		aFileParser.TrimBeginAndEnd(line);
		aFileParser.SplitLine(line, words);

		if (words[0] == "#end")
		{
			break;
		}
		else
		{
			CE_ASSERT_ALWAYS("Unsupported Component-data");
		}
	}
}