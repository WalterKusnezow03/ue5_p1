#pragma once

#include "CoreMinimal.h"
#include "ActorTracking/Tracking/TrajectoryCollection/Container/Trajectory.h"


class ACTORTRACKING_API INotifyActorTrajectoryTrackerChanged {

public:
    virtual void OnTrajectoryAddedWorldSpace(Trajectory other) {};
};