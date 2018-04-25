#include "stdafx.h"
#include "CE_GeneticAlgorithm.h"

CE_Genome::CE_Genome() 
	: myFitness(0.f)
{
}

CE_Genome::CE_Genome(float aFitness, const CE_GrowingArray<float>& someWeights)
	: myFitness(aFitness)
	, myWeights(someWeights)
{
}

//////////////////////////////////////////////////////////////////////////

CE_GeneticAlgorithm::CE_GeneticAlgorithm(int aNumberOfWeightsInNN)
	: myTotalFitness(0.f)
	, myChromoLenght(0)
	, myMutationRate(0.3f)
	, myCrossOverRate(0.7f)
	, myGenerationCounter(0)
	, myNumWeightsPerGenome(aNumberOfWeightsInNN)
{
}

void CE_GeneticAlgorithm::Epoch()
{
	++myGenerationCounter;

	myTotalFitness = 0.f;

	for (const CE_Genome& genome : myPopulation)
		myTotalFitness += genome.myFitness;

	CE_GrowingArray<CE_Genome> newPop;
	while (newPop.Size() < myChromoLenght)
	{
		CE_Genome mom = GetChromoRoulette(myPopulation);
		CE_Genome dad = GetChromoRoulette(myPopulation);

		CE_GrowingArray<float> child1;
		CE_GrowingArray<float> child2;

		Crossover(mom.myWeights, dad.myWeights, child1, child2);

		Mutate(child1);
		Mutate(child2);

		newPop.Add(CE_Genome(0.f, child1));
		newPop.Add(CE_Genome(0.f, child2));
	}

	myPopulation = newPop;
}

const CE_Genome& CE_GeneticAlgorithm::CreateNewGenome()
{
	CE_GrowingArray<float> weights;
	for (int i = 0; i < myNumWeightsPerGenome; ++i)
		weights.Add(CE_RandClamped());

	myPopulation.Add(CE_Genome(0.f, weights));
	++myChromoLenght;
	return myPopulation.GetLast();
}

void CE_GeneticAlgorithm::Crossover(const CE_GrowingArray<float>& aMom, const CE_GrowingArray<float>& aDad, CE_GrowingArray<float>& aChild1, CE_GrowingArray<float>& aChild2)
{
	CE_ASSERT(aMom.Size() == aDad.Size(), "Missmatching parent-lenghts, how??");
	bool needCrossover = CE_RandFloat() > myCrossOverRate;

	if (!needCrossover)
	{
		for (int i = 0; i < aMom.Size(); ++i)
		{
			if (aMom[i] != aDad[i])
			{
				needCrossover = true;
				break;
			}
		}
	}

	if (needCrossover)
	{
		aChild1 = aMom;
		aChild2 = aDad;
		return;
	}

	int crossPoint = CE_RandInt(0, myChromoLenght - 1);
	for (int i = 0; i < crossPoint; ++i)
	{
		aChild1.Add(aMom[i]);
		aChild2.Add(aDad[i]);
	}

	for (int i = crossPoint; i < aMom.Size(); ++i)
	{
		aChild1.Add(aDad[i]);
		aChild2.Add(aMom[i]);
	}
}

void CE_GeneticAlgorithm::Mutate(CE_GrowingArray<float>& aChromosome)
{
	for (float& gene : aChromosome)
	{
		if (CE_RandFloat() < myMutationRate)
			gene += CE_RandClamped() * 0.3f;
	}
}

CE_Genome CE_GeneticAlgorithm::GetChromoRoulette(const CE_GrowingArray<CE_Genome>& anPopulation)
{
	float slice = CE_RandFloat() * myTotalFitness;

	
	int bestIndex = -1;
	float accumFitness = 0.f;
	for (int i = 0; i < anPopulation.Size(); ++i)
	{
		accumFitness += anPopulation[i].myFitness;
		if (accumFitness >= slice)
		{
			bestIndex = i;
			break;
		}
	}

	CE_ASSERT(bestIndex != -1, "Failed to find a new Genome..?");
	return anPopulation[bestIndex];
}

