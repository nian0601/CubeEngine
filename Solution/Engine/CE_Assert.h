#pragma once

#define CE_ASSERT(anExpression, ...) CE_DebugLogger::GetInstance()->Assert(anExpression, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);
