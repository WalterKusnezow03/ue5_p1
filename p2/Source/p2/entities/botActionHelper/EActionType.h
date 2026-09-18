#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EActionType
{
    ENone, //empty action
    EMoveToPlayer,
    EMoveToSpecialPosition,
    ERoam,
    EWait
};