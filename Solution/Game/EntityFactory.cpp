#include "stdafx.h"

#include "EntityFactory.h"
#include "TranslationComponent.h"
#include "RenderComponent.h"
#include "MovementComponent.h"
#include "InventoryComponent.h"
#include "AABBComponent.h"
#include "ResourceComponent.h"
#include "BehaviorComponent.h"

#include <CE_BehaviorTree.h>
#include <CE_BTMoveToNode.h>
#include <CE_BlackBoard.h>
#include <CE_BTInitNode.h>
#include <CE_BTSequenceNode.h>
#include <CE_FileParser.h>
#include <CE_FileSystem.h>

#include "BT_FindStockpileNode.h"
#include "BT_GatherResourceNode.h"
#include "LifetimeComponent.h"
#include <CE_ObjManager.h>

EntityFactory::EntityFactory(CE_World& anRealWorld)
	: myRealWorld(anRealWorld)
{
	myTemplateWorld = new CE_World();
	LoadTemplateEntities();
}


EntityFactory::~EntityFactory()
{
	CE_SAFE_DELETE(myTemplateWorld);
}

void EntityFactory::LoadTemplateEntities()
{
	CE_GrowingArray<CE_FileSystem::FileInfo> entityFiles;
	CE_FileSystem::GetAllFilesFromDirectory("Data/Entities", entityFiles);

	for (const CE_FileSystem::FileInfo& file : entityFiles)
		myEntityMap[file.myFileNameNoExtention] = LoadFromDisk(file.myFilePath.c_str());
}

CE_Entity EntityFactory::InstansiateEntity(const CE_String& anIdentifier)
{
	CE_ASSERT(myEntityMap.KeyExists(anIdentifier), "Couldnt find Entity %i to Instansiate");

	CE_Entity templateEntity = myEntityMap[anIdentifier];
	CE_Entity newEntity = myRealWorld.CreateEmptyEntity();

	CopyComponent<MovementComponent>(templateEntity, newEntity);
	CopyComponent<InventoryComponent>(templateEntity, newEntity);
	CopyComponent<RenderComponent>(templateEntity, newEntity);
	CopyComponent<TranslationComponent>(templateEntity, newEntity);
	CopyComponent<AABBComponent>(templateEntity, newEntity);
	CopyComponent<ResourceComponent>(templateEntity, newEntity);
	CopyComponent<BehaviorComponent>(templateEntity, newEntity);
	CopyComponent<LifetimeComponent>(templateEntity, newEntity);

	return newEntity;
}

