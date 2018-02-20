#pragma once

#include "CE_DebugRenderManager.h"
#include "CE_Engine.h"

#define CE_DRAW_LINE(aStart, aEnd) {CE_Engine::GetDebugRenderManager().myLines.Add(CE_Line(aStart, aEnd));}
#define CE_DRAW_LINE_COLOR(aStart, aEnd, aColor) {CE_Engine::GetDebugRenderManager().myLines.Add(CE_Line(aStart, aEnd, aColor));}
#define CE_DRAW_LINE_TWO_COLOR(aStart, aEnd, aColor1, aColor2) {CE_Engine::GetDebugRenderManager().myLines.Add(CE_Line(aStart, aEnd, aColor1, aColor2));}