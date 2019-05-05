#include "stdafx.h"

#include "EntityFactory.h"
#include "LightingTestContext.h"
#include "RenderComponent.h"
#include "RenderProcessor.h"
#include "TranslationComponent.h"

#include <CE_Camera.h>
#include <CE_Engine.h>
#include <CE_Input.h>
#include <CE_Window.h>
#include <CE_World.h>

LightingTestContext::LightingTestContext()
{
}


LightingTestContext::~LightingTestContext()
{
	CE_SAFE_DELETE(myEntityFactory);
	CE_SAFE_DELETE(myWorld);
}

void LightingTestContext::Init(CE_Engine& anEngine)
{
	myWorld = new CE_World();
	myEntityFactory = new EntityFactory(*myWorld);

	CE_Window& mainWindow = anEngine.GetMainWindow();
	CE_Camera* camera = mainWindow.GetCamera();
	camera->SetPosition(CE_Vector3f(5.f, 10.f, -5.f));

	RenderProcessor* renderProcessor = new RenderProcessor(*myWorld, anEngine.GetRendererProxy());
	myWorld->AddProcessor(renderProcessor);

	InitWorld();
}

void LightingTestContext::Update(float aDelta)
{
	myWorld->Update(aDelta);

	static float counter = 0.f;
	counter += aDelta;
	float x = sin(counter) * 10.f + 6.f;

	TranslationComponent& lightTranslate = myWorld->GetComponent<TranslationComponent>(myPointLight);
	lightTranslate.myOrientation.SetPos({ x, 5.f, 12.f });

	TranslationComponent& lightTranslate1 = myWorld->GetComponent<TranslationComponent>(myPointLight1);
	lightTranslate1.myOrientation.SetPos({ x, 11.f, 12.f });

	TranslationComponent& lightTranslate2 = myWorld->GetComponent<TranslationComponent>(myPointLight2);
	lightTranslate2.myOrientation.SetPos({ x, 17.f, 12.f });
}

void LightingTestContext::Render()
{
}

void LightingTestContext::InitWorld()
{
	float z = 15.f;
	for (int y = 0; y < 10; ++y)
	{
		float roughness = y / 10.f;
		for (int x = 0; x < 10; ++x)
		{
			float realX = -5.f;
			realX += static_cast<float>(x) + x;

			float realY = 2.f;
			realY += static_cast<float>(y) + y;

			CE_Vector3f pos(realX, realY, z);
			CE_Entity metalness = myEntityFactory->InstansiateEntity("sphere");
			TranslationComponent& translate = myWorld->GetComponent<TranslationComponent>(metalness);
			translate.myOrientation.SetPos(pos);
			translate.myScale = 2.f;

			RenderComponent& render = myWorld->GetComponent<RenderComponent>(metalness);
			render.myEntries[0].myMetalness = x / 10.f;
			render.myEntries[0].myRoughness = roughness;
		}
	}

	myPointLight = myEntityFactory->InstansiateEntity("point_light");
	myPointLight1 = myEntityFactory->InstansiateEntity("point_light");
	myPointLight2 = myEntityFactory->InstansiateEntity("point_light");
}