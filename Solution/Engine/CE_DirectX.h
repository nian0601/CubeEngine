#pragma once

struct ID3D11Debug;
struct ID3D11Device;
struct ID3D11DeviceContext;

class CE_DirectX
{
public:
	CE_DirectX();
	~CE_DirectX();

	ID3D11Device* GetDevice() const { return myDevice; }
	ID3D11DeviceContext* GetContext() const { return myDeviceContext; }

private:
	void SetupDevice();
	void SetupDebugInterface();

	ID3D11Debug* myDebugInterface;
	ID3D11Device* myDevice;
	ID3D11DeviceContext* myDeviceContext;
};

