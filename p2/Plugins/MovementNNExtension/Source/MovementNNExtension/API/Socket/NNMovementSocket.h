#pragma once 

#include "CoreMinimal.h"

#include "NNCommunicationPlugin/API/Socket/NNCommunicationSocket.h"

#include "MovementNNExtension/API/Handle/NNMovementRequestHandle.h"
#include "MovementNNExtension/API/SocketInterface/NNMovementRequestHandleInterface.h"

#include "NNMovementSocket.generated.h"

/// @brief Python socket with Shared Memory Support for Python Script!
/// ---- NN SOCKET BASE WITH SHARED MEMORY SUPPORT----
UCLASS()
class MOVEMENTNNEXTENSION_API ANNMovementSocket : 
public ANNCommunicationSocket,
public INNMovementRequestHandleInterface
{
    GENERATED_BODY()


protected:
    static ANNMovementSocket *movementIntancePtr;

    //processes the requests
    NNMovementRequestHandle *GetHandle();

    bool logConnection = false;

public:
    static void MakeNNMovementSocketInstance(UWorld *world);

    static ANNMovementSocket *NNMovementinstance(){
        if(ANNMovementSocket *casted = Cast<ANNMovementSocket>(movementIntancePtr)){
            return casted;
        }
        return nullptr;
    }
    
    // -- IPathFinderNNRequestHandleInterface overrides --
    //predicts a node if task not qeued
    virtual void PredictTrajectories(
        INNMovementInterface *interfaceNotify,
        AActor *actor
    ) override;
    

    

protected:

    //using ANNSocket::WriteData;

    virtual void TickSocketConnected(float deltatime) override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void OnBeginPlaySetupHandleStructure() override;
    virtual void OnBeginPlaySetupFrameNames() override;
    virtual void OnBeginPlayLaunchPython() override;
};