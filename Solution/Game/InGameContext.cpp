#include "stdafx.h"

#include <CE_Camera.h>
#include <CE_World.h>
#include <CE_Engine.h>

#include "EntityFactory.h"
#include "InGameContext.h"

#include "TranslationComponent.h"

#include "AABBProcessor.h"
#include "AIEventProcessor.h"
#include "BehaviorProcessor.h"
#include "CreateEntityProcessor.h"
#include "InputProcessor.h"
#include "MovementProcessor.h"
#include "RenderProcessor.h"
#include "SelectionProcessor.h"

#include <CE_DebugDraw.h>

InGameContext::InGameContext()
{
	CE_TYPE_REGISTER(float);
	CE_TYPE_REGISTER(CE_Vector3f);
	CE_TYPE_REGISTER(CE_Vector4f);
	CE_TYPE_REGISTER(CE_World*);
	CE_TYPE_REGISTER(double);
}


InGameContext::~InGameContext()
{
	CE_SAFE_DELETE(myEntityFactory);
	CE_SAFE_DELETE(myTemplateWorld);
	CE_SAFE_DELETE(myWorld);
}

void InGameContext::Init(CE_Engine& anEngine)
{
	myWorld = new CE_World();
	myTemplateWorld = new CE_World();
	myEntityFactory = new EntityFactory(*myWorld, *myTemplateWorld);

	CE_Camera& camera = anEngine.GetCamera();
	camera.SetPosition(CE_Vector3f(5.f, 10.f, -5.f));
	camera.Rotate(CE_Matrix44f::CreateRotateAroundX(3.14f * 0.25));

	RenderProcessor* renderProcessor = new RenderProcessor(*myWorld, anEngine.GetRendererProxy());
	myWorld->AddProcessor(renderProcessor);

	InputProcessor* inputProcessor = new InputProcessor(*myWorld, anEngine.GetInput());
	myWorld->AddProcessor(inputProcessor);

	CreateEntityProcessor* createProcessor = new CreateEntityProcessor(*myWorld, *myEntityFactory);
	myWorld->AddProcessor(createProcessor);

	SelectionProcessor* selectionProcessor = new SelectionProcessor(*myWorld, camera);
	myWorld->AddProcessor(selectionProcessor);

	myWorld->AddProcessor<MovementProcessor>();
	myWorld->AddProcessor<AABBProcessor>();
	myWorld->AddProcessor<BehaviorProcessor>();
	myWorld->AddProcessor<AIEventProcessor>();

	InitWorld();

	CE_DRAW_LINE_TWO_COLOR(CE_Vector3f(0.f, 2.f, 0.f), CE_Vector3f(5.f, 2.f, 5.f), CE_Vector4f(1.f, 0.f, 0.f, 1.f), CE_Vector4f(0.f, 0.f, 1.f, 0.5f));
}

void InGameContext::Update(float aDelta)
{
	myWorld->Update(aDelta);
}

void InGameContext::Render()
{

}

void InGameContext::InitWorld()
{
	CE_Entity gatherer = myEntityFactory->InstansiateEntity(eEntityTypes::GATHERER);
	TranslationComponent& gathererTranslate = myWorld->GetComponent<TranslationComponent>(gatherer);
	gathererTranslate.myOrientation.SetPos(CE_Vector3f(7.f, 1.f, 1.f));

	InitWater();
	InitStone();
	InitStockpile();

	InitGrid();
}

void InGameContext::InitGrid()
{
	const int gridSize = 20;
	for (int z = 0; z < gridSize; ++z)
	{
		for (int x = 0; x < gridSize; ++x)
		{
			CE_Vector3f pos(static_cast<float>(x), 0.f, static_cast<float>(z));

			CE_Entity entity = myEntityFactory->InstansiateEntity(eEntityTypes::GROUND);
			TranslationComponent& translate = myWorld->GetComponent<TranslationComponent>(entity);
			translate.myOrientation.SetPos(pos);
		}
	}
}

void InGameContext::InitWater()
{
	CE_Entity water = myEntityFactory->InstansiateEntity(eEntityTypes::RESOURCE_WATER);
	TranslationComponent& waterTranslate1 = myWorld->GetComponent<TranslationComponent>(water);
	waterTranslate1.myOrientation.SetPos(CE_Vector3f(5.f, 1.f, 5.f));

	water = myEntityFactory->InstansiateEntity(eEntityTypes::RESOURCE_WATER);
	TranslationComponent& waterTranslate2 = myWorld->GetComponent<TranslationComponent>(water);
	waterTranslate2.myOrientation.SetPos(CE_Vector3f(5.f, 1.f, 4.f));

	water = myEntityFactory->InstansiateEntity(eEntityTypes::RESOURCE_WATER);
	TranslationComponent& waterTranslate3 = myWorld->GetComponent<TranslationComponent>(water);
	waterTranslate3.myOrientation.SetPos(CE_Vector3f(4.f, 1.f, 4.f));
}

void InGameContext::InitStone()
{
	CE_Entity stone = myEntityFactory->InstansiateEntity(eEntityTypes::RESOURCE_STONE);
	TranslationComponent& stoneTranslate1 = myWorld->GetComponent<TranslationComponent>(stone);
	stoneTranslate1.myOrientation.SetPos(CE_Vector3f(1.f, 1.f, 5.f));

	stone = myEntityFactory->InstansiateEntity(eEntityTypes::RESOURCE_STONE);
	TranslationComponent& stoneTranslate2 = myWorld->GetComponent<TranslationComponent>(stone);
	stoneTranslate2.myOrientation.SetPos(CE_Vector3f(1.f, 1.f, 4.f));

	stone = myEntityFactory->InstansiateEntity(eEntityTypes::RESOURCE_STONE);
	TranslationComponent& stoneTranslate3 = myWorld->GetComponent<TranslationComponent>(stone);
	stoneTranslate3.myOrientation.SetPos(CE_Vector3f(2.f, 1.f, 4.f));
}

void InGameContext::InitStockpile()
{
	CE_Entity stockpile = myEntityFactory->InstansiateEntity(eEntityTypes::STOCKPILE);
	TranslationComponent& stockpileTranslate1 = myWorld->GetComponent<TranslationComponent>(stockpile);
	stockpileTranslate1.myOrientation.SetPos(CE_Vector3f(1.f, 1.f, 1.f));

	stockpile = myEntityFactory->InstansiateEntity(eEntityTypes::STOCKPILE);
	TranslationComponent& stockpileTranslate2 = myWorld->GetComponent<TranslationComponent>(stockpile);
	stockpileTranslate2.myOrientation.SetPos(CE_Vector3f(1.f, 1.f, 2.f));

	stockpile = myEntityFactory->InstansiateEntity(eEntityTypes::STOCKPILE);
	TranslationComponent& stockpileTranslate3 = myWorld->GetComponent<TranslationComponent>(stockpile);
	stockpileTranslate3.myOrientation.SetPos(CE_Vector3f(2.f, 1.f, 2.f));

	stockpile = myEntityFactory->InstansiateEntity(eEntityTypes::STOCKPILE);
	TranslationComponent& stockpileTranslate4 = myWorld->GetComponent<TranslationComponent>(stockpile);
	stockpileTranslate4.myOrientation.SetPos(CE_Vector3f(2.f, 1.f, 1.f));
}