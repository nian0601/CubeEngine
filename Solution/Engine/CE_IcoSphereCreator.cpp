#include "stdafx.h"
#include "CE_IcoSphereCreator.h"


void CE_IcoSphereCreator::Create(int aRecursionLevel)
{
	CreateInitialSphere();

	// refine triangles
	for (int r = 0; r < aRecursionLevel; r++)
	{
		CE_GrowingArray<unsigned int> indices2;
		for (int i = 0; i < myIndices.Size(); i += 3)
		{
			unsigned int v1 = myIndices[i];
			unsigned int v2 = myIndices[i+1];
			unsigned int v3 = myIndices[i+2];

			// replace triangle by 4 triangles
			int a = GetMiddlePoint(v1, v2);
			int b = GetMiddlePoint(v2, v3);
			int c = GetMiddlePoint(v3, v1);


			AddFace(v1, a, c, indices2);
			AddFace(v2, b, a, indices2);
			AddFace(v3, c, b, indices2);
			AddFace(a, b, c, indices2);
		}
		
		myIndices = indices2;
	}
}

void CE_IcoSphereCreator::CreateInitialSphere()
{
	float t = (1.f + sqrt(5.f)) / 2.f;

	float w = 0.f;
	AddVertex(CE_Vector4f(-1.f, t, 0.f, w));
	AddVertex(CE_Vector4f(1.f, t, 0.f, w));
	AddVertex(CE_Vector4f(-1.f, -t, 0.f, w));
	AddVertex(CE_Vector4f(1.f, -t, 0.f, w));

	AddVertex(CE_Vector4f(0.f, -1.f, t, w));
	AddVertex(CE_Vector4f(0.f, 1.f, t, w));
	AddVertex(CE_Vector4f(0.f, -1.f, -t, w));
	AddVertex(CE_Vector4f(0.f, 1.f, -t, w));
					   
	AddVertex(CE_Vector4f(t, 0.f, -1.f, w));
	AddVertex(CE_Vector4f(t, 0.f, 1.f, w));
	AddVertex(CE_Vector4f(-t, 0.f, -1.f, w));
	AddVertex(CE_Vector4f(-t, 0.f, 1.f, w));


	// 5 faces around point 0
	AddFace(0, 11, 5, myIndices);
	AddFace(0, 5, 1, myIndices);
	AddFace(0, 1, 7, myIndices);
	AddFace(0, 7, 10, myIndices);
	AddFace(0, 10, 11, myIndices);

	// 5 adjacent faces 
	AddFace(1, 5, 9, myIndices);
	AddFace(5, 11, 4, myIndices);
	AddFace(11, 10, 2, myIndices);
	AddFace(10, 7, 6, myIndices);
	AddFace(7, 1, 8, myIndices);

	// 5 faces around point 3
	AddFace(3, 9, 4, myIndices);
	AddFace(3, 4, 2, myIndices);
	AddFace(3, 2, 6, myIndices);
	AddFace(3, 6, 8, myIndices);
	AddFace(3, 8, 9, myIndices);

	// 5 adjacent faces 
	AddFace(4, 9, 5, myIndices);
	AddFace(2, 4, 11, myIndices);
	AddFace(6, 2, 10, myIndices);
	AddFace(8, 6, 7, myIndices);
	AddFace(9, 8, 1, myIndices);
}

void CE_IcoSphereCreator::AddFace(int aIndex1, int aIndex2, int aIndex3, CE_GrowingArray<unsigned int>& someIndices)
{
	someIndices.Add(aIndex1);
	someIndices.Add(aIndex2);
	someIndices.Add(aIndex3);
}

int CE_IcoSphereCreator::GetMiddlePoint(int aPoint1, int aPoint2)
{
	bool firstIsSmaller = aPoint1 < aPoint2;
	long long smallerIndex = firstIsSmaller ? aPoint1 : aPoint2;
	long long greaterIndex = firstIsSmaller ? aPoint2 : aPoint1;
	long long key = (smallerIndex << 32) + greaterIndex;

	if (const int* index = myMiddlePointIndexCache.GetIfExists(key))
		return *index;

	const CE_Vector4f& vert1 = GetPosition(aPoint1);
	const CE_Vector4f& vert2 = GetPosition(aPoint2);

	CE_Vector4f middle = (vert1 + vert2) * 0.5f;
	int index = AddVertex(middle);

	myMiddlePointIndexCache[key] = index;
	return index;
}