#pragma once

struct CE_Neuron
{
	CE_Neuron(){}
	CE_Neuron(int aNumberOfInputs);

	CE_GrowingArray<float> myWeights;
};

//////////////////////////////////////////////////////////////////////////

struct CE_NeuronLayer
{
	CE_NeuronLayer(){}
	CE_NeuronLayer(int aNumberOfNeurons, int aNumberOfInputsPerNeuron);

	CE_GrowingArray<CE_Neuron> myNeurons;
};

//////////////////////////////////////////////////////////////////////////

class CE_NeuralNetwork
{
public:
	void CreateNet(int aNumInputs, int aNumOutputs, int aNumHiddenLayers, int aNumNeuronsPerLayer);

	void GetWeights(CE_GrowingArray<float>& someWeightsOut) const;
	void PutWeights(const CE_GrowingArray<float>& someWeights);
	int GetNumWeights() const;

	void Update(const CE_GrowingArray<float>& someInputs, CE_GrowingArray<float>& someOutputs);

private:
	float Sigmoid(float aActivation);

	int myNumberOfInputs;
	int myNumberOfOutputs;
	int myNumberOfHiddenLayers;
	int myNumberOfNeuronsPerLayer;
	CE_GrowingArray<CE_NeuronLayer> myLayers;
};

