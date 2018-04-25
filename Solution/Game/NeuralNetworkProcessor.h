#pragma once
#include <CE_BaseProcessor.h>
#include <CE_Map.h>


class CE_GeneticAlgorithm;

struct TranslationComponent;
struct NeuralNetworkComponent;

class NeuralNetworkProcessor : public CE_BaseProcessor
{
public:
	NeuralNetworkProcessor(CE_World& aWorld);
	~NeuralNetworkProcessor();

	void Update(float aDelta) override;
	void EntityAdded(CE_Entity anEntity) override;

private:
	void PrepareNetwork(TranslationComponent& aTranslationComponent, CE_GrowingArray<float>& someInputsOut);
	void RunNetwork(NeuralNetworkComponent& aNeuralNetworkComponent, const CE_GrowingArray<float>& someInputs, CE_GrowingArray<float>& someOutputsOut);
	void ApplyOutputs(TranslationComponent& aTranslationComponent, const CE_GrowingArray<float>& someOutputs, float aDelta);
	void CalculateFitness(TranslationComponent& aTranslationComponent, NeuralNetworkComponent& aNeuralNetworkComponent, float aDelta);
	void FinishGeneration(const CE_GrowingArray<CE_Entity>& someEntities);


	CE_GeneticAlgorithm* myGeneticAlgorithm;
	CE_Map<CE_Entity, CE_Vector3f> myStartPositions;
	CE_Map<CE_Entity, int> myGenomeIndicies;
	CE_Vector3f myTargetPosition;
	float myCurrentGenerationTime;
	float myTimePerGeneration;
};

