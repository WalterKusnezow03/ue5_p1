#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM()
enum class ArmMotionStates
{
	none,
    changingState, //state currently in transition (thinking about it)
    handsFollowItem,
    running
};