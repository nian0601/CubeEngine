#pragma once

#include <CE_BaseComponent.h>
#include <CE_NeuralNetwork.h>

struct NeuralNetworkComponent : public CE_BaseComponent
{
	NeuralNetworkComponent()
		: myFitness(0.f)
	{}

	CE_NeuralNetwork myNetwork;
	float myFitness;
};