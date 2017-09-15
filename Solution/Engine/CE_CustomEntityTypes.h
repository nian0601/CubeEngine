#pragma once

#include "CE_GrowingArray.h"
#include "CE_StaticArray.h"

#define MAX_NUMBER_OF_COMPONENTS 32

class CE_BaseComponent;

typedef unsigned int CE_Entity;

typedef CE_GrowingArray<CE_BaseComponent*> CE_ComponentArray;
typedef CE_StaticArray<int, MAX_NUMBER_OF_COMPONENTS> CE_EntityComponentArray;
