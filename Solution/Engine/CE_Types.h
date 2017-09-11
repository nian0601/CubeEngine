#pragma once

enum CE_DepthState
{
	ENABLED,
	DISABLED,
	READ_NO_WRITE,
	NO_READ_NO_WRITE,
	_DEPTH_COUNT
};

enum CE_RasterizerState
{
	CULL_BACK,
	WIRE_FRAME,
	NO_CULLING,
	_RAZTER_COUNT
};

enum CE_BlendState
{
	NO_BLEND,
	ALPHA_BLEND,
	_BLEND_COUNT
};