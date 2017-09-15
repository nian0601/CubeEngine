#pragma once

class CE_Camera;
class CE_GPUContext;
class CE_Model;
class CE_Shader;

class CE_Renderer
{
public:
	CE_Renderer(CE_GPUContext& anGPUContext);
	~CE_Renderer();

	void Render(CE_Camera& aCamera);

	void AddCubeData(const CE_Matrix44f& anOrientation, const CE_Vector4f& aColor);

private:
	struct CubeData
	{
		CE_Matrix44f myOrientation;
		CE_Vector4f myColor;
	};
	CE_GrowingArray<CubeData> myCubeData;

	CE_Shader* myCubeShader;
	CE_Model* myCubeModel;
	CE_GPUContext& myGPUContext;
};

