#pragma once

#include "CE_Map.h"
#include "CE_FileSystem.h"

class CE_GenericShader;
class CE_GPUContext;

class CE_ShaderManager
{
public:
	static void Create(CE_GPUContext& aGPUContext);
	static void Destroy();
	static CE_ShaderManager* GetInstance() { return ourInstance; }

	CE_GenericShader* GetShader(const char* aName);

	void Update();

private:
	CE_ShaderManager(CE_GPUContext& aGPUContext);
	~CE_ShaderManager();

	CE_Map<CE_String, CE_GenericShader*> myShaderMap;
	CE_Map<CE_String, CE_String> myFullFilePaths;
	CE_GrowingArray<CE_GenericShader*> myShaders;
	CE_GrowingArray<CE_FileSystem::FileInfo> myFiles;
	CE_GPUContext& myGPUContext;

	static CE_ShaderManager* ourInstance;
};

