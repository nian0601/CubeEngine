#pragma once

#include "CE_GrowingArray.h"
#include "CE_StaticArray.h"

#define MAX_NUMBER_OF_COMPONENTS 32

struct CE_BaseComponent;

typedef unsigned int CE_Entity;
static const CE_Entity CE_Invalid_Entity = static_cast<unsigned int>(-1);

typedef CE_GrowingArray<CE_BaseComponent*> CE_ComponentArray;
typedef CE_StaticArray<int, MAX_NUMBER_OF_COMPONENTS> CE_EntityComponentArray;
