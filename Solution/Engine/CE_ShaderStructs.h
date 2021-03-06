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

struct CE_GlobalPBLData
{
	CE_Matrix44f myView;
	CE_Matrix44f myProjection;
	CE_Matrix44f myInvertedProjection;
	CE_Matrix44f myNotInvertedView;
	CE_Vector3f myCameraPosition;
	CE_Vector2f myScreenSize;
	CE_Vector3f padding;
};


//
// Object Data structs
//

struct CE_ModelShaderData
{
	CE_Matrix44f myWorld;
	CE_Vector4f myColorAndMetalness;
	CE_Vector4f myScaleAndRoughness;
};

struct CE_SpriteShaderData
{
	CE_Vector4f myColor;
};

struct CE_PointLightShaderData
{
	CE_Matrix44f myWorld;
	CE_Vector4f myColorAndIntensity;
	CE_Vector4f myRadius;
};

//
// Vertex Data structs
//

struct CE_PosNormColor_Vert
{
	CE_Vector4f myPosition;
	CE_Vector4f myNormal;
	CE_Vector4f myColor;
};

struct CE_Pos_Vert
{
	CE_Vector4f myPosition;
};

struct CE_Pos_UV_Vert
{
	CE_Vector4f myPosition;
	CE_Vector2f myTexCoord;
};