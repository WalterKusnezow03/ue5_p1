#pragma once
#include "CoreMinimal.h"
#include "NNCommunicationPlugin/API/Handle/NNRequestHandleBase.h"
#include "NNCommunicationPlugin/GameNN/ONNXModel.h"
#include "NNCommunicationPlugin/GameNN/FONNXModelsetupBase.h"
#include "NNCommunicationPlugin/API/Handle/NNRequestHandleTickDataOnnx.h"
#include <map>

#include "NNCommunicationPlugin/API/RequestTakerInterface/NNRequestTakerInterfaceBase.h"

#include "NNCommunicationONNX.generated.h"

//will decide between socket and ONNX Model
//public api for all prediction tasks:
UCLASS()
class NNCOMMUNICATIONPLUGIN_API ANNCommunicationONNX : 
public AActor,
public INNRequestTakerInterfaceBase
{
    GENERATED_BODY()

protected:
    static ANNCommunicationONNX *instancePtr;

    virtual void OnBeginPlaySetupHandleStructure();
    virtual void OnEndPlayCleanUpHandleStructure();
    virtual void OnBeginPlayLoadModels();



    ONNXModel modelTrajectoryLayered;
    ONNXModel rayModel;

    virtual ONNXModel *FindSelectedModel();
    ONNXModel *FindSelectedModel(FString name);

    void LoadModel(FONNXModelsetupBase &setup, FString name);

    FString selectedModel = "NONE";
    std::map<FString, ONNXModel> models;

    bool logConnection = false;

public:

    ANNCommunicationONNX();

    virtual bool NNIsConnected() override;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    NNRequestHandleBase *requestHandle = nullptr;
    //FNNRequestHandleTickData requestTickData;
    FNNRequestHandleTickDataOnnx *requestTickData = nullptr;

public:
    //predicts a node if task not qeued
    // Temporarily kept as a direct socket API while the request interface is disabled.
    void PredictData(
        IBaseNNInterface *interfaceNotify,
        FString name,
        UObject *targetObject = nullptr
    ) override {
        if(requestHandle){
            requestHandle->PredictData(interfaceNotify, name, targetObject);
        }
    }


private:
    bool PrepareRequestData(float deltatime);
};