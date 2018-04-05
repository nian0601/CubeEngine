#pragma once

// All credit goes to Andreas Kahler
// http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
//

#include "CE_ShaderStructs.h"
#include "CE_Map.h"

class CE_IcoSphereCreator
{
public:

	void Create(int aRecursionLevel);

	CE_GrowingArray<CE_PosNormColor_Vert> myVertices;
	CE_GrowingArray<unsigned int> myIndices;

private:
	void CreateInitialSphere();
	int AddVertex(const CE_Vector3f& aPosition);
	void AddFace(int aIndex1, int aIndex2, int aIndex3, CE_GrowingArray<unsigned int>& someIndices);
	int GetMiddlePoint(int aPoint1, int aPoint2);

	CE_Map<long long, int> myMiddlePointIndexCache;
};