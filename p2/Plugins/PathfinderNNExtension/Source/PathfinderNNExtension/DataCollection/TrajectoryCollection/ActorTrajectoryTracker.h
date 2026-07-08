#pragma once

#include "CoreMinimal.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/Container/TrajectoryCollection.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/Container/Trajectory.h"
#include "CoreMath/animation/timer/Timer.h"

/// @brief tracks a single actor trajectory in a tick intervall
/// Tick needs to be called from outside!
/// on setup filled with current location until max size passed reached.
class PATHFINDERNNEXTENSION_API ActorTrajectoryTracker {

public:
    void Setup(AActor *actor, float intervall, int listMaxSize);
    void Tick(float deltatime);


    TArray<Trajectory> localTrajectories() const;
    TArray<Trajectory> worldTrajectories() const;
    TArray<Trajectory> worldTrajectoriesNormalizedTime() const;

    //spotted by any bot or player
    void FlagVisible();
    bool IsFlaggedVisibleReset();

    FVector ActorLocation();

    AActor *GetActor();

private:
    AActor *trackedActor = nullptr;
    TrajectoryCollection collection;
    Timer timer;

    float intervallSaved = 1.0f;

    bool isVisible = false;

    void FillListOnSetup();
};