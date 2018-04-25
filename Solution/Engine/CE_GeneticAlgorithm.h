#pragma once

struct CE_Genome
{
	CE_Genome();
	CE_Genome(float aFitness, const CE_GrowingArray<float>& someWeights);

	float myFitness;
	CE_GrowingArray<float> myWeights;
};

//////////////////////////////////////////////////////////////////////////

class CE_GeneticAlgorithm
{
public:
	CE_GeneticAlgorithm(int aNumberOfWeightsInNN);

	CE_GrowingArray<CE_Genome>& GetPopulation() { return myPopulation; }
	void Epoch();

	const CE_Genome& CreateNewGenome();
	int GetGnomeCount() const { return myPopulation.Size(); }

private:
	void Crossover(const CE_GrowingArray<float>& aMom, const CE_GrowingArray<float>& aDad,
		CE_GrowingArray<float>& aChild1, CE_GrowingArray<float>& aChild2);

	void Mutate(CE_GrowingArray<float>& aChromosome);
	CE_Genome GetChromoRoulette(const CE_GrowingArray<CE_Genome>& anPopulation);

	CE_GrowingArray<CE_Genome> myPopulation;
	int myChromoLenght;
	int myNumWeightsPerGenome;

	float myTotalFitness;

	//Around 0.05 - 0.3
	float myMutationRate;

	//Around 0.7
	float myCrossOverRate;

	int myGenerationCounter;
};

