#pragma once
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;

class CE_GPUContext;

class CE_Texture
{
public:
	CE_Texture();
	~CE_Texture();

	void InitForShader(const CE_Vector2i& aSize, unsigned int aBindFlag, unsigned int aFormat, CE_GPUContext& aGPUContext);
	void InitAsBackBuffer(const CE_Vector2i& aSize, ID3D11Texture2D* aTexture, CE_GPUContext& aGPUContext);

	void Load(const CE_String& aFilePath, CE_GPUContext& aGPUContext);
	void LoadDDS(const CE_String& aFilePath, CE_GPUContext& aGPUContext);

	const CE_Vector2i& GetSize() const { return mySize; }

	ID3D11ShaderResourceView* GetShaderView() const { return myShaderView; }
	ID3D11RenderTargetView* GetRenderTarget() const { return myRenderTarget; }

	ID3D11ShaderResourceView* GetDepthView() const { return myDepthShaderView; }
	ID3D11DepthStencilView* GetDepthStencil() const { return myDepthStencil; }

private:
	void CreateRenderTarget(const CE_Vector2i& aSize, unsigned int aBindFlag, unsigned int aFormat, CE_GPUContext& aGPUContext);
	void CreateDepthStencil(const CE_Vector2i& aSize, CE_GPUContext& aGPUContext);

	CE_Vector2i mySize;
	CE_String myFilePath;
	unsigned int myFormat;

	ID3D11Texture2D* myTexture;
	ID3D11ShaderResourceView* myShaderView;
	ID3D11RenderTargetView* myRenderTarget;

	ID3D11Texture2D* myDepthTexture;
	ID3D11ShaderResourceView* myDepthShaderView;
	ID3D11DepthStencilView* myDepthStencil;
};

