#pragma once

#include "CoreMinimal.h"

/// @brief add ignored actors here which can be removed later by ptr.
/// provides FCollision params, trace complex set to false
/// Is needed because FCollisionQueryParams cant remove by pointer!
class IKHUMANOIDMODELL_API CollisionTracker {

public:
    CollisionTracker();
    ~CollisionTracker();


    void AddIgnoredActor(AActor *actor);
    void RemoveIgnoredActor(AActor *actor);

    FCollisionQueryParams getCollisonParams();

private:
    TArray<AActor *> tracked;
};