#pragma once

#include "CoreMinimal.h"
#include "ActorTracking/Tracking/TrajectoryCollection/Container/Trajectory.h"
#include "ActorTracking/Tracking/TrajectoryCollection/Container/TrajectoryCollection.h"
#include "CoreMath/animation/timer/Timer.h"

/// @brief tracks a single actor trajectory in a tick intervall
/// Tick needs to be called from outside!
/// on setup filled with current location until max size passed reached.
class ACTORTRACKING_API ActorTrajectoryTrackerBase {

public:
    void Setup(AActor *actor, float intervall, int listMaxSize);
    void Tick(float deltatime);


    TArray<Trajectory> localTrajectories() const;
    TArray<Trajectory> worldTrajectories() const;
    TArray<Trajectory> worldTrajectoriesNormalizedTime() const;

    //spotted by any bot or player
    void FlagVisible();
    bool IsFlaggedVisibleReset();
    void SetUseVisibility(bool inFlag);
    

    FVector ActorLocation();

    AActor *GetActor();

protected:
    virtual void OnTrajectoryAdded();
    TrajectoryCollection collection;
private:
    AActor *trackedActor = nullptr;
    
    Timer timer;

    float intervallSaved = 1.0f;

    bool isVisible = false;
    bool useVisibilityFlag = false;

    void FillListOnSetup();
};