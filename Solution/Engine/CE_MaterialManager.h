#pragma once

struct CE_Material
{
	CE_Vector4f myAlbedo;
	CE_Vector4f myDiffuse;
	CE_Vector4f mySpecular;
	float myMetalness;
	float myRoughness;
};

class CE_MaterialManager
{
public:
	static void Create(const char* aMaterialPath);
	static void Destroy();
	static CE_MaterialManager* GetInstance() { return ourInstance; }

	const CE_Material* GetMaterial(const char* aMaterialName) const;

private:
	CE_MaterialManager(const char* aMaterialsPath);

	void LoadMaterial(const char* aMaterialPath);

	CE_Map<CE_String, CE_Material> myMaterials;

	static CE_MaterialManager* ourInstance;
};
