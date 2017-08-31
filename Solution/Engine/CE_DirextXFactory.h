#pragma once

struct ID3D11Device;
struct ID3D11Texture2D;
struct ID3D11DepthStencilView;

class CE_DirextXFactory
{
public:
	static CE_DirextXFactory* GetInstance();
	static void Create(ID3D11Device* aDevice);
	static void Destroy();

	void CreateTexture2D(const CE_Vector2i& aSize, unsigned int aFormat, unsigned int someBindFlags, unsigned int aUsage, ID3D11Texture2D*& outTexture);
	void CreateDepthStencilView(unsigned int aFormat, unsigned int aViewDimension, ID3D11Texture2D* aTexture, ID3D11DepthStencilView*& outView);

private:
	CE_DirextXFactory(ID3D11Device* aDevice);
	~CE_DirextXFactory();

	ID3D11Device* myDevice;

	static CE_DirextXFactory* ourInstance;
};

