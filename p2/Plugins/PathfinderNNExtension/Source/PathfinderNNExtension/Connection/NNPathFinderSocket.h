#pragma once 

#include "CoreMinimal.h"
#include "NNCommunicationPlugin/Communication/Connection/NNSocket.h"
#include "CoreMath/animation/timer/Timer.h"

#include "PathfinderNNExtension/DataCollection/PolygonCollection/NNPathFinderProxy.h"
#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"

#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/NActorTrajectoryTracker.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/MeshedPolygonTrajectoryLayered.h"
#include "PathfinderNNExtension/DataCollection/Task/PredictionTask.h"

#include "NNPathFinderSocket.generated.h"

/// @brief Python socket with Shared Memory Support for Python Script!
/// ---- NN SOCKET BASE WITH SHARED MEMORY SUPPORT----
UCLASS()
class PATHFINDERNNEXTENSION_API ANNPathFinderSocket : public ANNSocket {
    GENERATED_BODY()


protected:
    FString frameNameRequest = "DEFAULT_IN";
    FString frameNameResult = "DEFAULT_OUT";
    FString frameNameGroundThruth = "DEFAULT_GT";

public:
    static void MakePathFinderSocketInstance(UWorld *world);

    static ANNPathFinderSocket *PathFinderNNinstance(){
        if(ANNPathFinderSocket *casted = Cast<ANNPathFinderSocket>(instancePtr)){
            return casted;
        }
        return nullptr;
    }

    //predicts a node if task not qeued
    void PredictNode(
        AActor *actor,
        float radius
    );

    //notified by all entites whether the player
    //is visible or not
    void FlagVisible(AActor *actor);

    //debug function, no result, instant write data.
    void PredictNodeDebug(
        AActor *actor,
        float radius
    );

protected:
    using ANNSocket::WriteData;
    //void WriteData(FMeshedPolygon &polygon);
    void WriteDataRequest(FMeshedPolygonTrajectoryLayered &polygon);

    //using ANNSocket::WriteData;
    //void WriteData(const TArray<uint8> &array);
    virtual void Tick(float deltatime) override;

    virtual void TickSocketConnected(float deltatime) override;
    virtual void BeginPlay() override;
    
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    NNPathFinderProxy proxy;

    NActorTrajectoryTracker actorTracker;



    TArray<PredictionTask> tasks;
};