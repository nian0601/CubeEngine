#include "stdafx.h"

#include "EntityFactory.h"
#include "NeuralNetworkComponent.h"
#include "NeuralNetworkContext.h"
#include "NeuralNetworkProcessor.h"
#include "RenderComponent.h"
#include "RenderProcessor.h"
#include "TranslationComponent.h"

#include <CE_Camera.h>
#include <CE_Engine.h>
#include <CE_Window.h>
#include <CE_World.h>


NeuralNetworkContext::NeuralNetworkContext()
{
}


NeuralNetworkContext::~NeuralNetworkContext()
{
	CE_SAFE_DELETE(myEntityFactory);
	CE_SAFE_DELETE(myWorld);
}

void NeuralNetworkContext::Init(CE_Engine& anEngine)
{

	myWorld = new CE_World();
	myEntityFactory = new EntityFactory(*myWorld);

	CE_Window& mainWindow = anEngine.GetMainWindow();
	CE_Camera* camera = mainWindow.GetCamera();
	camera->SetPosition(CE_Vector3f(5.f, 10.f, -5.f));
	camera->Rotate(CE_Matrix44f::CreateRotateAroundX(3.14f * 0.25));

	RenderProcessor* renderProcessor = new RenderProcessor(*myWorld, anEngine.GetRendererProxy());
	myWorld->AddProcessor(renderProcessor);

	myWorld->AddProcessor<NeuralNetworkProcessor>();

	for (int i = 0; i < 25; ++i)
	{
		CE_Entity nn_entity = myEntityFactory->InstansiateEntity("nn_entity");
		TranslationComponent& treeTranslate = myWorld->GetComponent<TranslationComponent>(nn_entity);
		treeTranslate.myOrientation.SetPos(CE_Vector3f(CE_RandFloat() * 10.f, 0.f, CE_RandFloat() * 10.f));

		myWorld->AddComponent<NeuralNetworkComponent>(nn_entity);
	}

	CE_Entity nn_target = myEntityFactory->InstansiateEntity("nn_target");
	TranslationComponent& nn_target_translate = myWorld->GetComponent<TranslationComponent>(nn_target);
	nn_target_translate.myOrientation.SetPos(CE_Vector3f(5.f, 0.f, 5.f));
}

void NeuralNetworkContext::Update(float aDelta)
{
	myWorld->Update(aDelta);
}

void NeuralNetworkContext::Render()
{
}