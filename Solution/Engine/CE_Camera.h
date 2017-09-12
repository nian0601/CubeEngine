#pragma once
class CE_Camera
{
public:
	CE_Camera(const CE_Vector2i& aSize);
	~CE_Camera();

	void Update();

	void Rotate(const CE_Matrix44f& aRotation);
	void SetPosition(const CE_Vector3f& aPosition);

	const CE_Matrix44f& GetProjection() const { return myProjection; }
	const CE_Matrix44f& GetView() const { return myView; }

private:
	CE_Matrix44f myOrientation;
	CE_Matrix44f myView;
	CE_Matrix44f myProjection;

	bool myIsDirty;
};

