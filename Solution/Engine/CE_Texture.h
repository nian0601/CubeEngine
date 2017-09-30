#pragma once
struct ID3D11ShaderResourceView;

class CE_GPUContext;

class CE_Texture
{
public:
	CE_Texture();
	~CE_Texture();

	void Load(const CE_String& aFilePath, CE_GPUContext& aGPUContext);

	const CE_Vector2f& GetSize() const { return mySize; }
	ID3D11ShaderResourceView* GetShaderView() { return myShaderView; }

private:
	CE_Vector2f mySize;
	CE_String myFilePath;

	ID3D11ShaderResourceView* myShaderView;
};

