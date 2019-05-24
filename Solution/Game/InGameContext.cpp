#include "stdafx.h"

#define _USE_MATH_DEFINES
#include <math.h>
#define CU_PI_DIV_2 static_cast<float>(M_PI) / 2.f

#include "AABBProcessor.h"
#include "AIEventProcessor.h"
#include "BehaviorProcessor.h"
#include "CreateEntityProcessor.h"
#include "EntityFactory.h"
#include "InGameContext.h"
#include "InputProcessor.h"
#include "LifetimeProcessor.h"
#include "MovementProcessor.h"
#include "RenderComponent.h"
#include "RenderProcessor.h"
#include "RotationComponent.h"
#include "RotationProcessor.h"
#include "SelectionProcessor.h"
#include "TranslationComponent.h"

#include <CE_BlackBoard.h>
#include <CE_Camera.h>
#include <CE_DebugDraw.h>
#include <CE_Engine.h>
#include <CE_Input.h>
#include <CE_NavMesh.h>
#include <CE_PathFinder.h>
#include <CE_Window.h>
#include <CE_World.h>

#include <CPY_PhysicsWorld.h>

InGameContext::InGameContext()
{
}


InGameContext::~InGameContext()
{
	CE_SAFE_DELETE(myPathFinder);
	CE_SAFE_DELETE(myNavMesh);
	CE_SAFE_DELETE(myEntityFactory);
	CE_SAFE_DELETE(myWorld);
	CE_SAFE_DELETE(myPhysicsWorld);
	CE_SAFE_DELETE(myGlobalBlackboard);
}

void InGameContext::Init(CE_Engine& anEngine)
{
	myGlobalBlackboard = new CE_Blackboard();
	myPhysicsWorld = new CPY_PhysicsWorld();

	myWorld = new CE_World();
	myEntityFactory = new EntityFactory(*myWorld, anEngine.GetObjManager());

	CE_Window& mainWindow = anEngine.GetMainWindow();
	CE_Camera* camera = mainWindow.GetCamera();
	camera->SetPosition(CE_Vector3f(5.f, 10.f, -5.f));
	camera->Rotate(CE_Matrix44f::CreateRotateAroundX(3.14f * 0.25));

	myWorld->AddProcessor(new RenderProcessor(*myWorld, anEngine.GetRendererProxy()));
	myWorld->AddProcessor(new InputProcessor(*myWorld, anEngine.GetInput()));
	myWorld->AddProcessor(new CreateEntityProcessor(*myWorld, *myEntityFactory));
	myWorld->AddProcessor(new SelectionProcessor(*myWorld, *camera, *myPhysicsWorld));
	myWorld->AddProcessor(new AABBProcessor(*myWorld, *myPhysicsWorld));
	myWorld->AddProcessor(new BehaviorProcessor(*myWorld, *myGlobalBlackboard));

	myWorld->AddProcessor<MovementProcessor>();
	myWorld->AddProcessor<AIEventProcessor>();
	myWorld->AddProcessor<RotationProcessor>();
	myWorld->AddProcessor<LifetimeProcessor>();

	InitWorld();

	myNavMesh = new CE_NavMesh();
	myPathFinder = new CE_PathFinder(*myNavMesh);
	myGlobalBlackboard->Set("pathfinder", myPathFinder);
}

void InGameContext::Update(float aDelta)
{
	myWorld->Update(aDelta);
}

void InGameContext::Render()
{
	myNavMesh->DebugDraw();
}

void InGameContext::InitWorld()
{
	CE_Entity gatherer = myEntityFactory->InstansiateEntity("gatherer");
	TranslationComponent& gathererTranslate = myWorld->GetComponent<TranslationComponent>(gatherer);
	gathererTranslate.myOrientation.SetPos(CE_Vector3f(6.5f, 1.f, 1.f));

	RotationComponent& rotation = myWorld->AddComponent<RotationComponent>(gatherer);
	rotation.mySpeeds = CE_Vector3f(0.f);
	rotation.mySpeeds.y = 3.14f / 3.f;

	//InitWater();
	//InitStone();
	//InitStockpile();

	InitGrid();
}

void InGameContext::InitGrid()
{
	const int gridSize = 20;
	for (int z = 0; z < gridSize; ++z)
	{
		for (int x = 0; x < gridSize; ++x)
		{
			CE_Vector3f pos(static_cast<float>(x), -1.f, static_cast<float>(z));
	
			CE_Entity entity = myEntityFactory->InstansiateEntity("ground");
			TranslationComponent& translate = myWorld->GetComponent<TranslationComponent>(entity);
			translate.myOrientation.SetPos(pos);
		}
	}
}

void InGameContext::InitWater()
{
	CE_Entity water = myEntityFactory->InstansiateEntity("water");
	TranslationComponent& waterTranslate1 = myWorld->GetComponent<TranslationComponent>(water);
	waterTranslate1.myOrientation.SetPos(CE_Vector3f(5.f, 1.f, 5.f));

	water = myEntityFactory->InstansiateEntity("water");
	TranslationComponent& waterTranslate2 = myWorld->GetComponent<TranslationComponent>(water);
	waterTranslate2.myOrientation.SetPos(CE_Vector3f(5.f, 1.f, 4.f));

	water = myEntityFactory->InstansiateEntity("water");
	TranslationComponent& waterTranslate3 = myWorld->GetComponent<TranslationComponent>(water);
	waterTranslate3.myOrientation.SetPos(CE_Vector3f(4.f, 1.f, 4.f));
}

void InGameContext::InitStone()
{
	CE_Entity stone = myEntityFactory->InstansiateEntity("stone");
	TranslationComponent& stoneTranslate1 = myWorld->GetComponent<TranslationComponent>(stone);
	stoneTranslate1.myOrientation.SetPos(CE_Vector3f(1.f, 1.f, 5.f));

	stone = myEntityFactory->InstansiateEntity("stone");
	TranslationComponent& stoneTranslate2 = myWorld->GetComponent<TranslationComponent>(stone);
	stoneTranslate2.myOrientation.SetPos(CE_Vector3f(1.f, 1.f, 4.f));

	stone = myEntityFactory->InstansiateEntity("stone");
	TranslationComponent& stoneTranslate3 = myWorld->GetComponent<TranslationComponent>(stone);
	stoneTranslate3.myOrientation.SetPos(CE_Vector3f(2.f, 1.f, 4.f));
}

void InGameContext::InitStockpile()
{
	CE_Entity stockpile = myEntityFactory->InstansiateEntity("stockpile");
	TranslationComponent& stockpileTranslate1 = myWorld->GetComponent<TranslationComponent>(stockpile);
	stockpileTranslate1.myOrientation.SetPos(CE_Vector3f(1.f, 1.f, 1.f));

	stockpile = myEntityFactory->InstansiateEntity("stockpile");
	TranslationComponent& stockpileTranslate2 = myWorld->GetComponent<TranslationComponent>(stockpile);
	stockpileTranslate2.myOrientation.SetPos(CE_Vector3f(1.f, 1.f, 2.f));

	stockpile = myEntityFactory->InstansiateEntity("stockpile");
	TranslationComponent& stockpileTranslate3 = myWorld->GetComponent<TranslationComponent>(stockpile);
	stockpileTranslate3.myOrientation.SetPos(CE_Vector3f(2.f, 1.f, 2.f));

	stockpile = myEntityFactory->InstansiateEntity("stockpile");
	TranslationComponent& stockpileTranslate4 = myWorld->GetComponent<TranslationComponent>(stockpile);
	stockpileTranslate4.myOrientation.SetPos(CE_Vector3f(2.f, 1.f, 1.f));
}