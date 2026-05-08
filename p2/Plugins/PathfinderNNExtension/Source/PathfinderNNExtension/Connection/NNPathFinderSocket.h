#pragma once 

#include "CoreMinimal.h"
#include "NNCommunicationPlugin/Communication/Connection/NNSocket.h"
#include "CoreMath/animation/timer/Timer.h"

#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"

#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/NActorTrajectoryTracker.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/MeshedPolygonTrajectoryLayered.h"
#include "PathfinderNNExtension/DataCollection/Task/PredictionTask.h"

#include "PathfinderNNExtension/Request/FPathFinderNNRequestPackage.h"
#include "PathfinderNNExtension/Request/FPathFinderNNRequestQueue.h"
#include "PathfinderNNExtension/Interface/PathfinderNNInterface.h"
#include "PathfinderNNExtension/Storage/FPathFinderNNSampleSet.h"

#include "StoragePlugin/Storage/ImageData/Image/Image.h"

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

    FString frameNameBatch = "DEFAULT_BATCH";

    virtual void OnReceivePythonPrint(FString message) override;

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
        IPathfinderNNInterface *interfaceNotify,
        AActor *actor
    );

    

    //notified by all entites whether the player
    //is visible or not
    void FlagVisible(AActor *actor);

    
protected:
    using ANNSocket::WriteData;
    void WriteDataRequest(TArray<uint8> &data, int resultBytes);
    void WriteDataGroundTruth(TArray<uint8> &buffer);
    void ReadDataResult();
    void TickReadDataResult();



    //using ANNSocket::WriteData;
    //void WriteData(const TArray<uint8> &array);
    virtual void Tick(float deltatime) override;

    virtual void TickSocketConnected(float deltatime) override;
    virtual void BeginPlay() override;
    
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;



    

    NActorTrajectoryTracker actorTracker;

    PredictionTask task;
    void TickTask();

    //request queue for tasks
    FPathFinderNNRequestQueue requests;
    void PredictNextTask();

    void PredictNode(
        AActor *actor
    );
    

    TArray<Image> heatMaps;
    void SaveHeatMapsOnEndPlay();



    FPathFinderNNSampleSet batchTask;
    void LoadBatchIfNotDoneYet();

    //TArray<PredictionTask> tasks;
};