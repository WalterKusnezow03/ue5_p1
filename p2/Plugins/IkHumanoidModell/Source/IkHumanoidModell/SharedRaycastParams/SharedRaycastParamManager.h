#pragma once

#include "CoreMinimal.h"
#include "GameCore/Raycast/query/CollisionTracker.h"
#include "SharedRaycastParamManager.generated.h"

/// @brief will track all skelletons and items in a collision list and 
/// automatically make an instance of this object if not created for this world yet
/// will share all collision params of all skelleton limbs and items
UCLASS()
class IKHUMANOIDMODELL_API ASharedRaycastParamManager : public AActor {
    GENERATED_BODY()

public:

    /// @brief gets the collison params with ignored actor list internal from 
    /// ASharedRaycastParamManager instance - is all skelletons tracked, so they dont ontop of each other.
    /// carried items tracked too!
    static FCollisionQueryParams getCollisonParams();

    // -- upon removing or adding a new actor the instance of this single ton is created in background --
    // ->plugin launch not needed.
    static void Add(AActor *actor);
    static void Remove(AActor *actor);

    static const CollisionTracker *getShared();

    static UWorld *world();

protected:
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    static ASharedRaycastParamManager *instance;

    //collision params tracking
    CollisionTracker sharedCollisionParamsProvider;

    ///@brief creates a Manager instane if needed (if a skelleton is created, the first one will launch the
    /// manager, if world ended, new is needed.)
    ///Note: A Skelleton cant die yet and should maybe release its actors here for reusuage pool
    static void MakeInstanceIfNeeded(UWorld *world);
    static void MakeInstanceIfNeeded(AActor *actor);

};