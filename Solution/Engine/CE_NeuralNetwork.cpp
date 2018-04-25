#include "stdafx.h"
#include "CE_NeuralNetwork.h"


CE_Neuron::CE_Neuron(int aNumberOfInputs)
{
	for (int i = 0; i < aNumberOfInputs+1; ++i)
		myWeights.Add(CE_RandClamped());
}

//////////////////////////////////////////////////////////////////////////

CE_NeuronLayer::CE_NeuronLayer(int aNumberOfNeurons, int aNumberOfInputsPerNeuron)
{
	for (int i = 0; i < aNumberOfNeurons; ++i)
		myNeurons.Add(CE_Neuron(aNumberOfInputsPerNeuron));
}

//////////////////////////////////////////////////////////////////////////


void CE_NeuralNetwork::CreateNet(int aNumInputs, int aNumOutputs, int aNumHiddenLayers, int aNumNeuronsPerLayer)
{
	myNumberOfInputs = aNumInputs;
	myNumberOfOutputs = aNumOutputs;
	myNumberOfHiddenLayers = aNumHiddenLayers;
	myNumberOfNeuronsPerLayer = aNumNeuronsPerLayer;

	myLayers.Add(CE_NeuronLayer(myNumberOfNeuronsPerLayer, myNumberOfInputs));

	for (int i = 0; i < myNumberOfHiddenLayers-1; ++i)
		myLayers.Add(CE_NeuronLayer(myNumberOfNeuronsPerLayer, myNumberOfNeuronsPerLayer));

	myLayers.Add(CE_NeuronLayer(myNumberOfOutputs, myNumberOfNeuronsPerLayer));
}

void CE_NeuralNetwork::GetWeights(CE_GrowingArray<float>& someWeightsOut) const
{
	for (const CE_NeuronLayer& layer : myLayers)
	{
		for (const CE_Neuron& neuron : layer.myNeurons)
		{
			for (int i = 0; i < neuron.myWeights.Size(); ++i)
				someWeightsOut.Add(neuron.myWeights[i]);
		}
	}
}

void CE_NeuralNetwork::PutWeights(const CE_GrowingArray<float>& someWeights)
{
	int count = 0;
	for (CE_NeuronLayer& layer : myLayers)
	{
		for (CE_Neuron& neuron : layer.myNeurons)
		{
			for (int i = 0; i < neuron.myWeights.Size(); ++i)
			{
				neuron.myWeights[i] = someWeights[count++];
			}
		}
	}
}

int CE_NeuralNetwork::GetNumWeights() const
{
	int count = 0;
	for (const CE_NeuronLayer& layer : myLayers)
	{
		for (const CE_Neuron& neuron : layer.myNeurons)
			count += neuron.myWeights.Size();
	}

	return count;
}

void CE_NeuralNetwork::Update(const CE_GrowingArray<float>& someInputs, CE_GrowingArray<float>& someOutputs)
{
	if (someInputs.Size() != myNumberOfInputs)
	{
		CE_ASSERT_ALWAYS("Invalid number of inputs!");
		return;
	}

	int weigth = 0;
	CE_GrowingArray<float> inputs = someInputs;

	//For each Layer...
	for (int i = 0; i < myLayers.Size(); ++i)
	{
		if (i > 0)
			inputs = someOutputs;

		someOutputs.RemoveAll();

		weigth = 0;

		const CE_NeuronLayer& layer = myLayers[i];
		const CE_GrowingArray<CE_Neuron>& neurons = layer.myNeurons;

		//For each Neuron...
		for (const CE_Neuron& neuron : neurons)
		{
			float totalInput = 0.f;

			//For each weight...
			for (int k = 0; k < neuron.myWeights.Size() - 1; ++k)
				totalInput += neuron.myWeights[k] * inputs[k];

			totalInput += neuron.myWeights.GetLast() * -1.f;

			someOutputs.Add(Sigmoid(totalInput));
		}
	}

	CE_ASSERT(someOutputs.Size() == myNumberOfOutputs, "Invalid number of outputs");
}

float CE_NeuralNetwork::Sigmoid(float aActivation)
{
	return (1 / (1 + exp(-aActivation)));
}
