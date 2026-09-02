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

#include "PathfinderNNExtension/API/Handle/NNRequestHandle.h"
#include "PathfinderNNExtension/API/Handle/NNRequestHandleTickData.h"

#include "PathfinderNNExtension/API/Handle/NNRequestHandleInterface.h"


#include "NNPathFinderSocket.generated.h"

/// @brief Python socket with Shared Memory Support for Python Script!
/// ---- NN SOCKET BASE WITH SHARED MEMORY SUPPORT----
UCLASS()
class PATHFINDERNNEXTENSION_API ANNPathFinderSocket : 
public ANNSocket,
public INNRequestHandleInterface
{
    GENERATED_BODY()


protected:
    FString frameNameRequest = "DEFAULT_IN";
    FString frameNameResult = "DEFAULT_OUT";
    FString frameNameGroundThruth = "DEFAULT_GT";

    FString frameNameBatch = "DEFAULT_BATCH";

    virtual void OnReceivePythonPrint(FString message) override;


    NNRequestHandle requestHandle;
    FNNRequestHandleTickData requestTickData;

    bool logConnection = false;

public:
    static void MakePathFinderSocketInstance(UWorld *world);

    static ANNPathFinderSocket *PathFinderNNinstance(){
        if(ANNPathFinderSocket *casted = Cast<ANNPathFinderSocket>(instancePtr)){
            return casted;
        }
        return nullptr;
    }
    
    // -- INNRequestHandleInterface override --
    //predicts a node if task not qeued
    virtual void PredictNode(
        IPathfinderNNInterface *interfaceNotify,
        AActor *actor
    ) override;

    
    //connection state of socket to display in widget
    virtual bool NNIsConnected() override;

    //notified by all entites whether the player
    //is visible or not
    virtual void FlagVisible(AActor *actor) override;

    virtual void SetTrainingAllowed() override;
    virtual void EnableHeatMapSaveOnEnd(bool saveOnEnd) override;
    virtual bool HeatMapSaveOnEndEnabled() override;

protected:
    using ANNSocket::WriteData;
    void WriteDataRequest(TArray<uint8> &data, int resultBytes);
    void WriteDataGroundTruth(TArray<uint8> &buffer);
   



    //using ANNSocket::WriteData;
    //void WriteData(const TArray<uint8> &array);
    virtual void Tick(float deltatime) override;

    virtual void TickSocketConnected(float deltatime) override;
    virtual void BeginPlay() override;
    
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;



    



    bool trainingStartAllowed = false;
    
private:
    bool CloseTrainSharedMemoryFrame(const FString &message);
};