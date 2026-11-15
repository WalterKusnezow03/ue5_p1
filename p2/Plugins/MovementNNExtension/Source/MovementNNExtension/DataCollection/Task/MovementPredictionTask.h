#pragma once

#include "CoreMinimal.h"
#include "ActorTracking/Tracking/TrajectoryCollection/ActorTracker/ActorTrajectoryTracker.h"

#include "NNCommunicationPlugin/GameNN/ModelInput/FONNXModelInput.h"

#include "NNCommunicationPlugin/DataCollection/Task/PredictionTaskBase.h"

#include "MovementNNExtension/DataCollection/Sample/FMovementSample.h"

#include "ActorTracking/Tracking/TrajectoryCollection/Notify/NotifyActorTrajectoryTrackerChanged.h"

class FNNRequestPackage;


/// Will store task information about the tracked actor
/// and 
/// store the map once the player reappears in the
/// map, with a single pixel drawn
/// ---- will enable asynchronus prediction ----
class MOVEMENTNNEXTENSION_API MovementPredictionTask : 
public PredictionTaskBase, 
public INotifyActorTrajectoryTrackerChanged {

private:
    FMovementSample sampleData;

public:
    void Reset() override;

    //is reusable.
    void Setup(ActorTrajectoryTracker *trackedActor); //needed for visibilty check

    void PrepareRequestBinary(TArray<uint8> &bytes) override; //binary for shared mem python nn
    void PrepareRequestBinary(FONNXModelInput &model) override; //onnx input for ONNX Model

    //returns true if result is ready for NN Backward pass!
    bool TickVisiblityCheckAndPrepareGroundTruthBinary(TArray<uint8> &resultbytes) override;

    bool IsValid() override;

    bool IsSameActor(AActor *actorCheck);



    

    void GenerateAndNotifyResultData(FNNRequestPackage *queue) override;
    
    void EmbedRequestData(FNNRequestPackage &queue) override;

    //todo: set type for prediction task / can be swicthed
    //todo: get type of prediction task

    ///@brief returns the sample data (the Trajectory Layered Polygon data sample)
    SampleInterface *GetSampleData() override;


    //notify by actor trajectory tracker
    void OnTrajectoryAddedWorldSpace(Trajectory other) override;

private:
    void PrepareRequestMap(FMovementSample &polygonData);
    void PrepareResultMap();

    //targeted actor
    ActorTrajectoryTracker *trackedActorPtr = nullptr;

    FVector locationOfRequest;    

    //Unet input as 144 144, do not change, first check py script!!
    void ResizeToNNSize(FMovementSample &polygonData);
    void Resize(FMovementSample &polygonData, int x, int y);

    void UnsubscribeFromActorTracker();
    void SubscribeToActorTracker();
};