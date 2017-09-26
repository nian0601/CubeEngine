#pragma once

class CE_DirectX;
class CE_WindowHandler;

struct ID3D11Device;
struct ID3D11DeviceContext;

class CE_GPUContext
{
public:
	CE_GPUContext(const CE_WindowHandler* aWindowHandler);
	~CE_GPUContext();

	void EndFrame();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetContext() const;

	const CE_Vector2i& GetWindowSize() const;

private:
	CE_DirectX* myDirectX;
};

