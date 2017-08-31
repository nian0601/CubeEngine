#pragma once

#define CE_SAFE_DELETE(aPointer) {delete aPointer; aPointer = nullptr;}
#define CE_SAFE_DELETE_ARRAY(aPointer) {delete[] aPointer; aPointer = nullptr;}

#define CE_SAFE_RELEASE(aPointer) {aPointer->Release(); aPointer = nullptr;}

#define CE_ZERO_MEMORY(aObject) {ZeroMemory(&aObject, sizeof(aObject));}