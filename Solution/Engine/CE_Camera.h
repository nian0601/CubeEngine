#pragma once
class CE_Camera
{
public:
	CE_Camera(const CE_Vector2i& aSize);
	~CE_Camera();

	void Update();

	void Rotate(const CE_Matrix44f& aRotation);
	void SetPosition(const CE_Vector3f& aPosition);
	void Move(const CE_Vector3f& aMoveVector);

	const CE_Matrix44f& GetProjection() const { return myProjection; }
	const CE_Matrix44f& GetInvertedProjection() const { return myInvertedProjection; }
	const CE_Matrix44f& GetView() const { return myView; }
	const CE_Matrix44f& GetNotInvertedView() const { return myOrientation; }
	const CE_Matrix44f& GetOrthagonalProjection() const { return myOrthagonalProjection; }

private:
	CE_Matrix44f myOrientation;
	CE_Matrix44f myView;
	CE_Matrix44f myProjection;
	CE_Matrix44f myInvertedProjection;
	CE_Matrix44f myOrthagonalProjection;

	bool myIsDirty;
};