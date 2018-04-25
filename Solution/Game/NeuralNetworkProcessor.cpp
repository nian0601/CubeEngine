#include "stdafx.h"
#include "NeuralNetworkProcessor.h"

#include <CE_GeneticAlgorithm.h>
#include <CE_NeuralNetwork.h>

#include "TranslationComponent.h"
#include "NeuralNetworkComponent.h"
#include <CE_DebugDraw.h>
#include "InputSingletonComponent.h"

NeuralNetworkProcessor::NeuralNetworkProcessor(CE_World& aWorld)
	: CE_BaseProcessor(aWorld, CE_CreateFilter<CE_Requires<NeuralNetworkComponent, TranslationComponent>>())
	, myGeneticAlgorithm(nullptr)
	, myTargetPosition(5.f, 0.f, 5.f)
	, myCurrentGenerationTime(0.f)
	, myTimePerGeneration(20.f)
{
}

NeuralNetworkProcessor::~NeuralNetworkProcessor()
{
	CE_SAFE_DELETE(myGeneticAlgorithm);
}

void NeuralNetworkProcessor::Update(float aDelta)
{
	const CE_GrowingArray<CE_Entity>& entities = GetEntities();

	for (const CE_Entity& entity : entities)
	{
		TranslationComponent& translation = GetComponent<TranslationComponent>(entity);
		NeuralNetworkComponent& nnComponent = GetComponent<NeuralNetworkComponent>(entity);

		CE_GrowingArray<float> inputs;
		PrepareNetwork(translation, inputs);

		CE_GrowingArray<float> outputs;
		RunNetwork(nnComponent, inputs, outputs);

		ApplyOutputs(translation, outputs, aDelta);
		CalculateFitness(translation, nnComponent, aDelta);
	}

	myCurrentGenerationTime += aDelta;
	if(myCurrentGenerationTime >= myTimePerGeneration)
	{
		myCurrentGenerationTime = 0.f;
		FinishGeneration(entities);
	}
}

void NeuralNetworkProcessor::EntityAdded(CE_Entity anEntity)
{
	NeuralNetworkComponent& nnComponent = GetComponent<NeuralNetworkComponent>(anEntity);
	nnComponent.myNetwork.CreateNet(4, 3, 1, 6);

	int weightCount = nnComponent.myNetwork.GetNumWeights();

	if (myGeneticAlgorithm == nullptr)
		myGeneticAlgorithm = new CE_GeneticAlgorithm(weightCount);

	const CE_Genome& genome = myGeneticAlgorithm->CreateNewGenome();
	nnComponent.myNetwork.PutWeights(genome.myWeights);
	myGenomeIndicies[anEntity] = myGeneticAlgorithm->GetGnomeCount() - 1;

	TranslationComponent& translation = GetComponent<TranslationComponent>(anEntity);
	myStartPositions[anEntity] = translation.myOrientation.GetPos();
}

void NeuralNetworkProcessor::PrepareNetwork(TranslationComponent& aTranslationComponent, CE_GrowingArray<float>& someInputsOut)
{
	CE_Vector3f oldPos = aTranslationComponent.myOrientation.GetPos();
	CE_Vector3f toTarget = myTargetPosition - oldPos;
	CE_Normalize(toTarget);

	someInputsOut.Add(toTarget.x);
	someInputsOut.Add(toTarget.z);
	someInputsOut.Add(oldPos.x);
	someInputsOut.Add(oldPos.z);
}

void NeuralNetworkProcessor::RunNetwork(NeuralNetworkComponent& aNeuralNetworkComponent, const CE_GrowingArray<float>& someInputs, CE_GrowingArray<float>& someOutputsOut)
{
	aNeuralNetworkComponent.myNetwork.Update(someInputs, someOutputsOut);

	for (float& output : someOutputsOut)
		output = CE_Lerp(-1.f, 1.f, output);
}

void NeuralNetworkProcessor::ApplyOutputs(TranslationComponent& aTranslationComponent, const CE_GrowingArray<float>& someOutputs, float aDelta)
{
	CE_Vector3f moveDir(someOutputs[0], 0.f, someOutputs[1]);
	CE_Vector3f newPos = aTranslationComponent.myOrientation.GetPos() + moveDir * someOutputs[2] * aDelta;
	aTranslationComponent.myOrientation.SetPos(newPos);
}

void NeuralNetworkProcessor::CalculateFitness(TranslationComponent& aTranslationComponent, NeuralNetworkComponent& aNeuralNetworkComponent, float aDelta)
{
	const float maxDist = 25.f * 25.f;
	const float maxScore = 25.f;

	float distanceToTargetPosition = CE_Length2(aTranslationComponent.myOrientation.GetPos() - myTargetPosition);
	float ratio = 1.f - ((distanceToTargetPosition * distanceToTargetPosition) / maxDist);

	aNeuralNetworkComponent.myFitness += ratio * maxScore * aDelta;
	aNeuralNetworkComponent.myFitness = CE_Max(0.f, aNeuralNetworkComponent.myFitness);
}

void NeuralNetworkProcessor::FinishGeneration(const CE_GrowingArray<CE_Entity>& someEntities)
{
	CE_GrowingArray<CE_Genome>& population = myGeneticAlgorithm->GetPopulation();
	for (const CE_Entity& entity : someEntities)
	{
		NeuralNetworkComponent& nnComponent = GetComponent<NeuralNetworkComponent>(entity);

		int genomeIndex = myGenomeIndicies[entity];
		population[genomeIndex].myFitness = nnComponent.myFitness;
		nnComponent.myFitness = 0.f;
	}

	myGeneticAlgorithm->Epoch();

	for (const CE_Entity& entity : someEntities)
	{
		NeuralNetworkComponent& nnComponent = GetComponent<NeuralNetworkComponent>(entity);

		int genomeIndex = myGenomeIndicies[entity];

		CE_GrowingArray<float> oldWeights;
		nnComponent.myNetwork.GetWeights(oldWeights);
		nnComponent.myNetwork.PutWeights(population[genomeIndex].myWeights);

		TranslationComponent& translation = GetComponent<TranslationComponent>(entity);
		translation.myOrientation.SetPos(myStartPositions[entity]);
	}
}
