#pragma once

struct ID3D11Debug;
struct ID3D11DepthStencilView;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct ID3D11Texture2D;

struct IDXGISwapChain;

struct D3D11_VIEWPORT;

class CE_WindowHandler;
class CE_DirectX
{
public:
	CE_DirectX(const CE_WindowHandler* aWindowHandler);
	~CE_DirectX();

	void EndFrame();

	ID3D11Device* GetDevice() const { return myDevice; }
	ID3D11DeviceContext* GetContext() const { return myDeviceContext; }

	const CE_Vector2i& GetWindowSize() const;

private:
	void SetupSwapChain();
	void SetupDebugInterface();
	void SetupBackbuffer();

	void GetRefreshRate(int& aNumeratorOut, int& aDenominatorOut) const;

	const CE_WindowHandler* myWindowHandler;

	ID3D11Debug* myDebugInterface;
	
	IDXGISwapChain* mySwapChain;
	ID3D11Device* myDevice;
	ID3D11DeviceContext* myDeviceContext;


	ID3D11Texture2D* myBackbuffer;
	ID3D11RenderTargetView* myBackbufferRenderTarget;
	ID3D11ShaderResourceView* myBackbufferShaderResourceView;

	ID3D11Texture2D* myDepthStencil;
	ID3D11DepthStencilView* myDepthStencilView;

	D3D11_VIEWPORT* myViewPort;
};

