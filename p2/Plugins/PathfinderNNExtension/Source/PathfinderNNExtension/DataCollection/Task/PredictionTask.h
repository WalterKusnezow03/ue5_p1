#pragma once

#include "CoreMinimal.h"
#include "PathfinderNNExtension/DataCollection/PolygonCollection/NNPathFinderProxy.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/ActorTrajectoryTracker.h"

/// Will store task information about the tracked actor
/// and 
/// store the map once the player reappears in the
/// map, with a single pixel drawn
/// ---- will enable asynchronus prediction ----
class PATHFINDERNNEXTENSION_API PredictionTask {

public:
    //is reusable.
    void Setup(ActorTrajectoryTracker *trackedActor); //needed for visibilty check

    void PrepareRequestBinary(TArray<uint8> &bytes); //binary for shared mem python nn

    //returns true if result is ready for NN Backward pass!
    bool TickVisiblityCheck(TArray<uint8> &resultbytes);

    bool IsValid();

    bool IsSameActor(AActor *actorCheck);

private:
    bool taskStarted = false;
    bool taskCompleted = false;

    void PrepareRequestMap(FMeshedPolygonTrajectoryLayered &polygonData);
    void PrepareResultMap();

    NNPathFinderProxy polygonProxy;
    ActorTrajectoryTracker *trackedActorPtr = nullptr;

    FVector locationOfRequest;

    FMeshedPolygonTrajectoryLayered polygonDataCache;
};