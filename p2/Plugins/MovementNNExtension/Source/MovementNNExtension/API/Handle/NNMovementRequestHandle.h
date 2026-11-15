#pragma once
#include "CoreMinimal.h"


#include "NNCommunicationPlugin/API/Handle/NNRequestHandleBase.h"
#include "MovementNNExtension/DataCollection/Task/MovementPredictionTask.h"
#include "ActorTracking/Tracking/TrajectoryCollection/NActorTrajectoryTracker.h"

#include "NNCommunicationPlugin/Storage/TNNSampleSetBase.h"

//Base Handle for requests: register actors / prepare data
//and process prediction data
// Request processing is temporarily limited to queue and task plumbing.
// Note: this is a framework class - derive it and implement the missing classes and behaviour.
class MOVEMENTNNEXTENSION_API NNMovementRequestHandle : public NNRequestHandleBase {

    

public:
    

    virtual void BeginPlay() override;

    virtual void PredictData(
        IBaseNNInterface *interfaceNotify,
        FString nameTaskTarget,
        UObject *targetObject = nullptr
    ) override;


protected:
    MovementPredictionTask *GetTask();
    virtual FNNSampleSetBase *GetBatchTask() override;
    TNNSampleSetBase<FMovementSample> batchData;

    virtual void OnBeginPlaySetupTask() override;
    virtual void OnBeginPlaySetupBatchTask() override;


    virtual void OnTick(float deltatime) override;

    NActorTrajectoryTracker actorTracker;
  

    virtual bool PrepareTaskFor(FNNRequestPackage *package) override;
    virtual void OnTickTaskWaitForGroundTruth() override;
    virtual void OnTickTaskArrivedGroundTruth() override;
    virtual void OnTickTaskArrivedGroundTruthValid() override;
    virtual void OnPredictionArrive() override;

    

private:
    void SubscribeActorIfNeeded(
        UObject *targetObject
    );
};