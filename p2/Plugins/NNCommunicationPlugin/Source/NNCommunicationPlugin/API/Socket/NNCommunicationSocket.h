#pragma once 

#include "CoreMinimal.h"
#include "NNCommunicationPlugin/Communication/Connection/NNSocket.h"




#include "NNCommunicationPlugin/API/Handle/NNRequestHandleBase.h"

#include "NNCommunicationPlugin/API/Handle/NNRequestHandleTickDataBase.h"

#include "NNCommunicationPlugin/API/RequestTakerInterface/NNRequestTakerInterfaceBase.h"


#include "NNCommunicationSocket.generated.h"

/// @brief Python socket with Shared Memory Support for Python Script!
/// ---- NN SOCKET BASE WITH SHARED MEMORY SUPPORT----
// Request-handler interface is temporarily disconnected while its implementation is incomplete.
UCLASS()
class NNCOMMUNICATIONPLUGIN_API ANNCommunicationSocket : 
public ANNSocket,
public INNRequestTakerInterfaceBase
{
    GENERATED_BODY()


protected:
    FString frameNameRequest = "DEFAULT_IN";
    FString frameNameResult = "DEFAULT_OUT";
    FString frameNameGroundThruth = "DEFAULT_GT";

    FString frameNameBatch = "DEFAULT_BATCH";

    FString frameNameNNType = "DEFAULT_NNTYPE";

    virtual void OnReceivePythonPrint(FString message) override;

    //processes the requests
    NNRequestHandleBase *requestHandle = nullptr;

    //in (nn prediction) and out (nn requst) data 
    FNNRequestHandleTickDataBase *requestTickData = nullptr;

    bool logConnection = false;
    bool trainingStartAllowed = false;

public:
    
    
    // Temporarily kept as a direct socket API while the request interface is disabled.
    virtual void PredictData(
        IBaseNNInterface *interfaceNotify,
        FString name,
        UObject *targetObject = nullptr
    ) override {
        if(requestHandle){
            requestHandle->PredictData(interfaceNotify, name, targetObject);
        }
    }

    
    //connection state of socket to display in widget
    virtual bool NNIsConnected() override;
    //heatmap / other widget for minimap
    

    

    virtual void SetTrainingAllowed() override;
    

protected:
    using ANNSocket::WriteData;
    void WriteDataRequest(TArray<uint8> &data, int resultBytes);
    void WriteDataGroundTruth(TArray<uint8> &buffer);
   
    //using ANNSocket::WriteData;
    //void WriteData(const TArray<uint8> &array);
    virtual void Tick(float deltatime) override;

    virtual void TickSocketConnected(float deltatime) override;
    virtual void BeginPlay() override;

    //OVERRIDE NEEDED!
    virtual void OnBeginPlaySetupHandleStructure(); //has to be overriden!
    virtual void OnBeginPlaySetupFrameNames();
    virtual void OnBeginPlayLaunchPython();

    void RequestHandleBeginPlay();

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    void OnEndPlayCleanUpHandleStructure();

private:
    bool CloseTrainSharedMemoryFrame(const FString &message);

    void ResetRequestData(float deltatime);
    void TryReadPredictionData();
    void ProcessTrainingFlag();
    void TickRequestHandle();
};