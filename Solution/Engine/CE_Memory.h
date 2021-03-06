#pragma once

#define CE_SAFE_DELETE(aPointer) {delete aPointer; aPointer = nullptr;}
#define CE_SAFE_DELETE_ARRAY(aPointer) {delete[] aPointer; aPointer = nullptr;}

#define CE_SAFE_RELEASE(aPointer) \
if(aPointer) \
{ \
	aPointer->Release(); \
	aPointer = nullptr; \
}

#define CE_ZERO_MEMORY(aObject) {memset(&aObject, 0, sizeof(aObject));}
#define CE_MEM_COPY(aDestination, aSource, aSize) {memcpy(aDestination, aSource, aSize);}