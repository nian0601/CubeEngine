#pragma once

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Texture2D;
struct ID3D11DepthStencilView;

struct ID3D11BlendState;
struct ID3D11DepthStencilState;
struct ID3D11RasterizerState;
struct ID3D11SamplerState;

class CE_DirextXFactory
{
public:
	static CE_DirextXFactory* GetInstance();
	static void Create(ID3D11Device* aDevice, ID3D11DeviceContext* aContext);
	static void Destroy();

	void CreateTexture2D(const CE_Vector2i& aSize, unsigned int aFormat, unsigned int someBindFlags, unsigned int aUsage, ID3D11Texture2D*& outTexture);
	void CreateDepthStencilView(unsigned int aFormat, unsigned int aViewDimension, ID3D11Texture2D* aTexture, ID3D11DepthStencilView*& outView);

	CE_RasterizerState GetRasterizeState() const { return myCurrentRasterizeState; }
	void SetRasterizerState(CE_RasterizerState anState);

	CE_DepthState GetDepthState() const { return myCurrentDepthState; }
	void SetDepthStencilState(CE_DepthState anState);

	CE_BlendState GetBlendState() const { return myCurrentBlendState; }
	void SetBlendState(CE_BlendState anState);

private:
	CE_DirextXFactory(ID3D11Device* aDevice, ID3D11DeviceContext* aContext);
	~CE_DirextXFactory();

	void SetupRasterizerStates();
	void SetupDepthStencilStates();
	void SetupBlendStates();
	void SetupSamplerStates();

	ID3D11Device* myDevice;
	ID3D11DeviceContext* myContext;

	CE_RasterizerState myCurrentRasterizeState;
	ID3D11RasterizerState* myRasterizerStates[static_cast<int>(CE_RasterizerState::_RAZTER_COUNT)];

	CE_DepthState myCurrentDepthState;
	ID3D11DepthStencilState* myDepthStencilStates[static_cast<int>(CE_DepthState::_DEPTH_COUNT)];

	CE_BlendState myCurrentBlendState;
	ID3D11BlendState* myBlendStates[static_cast<int>(CE_BlendState::_BLEND_COUNT)];

	ID3D11SamplerState* mySamplerStates[static_cast<int>(CE_SamplerState::_SAMPLER_COUNT)];

	static CE_DirextXFactory* ourInstance;
};

class CE_SetResetRasterizer
{
public:
	CE_SetResetRasterizer(CE_RasterizerState aState)
	{
		myOldState = CE_DirextXFactory::GetInstance()->GetRasterizeState();
		CE_DirextXFactory::GetInstance()->SetRasterizerState(aState);
	}

	~CE_SetResetRasterizer()
	{
		CE_DirextXFactory::GetInstance()->SetRasterizerState(myOldState);
	}

private:
	CE_RasterizerState myOldState;
};

class CE_SetResetDepth
{
public:
	CE_SetResetDepth(CE_DepthState aState)
	{
		myOldState = CE_DirextXFactory::GetInstance()->GetDepthState();
		CE_DirextXFactory::GetInstance()->SetDepthStencilState(aState);
	}

	~CE_SetResetDepth()
	{
		CE_DirextXFactory::GetInstance()->SetDepthStencilState(myOldState);
	}

private:
	CE_DepthState myOldState;
};

class CE_SetResetBlend
{
public:
	CE_SetResetBlend(CE_BlendState aState)
	{
		myOldState = CE_DirextXFactory::GetInstance()->GetBlendState();
		CE_DirextXFactory::GetInstance()->SetBlendState(aState);
	}

	~CE_SetResetBlend()
	{
		CE_DirextXFactory::GetInstance()->SetBlendState(myOldState);
	}

private:
	CE_BlendState myOldState;
};