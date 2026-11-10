#pragma once
#include "CoreMinimal.h"
#include "NNRequestHandleTickDataBase.h"





#include "NNCommunicationPlugin/Request/FNNRequestQueue.h"
#include "NNCommunicationPlugin/Request/FNNRequestPackage.h"


#include "NNCommunicationPlugin/API/NotifyInterface/BaseNNInterface.h"
#include "NNCommunicationPlugin/DataCollection/Task/PredictionTaskBase.h"

#include "NNCommunicationPlugin/Storage/FNNSampleSetBase.h"

//Base Handle for requests: register actors / prepare data
//and process prediction data
// Request processing is temporarily limited to queue and task plumbing.
// Note: this is a framework class - derive it and implement the missing classes and behaviour.
class NNCOMMUNICATIONPLUGIN_API NNRequestHandleBase {

    // FNNSampleSetBase *batchTask = nullptr;

public:
    /// handle whether the batch should be loaded,
    /// and whether samples should be appended 
    /// (differentiate between compiled game onnx and python in editor usuage (training))
    void BeforeBeginPlaySetSampleUsuage(
        bool inLoadSamples,
        bool inSaveSamples
    );

    virtual void BeginPlay();
    virtual void Tick(FNNRequestHandleTickDataBase &tickData);

    virtual void PredictData(
        IBaseNNInterface *interfaceNotify,
        FString nameTaskTarget,
        UObject *targetObject = nullptr
    );

    virtual void EndPlay();

    //only call from ONNX NN PathFinder ! - never anywhere else!
    void ReadDataResultImmidiate(TArray<float> &data);


protected:
    virtual void OnBeginPlaySetupTask();
    virtual void OnBeginPlaySetupBatchTask();

    virtual void OnEndPlayBatchTaskSave();

    virtual void OnTick() {};


    FNNRequestQueue requests;

    //can be replaced by a subclass for custom behaivour on data input generation
    PredictionTaskBase *task = nullptr;

    //can be replaced by a subclass for custom samples (use TNNSampleSet)
    //is also a ptr which can be swapped.
    FNNSampleSetBase *batchTask = nullptr;
    virtual FNNSampleSetBase *GetBatchTask();

    void LoadBatchIfNotDoneYet(FNNRequestHandleTickDataBase &tickData);
    void PredictNextTask(FNNRequestHandleTickDataBase &tickData);
    void PredictData(
        FNNRequestPackage *package,
        FNNRequestHandleTickDataBase &tickData
    );
    bool PredictDataAllowed();


    virtual bool PrepareTaskFor(FNNRequestPackage *package);
    void UpdateRequestBinaryFor(FNNRequestHandleTickDataBase &tickData);

    void TickTask(FNNRequestHandleTickDataBase &tickData);
    virtual void OnTickTaskWaitForGroundTruth();
    virtual void OnTickTaskArrivedGroundTruth();
    virtual void OnTickTaskArrivedGroundTruthValid();
    void TickReadDataResult(FNNRequestHandleTickDataBase &tickData);

    virtual void OnPredictionArrive();

    void ReadDataResult(TArray<uint8> &bufferPrediction);


    

    bool TaskCompleted();

    //PREDICTION PROCESS
    virtual void GenerateAndNotifyResultPositionsForRequestQueue();
    bool alreadyReadPredictionData = false;

    //Save Flags
    bool saveSamples = true;
    bool loadSamples = true;
};