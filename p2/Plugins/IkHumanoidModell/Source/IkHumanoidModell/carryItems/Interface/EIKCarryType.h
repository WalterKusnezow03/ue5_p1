#pragma once

#include "CoreMinimal.h"

///@brief important difference between carried by the skelleton socket
///for weapons (like rifle, pistol, etc)
///and by hand (greande, compatible with generic arm animations)
UENUM()
enum class EIKCarryType
{
    ECarryBySocket, //default arm follow (IK) -> rifle / pistol, default weapons
    ECarryByHand    //item follow arm / hand (Item attached) -> single hand items: grenade, etc
};