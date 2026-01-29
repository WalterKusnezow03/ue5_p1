#pragma once
#include "CoreMinimal.h"

//game actors which are not nesecarrily weapons or entities like bots
UENUM()
enum class EGameActorEnum
{
    ELoadoutRoom,
    EGameStartRoom,
    EPauseRoom,
};