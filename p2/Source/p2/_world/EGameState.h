#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EGameState
{
    EGameLaunchScreen,
    EPauseScreen,
    EGamePlay,
    ESetupRoom,
};