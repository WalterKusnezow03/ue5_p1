#pragma once
#include "CoreMinimal.h"
#include "NNCommunicationPlugin/GameNN/ONNXModel.h"
#include "NNCommunicationPlugin/API/Handle/NNRequestHandleTickDataOnnx.h"
#include "NNCommunicationPlugin/API/ONNX/NNCommunicationONNX.h"
#include "MovementNNExtension/API/SocketInterface/NNMovementRequestHandleInterface.h"
#include "MovementNNExtension/API/Handle/NNMovementRequestHandle.h"

#include "NNMovementONNX.generated.h"

//will decide between socket and ONNX Model
//public api for all prediction tasks:
UCLASS()
class MOVEMENTNNEXTENSION_API ANNMovementONNX : 
public ANNCommunicationONNX,
public INNMovementRequestHandleInterface
{
    GENERATED_BODY()

protected:
    static ANNMovementONNX *instancePtr;

    NNMovementRequestHandle *GetHandle();

    virtual void OnBeginPlayLoadModels() override;
    virtual void OnBeginPlaySetupHandleStructure() override;

    ONNXModel *FindSelectedModel() override;

    FString modelName();

    bool logConnection = false;

public:
    static ANNMovementONNX *ApiInstance();
    static void MakeMovementONNXInstance(UWorld *World);

    ANNMovementONNX();



protected:
    
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    


public:
    //predicts a node if task not qeued
    virtual void PredictTrajectories(
        INNMovementInterface *interfaceNotify,
        AActor *actor
    ) override;

    
};