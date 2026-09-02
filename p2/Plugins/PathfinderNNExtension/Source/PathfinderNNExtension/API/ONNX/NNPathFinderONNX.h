#pragma once
#include "CoreMinimal.h"
#include "PathfinderNNExtension/API/Handle/NNRequestHandleInterface.h"
#include "PathfinderNNExtension/GameNN/ONNXModel.h"
#include "PathfinderNNExtension/API/Handle/NNRequestHandleTickDataOnnx.h"

#include "NNPathFinderONNX.generated.h"

//will decide between socket and ONNX Model
//public api for all prediction tasks:
UCLASS()
class PATHFINDERNNEXTENSION_API ANNPathFinderONNX : 
public AActor,
public INNRequestHandleInterface
{
    GENERATED_BODY()

private:
    static ANNPathFinderONNX *instancePtr;
    ONNXModel model;

    bool logConnection = false;

public:
    static ANNPathFinderONNX *ApiInstance();
    static void MakePathFinderONNXInstance(UWorld *World);

    ANNPathFinderONNX();

    virtual bool NNIsConnected() override;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    NNRequestHandle requestHandle;
    //FNNRequestHandleTickData requestTickData;
    FNNRequestHandleTickDataOnnx requestTickData;

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