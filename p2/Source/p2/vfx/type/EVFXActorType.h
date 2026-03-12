#pragma once
#include "CoreMinimal.h"

//game actors which are not nesecarrily weapons or entities like bots
UENUM()
enum class EVFXActorType
{
    ENone,
    EExplosion,
    ESmoke,
};