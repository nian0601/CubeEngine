#include "stdafx.h"

#include "CE_ObjLoader.h"
#include "CE_FileParser.h"

void CE_ObjLoader::Load(const char* aFilePath)
{
	myModels.RemoveAll();

	CE_FileParser file(aFilePath);


	CE_GrowingArray<CE_Vector4f> positions;
	CE_GrowingArray<CE_Vector4f> normals;

	// Do we really care? None of the Kenney-models are textured?
	// Lets load them for now anyway, even if they are not used..
	CE_GrowingArray<CE_Vector2f> uvs;

	CE_String line;
	CE_GrowingArray<CE_String> words;
	CE_GrowingArray<CE_String> vertexWords;
	Model* currentModel = nullptr;
	while (file.ReadLine(line))
	{
		file.TrimBeginAndEnd(line);
		file.SplitLine(line, words);

		if (words[0].Empty() || words[0] == "#")
			continue;

		if (words[0] == "v")
		{
			CE_Vector4f& position = positions.Add();
			position.x = file.GetFloat(words[1]);
			position.y = file.GetFloat(words[2]);
			position.z = file.GetFloat(words[3]);
			position.w = 1.f;
		}
		else if (words[0] == "vn")
		{
			CE_Vector4f& normal = normals.Add();
			normal.x = file.GetFloat(words[1]);
			normal.y = file.GetFloat(words[2]);
			normal.z = file.GetFloat(words[3]);
			normal.w = 1.f;
		}
		else if (words[0] == "vt")
		{
			CE_Vector2f& uv = uvs.Add();
			uv.x = file.GetFloat(words[1]);
			uv.y = file.GetFloat(words[2]);
		}
		else if (words[0] == "usemtl")
		{
			currentModel = &myModels.Add();
			currentModel->myMaterial = words[1];
		}
		else if (words[0] == "f")
		{
			CE_ASSERT(currentModel != nullptr, "We dont have an active model when reading indicies???");

			file.SplitWord(words[1], "/", vertexWords);
			AddVertex(*currentModel, file, vertexWords, positions, normals);

			file.SplitWord(words[2], "/", vertexWords);
			AddVertex(*currentModel, file, vertexWords, positions, normals);

			file.SplitWord(words[3], "/", vertexWords);
			AddVertex(*currentModel, file, vertexWords, positions, normals);
		}
	}
}

void CE_ObjLoader::AddVertex(Model& aModel, const CE_FileParser& aFileParser, const CE_GrowingArray<CE_String>& someVertexStrings, const CE_GrowingArray<CE_Vector4f>& somePositions, const CE_GrowingArray<CE_Vector4f>& someNormals)
{
	// Why is the indices in .OBJ starting at 1????
	int positionIndex = aFileParser.GetInt(someVertexStrings[0]) - 1;
	//int uvIndex = aFileParser.GetInt(someVertexStrings[1]) - 1;
	int normalIndex = aFileParser.GetInt(someVertexStrings[2]) - 1;

	CE_PosNormColor_Vert& vertex = aModel.myVertices.Add();
	vertex.myPosition = somePositions[positionIndex];
	vertex.myNormal = someNormals[normalIndex];
	//vertex.myTexCoord = uvs[uvIndex];
	vertex.myColor = { 1.f, 1.f, 1.f, 1.f };

	aModel.myIndices.Add(aModel.myVertices.Size() - 1);
}
