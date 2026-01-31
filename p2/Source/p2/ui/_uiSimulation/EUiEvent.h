#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EUiEvent
{
    HudTopText,
    HudTopTextTimed,
    HudMiniMapRegisterActor,
    HudMiniMapUnRegisterActor,
};