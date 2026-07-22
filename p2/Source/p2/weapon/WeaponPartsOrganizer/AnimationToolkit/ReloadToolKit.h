#pragma once

#include "CoreMinimal.h"


class P2_API ReloadToolKit {

public:

    void Setup(AActor *weapon, UStaticMeshComponent *mag);
    void Setup(USceneComponent *root, UStaticMeshComponent *mag);

    void TickReload(float DeltaTime);

private:
    UStaticMeshComponent *mag1 = nullptr;
    UStaticMeshComponent *mag2 = nullptr;

    UStaticMeshComponent *Duplicate(USceneComponent *root, UStaticMeshComponent *OriginalMeshComp);

    FVector magOriginalRelativeLocation;
    FRotator magOriginalRelativeRotation;

    //alternativ einfacher:
    //mag eject

    //mag dynamic move from plate carrier






    /*//mag 2 annähern
    KeyFrameAnimation magMoveToWeapon;

    //mag wechseln
    KeyFrameAnimation paralellReload;

    //mag 1 weg tun
    KeyFrameAnimation magPutAway;

    //mag 1 hide

    //swap mag ptrs*/





    //plate carrier ref needed for a valid start position
    //mag swap into free slot
    //lerp with generated bspline
};