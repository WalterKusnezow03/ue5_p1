#pragma once
#include "CoreMinimal.h"

//game actors which are not nesecarrily weapons or entities like bots
UENUM()
enum class EPauseWidgetEvent
{
    EOpenLoadoutRoom,
    EOpenGameStartRoom,
    EExitPauseRoom,
    EOpenSettingsWidget,
    ECloseSettingsWidget
};