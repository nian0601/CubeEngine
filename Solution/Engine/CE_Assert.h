#pragma once

#include "CE_DebugLogger.h"

#define CE_DEBUG_BREAK __debugbreak();
#define CE_ASSERT(anExpression, ...) \
{\
if(!(anExpression)) \
{\
	CE_DEBUG_BREAK;\
	CE_DebugLogger::GetInstance()->Assert(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);\
}\
}\

#define CE_ASSERT_ALWAYS(...) CE_DebugLogger::GetInstance()->Assert(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);
#define CE_CT_ASSERT(anExpression, aMessage) static_assert(anExpression, aMessage);

#define CE_ARRAY_BOUNDS_CHECK