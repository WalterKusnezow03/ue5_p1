#pragma once

#include "CoreMinimal.h"
#include "ActorTrajectoryTrackerBase.h"
#include "ActorTracking/Tracking/TrajectoryCollection/Notify/NotifyActorTrajectoryTrackerChanged.h"

/// @brief extends ActorTrajectoryTrackerBase functionality
/// by allowing subsribers on trajectory change (add / update of trajectory list)
class ACTORTRACKING_API ActorTrajectoryTracker : public ActorTrajectoryTrackerBase {

public:
    void AddSubscriber(INotifyActorTrajectoryTrackerChanged *interface);
    void RemoveSubscriber(INotifyActorTrajectoryTrackerChanged *interface);

protected:
    virtual void OnTrajectoryAdded() override;

private:
    TArray<INotifyActorTrajectoryTrackerChanged *> subscribed;
};