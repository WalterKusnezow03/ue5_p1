#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EVerschlussState
{
    ENone, 
    EDefaultKickback,
    EEmptyMagKickback,
    EEmptyReloadRevert,
};