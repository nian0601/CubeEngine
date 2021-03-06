#include "stdafx.h"
#include "CE_ShaderManager.h"
#include "CE_PixelShader.h"
#include "CE_VertexShader.h"

CE_ShaderManager* CE_ShaderManager::ourInstance = nullptr;
void CE_ShaderManager::Create(CE_GPUContext& aGPUContext)
{
	CE_ASSERT(ourInstance == nullptr, "Tried to create ShaderManager twice!");
	ourInstance = new CE_ShaderManager(aGPUContext);
}

void CE_ShaderManager::Destroy()
{
	CE_SAFE_DELETE(ourInstance);
}

CE_ShaderManager::CE_ShaderManager(CE_GPUContext& aGPUContext)
	: myGPUContext(aGPUContext)
{
	CE_FileSystem::GetAllFilesFromDirectory("Data/Shaders", myFiles);
	
	for (const CE_FileSystem::FileInfo& fileInfo : myFiles)
	{
		CE_ASSERT(myFullFilePaths.KeyExists(fileInfo.myFileName) == false, "Find two files with the same name [%s]", fileInfo.myFileName.c_str());
		myFullFilePaths[fileInfo.myFileName] = fileInfo.myFilePath;
	}
}


CE_ShaderManager::~CE_ShaderManager()
{
	myShaders.DeleteAll();
}

CE_GenericShader* CE_ShaderManager::GetShader(const char* aName)
{
	if (myShaderMap.GetIfExists(aName) != nullptr)
		return myShaderMap[aName];

	CE_ASSERT(myFullFilePaths.KeyExists(aName) == true, "Unknown shadername [%s]", aName);
	CE_String fullPath = myFullFilePaths[aName];
	CE_String extension;
	CE_FileSystem::GetFileExtention(fullPath, extension);

	CE_GenericShader* shader = nullptr;
	if (extension == "px")
		shader = new CE_PixelShader(fullPath.c_str(), myGPUContext);
	else if (extension == "vx")
		shader = new CE_VertexShader(fullPath.c_str(), myGPUContext);

	CE_ASSERT(shader != nullptr, "Invalid shader extention [%s]", aName);
	myShaderMap[aName] = shader;
	myShaders.Add(shader);
	return shader;
}

void CE_ShaderManager::Update()
{
	if (CE_FileSystem::UpdateFileInfo(myFiles))
	{
		Sleep(200);
		for (CE_GenericShader* shader : myShaders)
			shader->Reload();
	}
}
