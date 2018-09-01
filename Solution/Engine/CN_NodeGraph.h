#pragma once

class CN_Node;
class CN_NodeGraph
{
	friend class CUI_NodeEditor;

public:
	CN_NodeGraph();
	~CN_NodeGraph();

	void Execute();

	void Load(const char* aFilePath);
	void Save(const char* aFilePath);

	void AddNode(CN_Node* aNode);
	void DeleteNode(CN_Node* aNode);

private:
	void ConnectPins(u32 aOutputNode, u32 aOutputPin, u32 aInputNode, u32 aInputPin);

	CE_String myFilePath;
	CN_Node* myInitNode;
	CE_GrowingArray<CN_Node*> myNodes;
	u32 myMaxNodeID;
};