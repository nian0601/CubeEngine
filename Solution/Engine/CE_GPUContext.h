#pragma once

class CE_DirectX;
class CE_WindowHandler;

struct ID3D11Device;
struct ID3D11DeviceContext;

class CE_GPUContext
{
public:
	CE_GPUContext(const CE_DirectX& aDirectX);
	~CE_GPUContext();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetContext() const;

private:
	const CE_DirectX& myDirectX;
};

