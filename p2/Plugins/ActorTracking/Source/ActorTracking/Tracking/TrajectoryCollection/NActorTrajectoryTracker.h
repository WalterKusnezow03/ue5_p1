#pragma once

#include "CoreMinimal.h"
#include "ActorTracking/Tracking/TrajectoryCollection/ActorTracker/ActorTrajectoryTracker.h"
#include <map>
#include "ActorTracking/Tracking/TrajectoryCollection/Container/Trajectory.h"

/// @brief tracks N actor trajectories in a tick intervall
/// Tick needs to be called from outside!
class ACTORTRACKING_API NActorTrajectoryTracker {

public:
    //should only be called once.
    void Setup(float intervall, int listmaxSize);

    //adds if not tracked yet
    void AddTrackedActorIfNeeded(AActor *actor);
    void RemoveTrackedActor(AActor *actor);

    void Tick(float deltaTime);

    //local to current actorLocation
    TArray<Trajectory> LocalTrajectories(AActor *actor);
    TArray<Trajectory> worldTrajectories(AActor *actor);
    bool HasTracked(AActor *actor);

    //flags a aactor as spotted for trajectory collection
    //trajectories are only collected if the player is visible
    void FlagVisible(AActor *actor);


    ActorTrajectoryTracker *FindIfTracked(AActor *actor);

protected:
    float intervallShared = 1.0f;
    int listMaxSizeShared = 10;

    std::map<FString, ActorTrajectoryTracker> trackerMap;
    bool HasTracked(FString name);
    
};
