#pragma once

#include "CoreMinimal.h"


UENUM()
enum class ESlipMode
{
    ESlipDynamic, 
    ESlipDynamicLiftOffPrediction,
    ESlipStatic
};