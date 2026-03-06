#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EBuildingTypeBlock
{
    ENone,
    EOutsideWall,
    EInsideWall,
    EOutsideFloor,
    EInsideFloor,
    ERoof,
};