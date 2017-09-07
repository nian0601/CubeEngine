#pragma once

struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;

class CE_Model
{
public:
	CE_Model();
	~CE_Model();

	void Init(ID3D11Device* aDevice);
	void Shutdown();
	void Render(ID3D11DeviceContext* aContext);

	int GetIndexCount();

private:
	struct VertexType
	{
		CE_Vector3f myPosition;
		CE_Vector4f myColor;
	};

	ID3D11Buffer* myVertexBuffer;
	int myVertexCount;

	ID3D11Buffer* myIndexBuffer;
	int myIndexCount;
};

