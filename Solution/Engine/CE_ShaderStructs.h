#pragma once

#include "CE_Matrix44.h"
#include "CE_Vector4.h"
#include "CE_Vector3.h"

//
// Global Data structs
//

struct CE_ProjectionData
{
	CE_Matrix44f myProjection;
};

struct CE_ViewProjectionData
{
	CE_Matrix44f myView;
	CE_Matrix44f myProjection;
};



//
// Object Data structs
//

struct CE_ModelShaderData
{
	CE_Matrix44f myWorld;
	CE_Vector4f myColor;
	CE_Vector4f myScale;
};

struct CE_SpriteShaderData
{
	CE_Vector4f myColor;
	CE_Vector2f myPosition;
	CE_Vector2f mySize;
	CE_Vector2f myHotspot;
	CE_Vector2f padding;
};

struct CE_FullscreenQuadShaderData
{
	CE_Matrix44f myInvertedProjection;
	CE_Matrix44f myNotInvertedView;
	CE_Vector3f myCameraPosition;
	float padding;
};

//
// Vertex Data structs
//

struct CE_PosNormColor_Vert
{
	CE_Vector3f myPosition;
	CE_Vector3f myNormal;
	CE_Vector4f myColor;
};

struct CE_Pos_Vert
{
	CE_Vector3f myPosition;
};

struct CE_Pos_UV_Vert
{
	CE_Vector3f myPosition;
	CE_Vector2f myTexCoord;
};