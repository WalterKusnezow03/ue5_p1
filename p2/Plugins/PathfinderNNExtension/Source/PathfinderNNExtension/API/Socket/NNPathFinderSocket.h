#pragma once 

#include "CoreMinimal.h"
#include "NNCommunicationPlugin/Communication/Connection/NNSocket.h"
#include "CoreMath/animation/timer/Timer.h"

#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"

#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/NActorTrajectoryTracker.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/MeshedPolygonTrajectoryLayered.h"
#include "PathfinderNNExtension/DataCollection/Task/PredictionTask.h"

 
  
#include "PathfinderNNExtension/Interface/PathfinderNNInterface.h"

#include "StoragePlugin/Storage/ImageData/Image/Image.h"

#include "PathfinderNNExtension/API/Handle/NNRequestHandle.h"

#include "PathfinderNNExtension/API/Handle/PathFinderNNRequestHandleInterface.h"

#include "NNCommunicationPlugin/API/Socket/NNCommunicationSocket.h"

#include "NNPathFinderSocket.generated.h"

/// @brief Python socket with Shared Memory Support for Python Script!
/// ---- NN SOCKET BASE WITH SHARED MEMORY SUPPORT----
UCLASS()
class PATHFINDERNNEXTENSION_API ANNPathFinderSocket : 
public ANNCommunicationSocket,
public IPathFinderNNRequestHandleInterface
{
    GENERATED_BODY()


protected:
    
    //processes the requests
    NNRequestHandle *GetHandle();

    bool logConnection = false;

public:
    static void MakePathFinderSocketInstance(UWorld *world);

    static ANNPathFinderSocket *PathFinderNNinstance(){
        if(ANNPathFinderSocket *casted = Cast<ANNPathFinderSocket>(instancePtr)){
            return casted;
        }
        return nullptr;
    }
    
    // -- IPathFinderNNRequestHandleInterface overrides --
    //predicts a node if task not qeued
    virtual void PredictNode(
        IPathfinderNNInterface *interfaceNotify,
        AActor *actor
    ) override;
    

    virtual HeatMapReceivers &GetHeatMapReceivers() override;

    virtual bool NNIsConnected() override {
        return ANNCommunicationSocket::NNIsConnected();
    }

    virtual void SetTrainingAllowed() override {
        ANNCommunicationSocket::SetTrainingAllowed();
    }

    //notified by all entites whether the player
    //is visible or not
    virtual void FlagVisible(AActor *actor) override;

    
    virtual void EnableHeatMapSaveOnEnd(bool saveOnEnd) override;
    virtual bool HeatMapSaveOnEndEnabled() override;

protected:
   
    void UpdateSelectedModelToSharedMemory();


    //using ANNSocket::WriteData;
    //void WriteData(const TArray<uint8> &array);
    virtual void Tick(float deltatime) override;

    virtual void TickSocketConnected(float deltatime) override;
    virtual void BeginPlay() override;
    
private:

    HeatMapReceivers empty;
};