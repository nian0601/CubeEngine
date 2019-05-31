#pragma once
#include "CE_ShaderStructs.h"

class CE_FileParser;
class CE_ObjLoader
{
public:
	struct Model
	{
		CE_GrowingArray<CE_PosNormColor_Vert> myVertices;
		CE_GrowingArray<unsigned int> myIndices;
		CE_String myMaterial;
		CE_Vector3f myMin;
		CE_Vector3f myMax;
	};

	void Load(const char* aFilePath);

	CE_GrowingArray<CE_PosNormColor_Vert> myVertices;
	CE_GrowingArray<unsigned int> myIndices;

	CE_GrowingArray<Model> myModels;

private:
	void AddVertex(Model& aModel, const CE_FileParser& aFileParser, const CE_GrowingArray<CE_String>& someVertexStrings, const CE_GrowingArray<CE_Vector4f>& somePositions, const CE_GrowingArray<CE_Vector4f>& someNormals);
};