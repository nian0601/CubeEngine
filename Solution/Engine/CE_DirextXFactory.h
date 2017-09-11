#pragma once

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Texture2D;
struct ID3D11DepthStencilView;

struct ID3D11RasterizerState;
struct ID3D11DepthStencilState;
struct ID3D11BlendState;

class CE_DirextXFactory
{
public:
	static CE_DirextXFactory* GetInstance();
	static void Create(ID3D11Device* aDevice, ID3D11DeviceContext* aContext);
	static void Destroy();

	void CreateTexture2D(const CE_Vector2i& aSize, unsigned int aFormat, unsigned int someBindFlags, unsigned int aUsage, ID3D11Texture2D*& outTexture);
	void CreateDepthStencilView(unsigned int aFormat, unsigned int aViewDimension, ID3D11Texture2D* aTexture, ID3D11DepthStencilView*& outView);

	void SetRasterizerState(CE_RasterizerState anState);
	void SetDepthStencilState(CE_DepthState anState);
	void SetBlendState(CE_BlendState anState);

private:
	CE_DirextXFactory(ID3D11Device* aDevice, ID3D11DeviceContext* aContext);
	~CE_DirextXFactory();

	void SetupRasterizerStates();
	void SetupDepthStencilStates();
	void SetupBlendStates();

	ID3D11Device* myDevice;
	ID3D11DeviceContext* myContext;

	ID3D11RasterizerState* myRasterizerStates[static_cast<int>(CE_RasterizerState::_RAZTER_COUNT)];
	ID3D11DepthStencilState* myDepthStencilStates[static_cast<int>(CE_DepthState::_DEPTH_COUNT)];
	ID3D11BlendState* myBlendStates[static_cast<int>(CE_BlendState::_BLEND_COUNT)];

	static CE_DirextXFactory* ourInstance;
};

