#pragma once

#include "CoreMinimal.h"
#include "PathfinderNNExtension/DataCollection/PolygonCollection/NNPathFinderProxy.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/ActorTrajectoryTracker.h"
#include "StoragePlugin/Storage/ImageData/Image/Image.h"

/// Will store task information about the tracked actor
/// and 
/// store the map once the player reappears in the
/// map, with a single pixel drawn
/// ---- will enable asynchronus prediction ----
class PATHFINDERNNEXTENSION_API PredictionTask {

private:
    float radiusMeter = 100.0f; //stays the same! Do not change!


public:
    void Reset();

    //is reusable.
    void Setup(ActorTrajectoryTracker *trackedActor); //needed for visibilty check

    void PrepareRequestBinary(TArray<uint8> &bytes); //binary for shared mem python nn

    //returns true if result is ready for NN Backward pass!
    bool TickVisiblityCheckAndPrepareGroundTruthBinary(TArray<uint8> &resultbytes);

    bool IsValid();

    bool IsSameActor(AActor *actorCheck);

    FMeshedPolygonTrajectoryLayered &GetPolygonData();

    bool WaitingForGroundTruth();
    bool TaskCompleted();

    int ResultGridSizeBytes();

    // heat map generation from result
    void GenerateMapFromResultBytes(const TArray<uint8> &buffer);
    

    void ColoredHeatMap(
        Image &image,
        FColor colorMin,
        FColor colorMax,
        FColor colorPolygonFlagged,
        FColor colorTrjacetory,
        FColor playerPosResult
    );

private:
    bool taskStarted = false;
    bool taskCompleted = true;

    void PrepareRequestMap(FMeshedPolygonTrajectoryLayered &polygonData);
    void PrepareResultMap();

    NNPathFinderProxy polygonProxy;
    ActorTrajectoryTracker *trackedActorPtr = nullptr;

    FVector locationOfRequest;

    FMeshedPolygonTrajectoryLayered polygonDataCache;
};