CE_Entity EntityFactory::LoadFromDisk(const char* aFilePath)
{
	CE_Entity entity = myTemplateWorld->CreateEmptyEntity();

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
		if (words[0] == "#resource")
			LoadResourceComponent(anEntity, aFileParser);
		if (words[0] == "#behavior")
			LoadBehaviorComponent(anEntity, aFileParser);
		if (words[0] == "#lifetime")
			LoadLifetimeComponent(anEntity, aFileParser);
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
		else if (words[0] == "#translate")
		{
			CE_ASSERT(openEntry == true, "Need '#entry' before you can specify a translate");

			CE_Vector3f translate;
			translate.x = aFileParser.GetFloat(words[1]);
			translate.y = aFileParser.GetFloat(words[2]);
			translate.z = aFileParser.GetFloat(words[3]);

			RenderComponent::Entry& entry = entries.GetLast();
			entry.myOffsetMatrix.SetPos(translate);
		}
		else if (words[0] == "#type")
		{
			CE_ASSERT(openEntry == true, "Need '#entry' before you can specify a type");

			RenderComponent::Entry& entry = entries.GetLast();
			entry.myType = ModelType::FromString(words[1]);
		}
		else if (words[0] == "#metalness")
		{
			CE_ASSERT(openEntry == true, "Need '#entry' before you can specify a metalness");

			RenderComponent::Entry& entry = entries.GetLast();
			entry.myMetalness = aFileParser.GetFloat(words[1]);
		}
		else if (words[0] == "#roughness")
		{
			CE_ASSERT(openEntry == true, "Need '#entry' before you can specify a roughness");

			RenderComponent::Entry& entry = entries.GetLast();
			entry.myRoughness= aFileParser.GetFloat(words[1]);
		}
		else if (words[0] == "#model")
		{
			CE_ASSERT(openEntry == true, "Need '#entry' before you can specify a model");

			RenderComponent::Entry& entry = entries.GetLast();
			entry.myModelID = CE_ObjManager::GetInstance()->GetObjID(words[1].c_str());
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

	RenderComponent& render = myTemplateWorld->AddComponent<RenderComponent>(anEntity);
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

	MovementComponent& movement = myTemplateWorld->AddComponent<MovementComponent>(anEntity);
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

	TranslationComponent& translation = myTemplateWorld->AddComponent<TranslationComponent>(anEntity);
	translation.myScale = scale;
}

void EntityFactory::LoadInventoryComponent(CE_Entity anEntity, CE_FileParser& aFileParser)
{
	LoadEmptyComponent(aFileParser);

	myTemplateWorld->AddComponent<InventoryComponent>(anEntity);
}

void EntityFactory::LoadAABBComponent(CE_Entity anEntity, CE_FileParser& aFileParser)
{
	CollisionLayer::eLayer layers = CollisionLayer::eLayer::NONE;
	CollisionLayer::eLayer collidesWith = CollisionLayer::eLayer::NONE;

	CE_String line;
	CE_GrowingArray<CE_String> words;

	while (aFileParser.ReadLine(line))
	{
		aFileParser.TrimBeginAndEnd(line);
		aFileParser.SplitLine(line, words);

		if (words[0] == "#layer")
		{
			layers = CollisionLayer::FromString(words[1]);
		}
		else if (words[0] == "#collideswith")
		{
			collidesWith = CollisionLayer::FromString(words[1]);
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

	AABBComponent& aabb = myTemplateWorld->AddComponent<AABBComponent>(anEntity);
	aabb.myCollisionLayers = layers;
	aabb.myCollidesWith = collidesWith;
}

void EntityFactory::LoadResourceComponent(CE_Entity anEntity, CE_FileParser& aFileParser)
{
	ResourceType::eType resource = ResourceType::eType::INVALID;

	CE_String line;
	CE_GrowingArray<CE_String> words;

	while (aFileParser.ReadLine(line))
	{
		aFileParser.TrimBeginAndEnd(line);
		aFileParser.SplitLine(line, words);

		if (words[0] == "#type")
		{
			resource = ResourceType::FromString(words[1]);
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

	CE_ASSERT(resource != ResourceType::eType::INVALID, "Invalid Resource type");

	ResourceComponent& resourceComponent = myTemplateWorld->AddComponent<ResourceComponent>(anEntity);
	resourceComponent.myResourceType = resource;
}

void EntityFactory::LoadBehaviorComponent(CE_Entity anEntity, CE_FileParser& aFileParser)
{
	CE_String line;
	CE_GrowingArray<CE_String> words;

	float speed = 0.f;

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

	BehaviorComponent& behavior = myTemplateWorld->AddComponent<BehaviorComponent>(anEntity);

	behavior.myBehaviorTree = new CE_BehaviorTree();
	CE_BTInitNode& initNode = behavior.myBehaviorTree->GetInitNode();

	CE_BTSequenceNode* sequence = new CE_BTSequenceNode();
	initNode.SetChildNode(sequence);

	sequence->AddChild(new CE_BTMoveToNode());
	sequence->AddChild(new BT_GatherResourceNode());
	sequence->AddChild(new BT_FindStockpileNode());
	sequence->AddChild(new CE_BTMoveToNode());

	CE_Blackboard& blackboard = behavior.myBehaviorTree->GetBlackboard();
	blackboard.Set("speed", speed);
	blackboard.Set("world", &myRealWorld);
}

void EntityFactory::LoadLifetimeComponent(CE_Entity anEntity, CE_FileParser& aFileParser)
{
	CE_String line;
	CE_GrowingArray<CE_String> words;

	float timeToLive = 0.f;
	while (aFileParser.ReadLine(line))
	{
		aFileParser.TrimBeginAndEnd(line);
		aFileParser.SplitLine(line, words);

		if (words[0] == "#timetolive")
		{
			timeToLive = aFileParser.GetFloat(words[1]);
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

	LifetimeComponent& lifetimeComponent = myTemplateWorld->AddComponent<LifetimeComponent>(anEntity);
	lifetimeComponent.myTimeToLive = timeToLive;
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