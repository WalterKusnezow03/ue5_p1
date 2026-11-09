#pragma once
#include "CoreMinimal.h"
#include "PathfinderNNExtension/API/Handle/PathFinderNNRequestHandleInterface.h"
#include "NNCommunicationPlugin/GameNN/ONNXModel.h"
#include "NNCommunicationPlugin/API/Handle/NNRequestHandleTickDataOnnx.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Base/EPolygonSampleType.h"
#include <map>

#include "NNCommunicationPlugin/API/ONNX/NNCommunicationONNX.h"

#include "PathfinderNNExtension/API/Handle/NNRequestHandle.h"

#include "NNPathFinderONNX.generated.h"

//will decide between socket and ONNX Model
//public api for all prediction tasks:
UCLASS()
class PATHFINDERNNEXTENSION_API ANNPathFinderONNX : 
public ANNCommunicationONNX,
public IPathFinderNNRequestHandleInterface
{
    GENERATED_BODY()

protected:
    NNRequestHandle *GetHandle();

    virtual void OnBeginPlayLoadModels() override;
    virtual void OnBeginPlaySetupHandleStructure() override;

    ONNXModel *FindSelectedModel() override;

    void LoadModel(EPolygonSampleType type);

    bool logConnection = false;

public:
    static ANNPathFinderONNX *ApiInstance();
    static void MakePathFinderONNXInstance(UWorld *World);

    ANNPathFinderONNX();

   

    

    //todo
    virtual HeatMapReceivers &GetHeatMapReceivers() override;

protected:
    virtual void BeginPlay() override;
    
    

    HeatMapReceivers none;

public:
    //predicts a node if task not qeued
    virtual void PredictNode(
        IPathfinderNNInterface *interfaceNotify,
        AActor *actor
    ) override;

    

    //notified by all entites whether the player
    //is visible or not
    virtual void FlagVisible(AActor *actor) override;

    //void SetTrainingAllowed();

    virtual void EnableHeatMapSaveOnEnd(bool saveOnEnd) override;
    virtual bool HeatMapSaveOnEndEnabled() override;

private:
    bool PrepareRequestData(float deltatime);
};