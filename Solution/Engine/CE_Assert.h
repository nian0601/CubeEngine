#pragma once

#include "CE_DebugLogger.h"

#define CE_ASSERT(anExpression, ...) CE_DebugLogger::GetInstance()->Assert(anExpression, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);
#define CE_ASSERT_ALWAYS(...) CE_DebugLogger::GetInstance()->Assert(false, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);

//#define CE_ARRAY_BOUNDS_CHECK