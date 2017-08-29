#pragma once

#define CE_SAFE_DELETE(aPointer) {delete aPointer; aPointer = nullptr;}
#define CE_SAFE_DELETE_ARRAY(aPointer) {delete[] aPointer; aPointer = nullptr;}