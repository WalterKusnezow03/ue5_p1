#pragma once

#include "CoreMinimal.h"

#include "NNCommunicationPlugin/GameNN/ModelInput/FONNXModelInput.h"
#include "NNCommunicationPlugin/Communication/Sample/Interface/SampleInterface.h"

class FNNRequestPackage;


/// Will store task information about the tracked actor
/// and 
/// store the map once the player reappears in the
/// map, with a single pixel drawn
/// ---- will enable asynchronus prediction ----
class NNCOMMUNICATIONPLUGIN_API PredictionTaskBase {

private:
    float radiusMeter = 100.0f; //stays the same! Do not change!
    //maybe 50

public:
    virtual void Reset();

    /** preperation of the request binary for the nn */
    virtual void PrepareRequestBinary(TArray<uint8> &bytes); //binary for shared mem python nn
    virtual void PrepareRequestBinary(FONNXModelInput &model); //onnx input for ONNX Model

    //returns true if result is ready for NN Backward pass!
    /** Will return whether a ground truth state is ready, which is written to the result bytes */
    virtual bool TickVisiblityCheckAndPrepareGroundTruthBinary(TArray<uint8> &resultbytes);

    virtual bool IsValid();

    

    virtual bool WaitingForGroundTruth();
    virtual bool TaskCompleted();

    virtual int ResultDataSizeBytes();

    // heat map generation from result
    virtual void ProcessFromPredictionBytes(const TArray<uint8> &buffer);

    //for onnx handler
    virtual void ProcessFromPredictionFloats(const TArray<float> &buffer);

    
    /** once a prediction has arrived and is post proccessed, the requester can be notified */
    virtual void GenerateAndNotifyResultData(FNNRequestPackage *queue);
    

    virtual void EndSave(){
        
    }
    
    /** embed request data into the data preperation structure */
    virtual void EmbedRequestData(FNNRequestPackage &queue);

    /** mark finished after result is extracted - which can be a post processed prediction */
    virtual void MarkTaskFinished();

    /** 
     * has to be overriden! 
     * Allows to get the sample which is currently made from
     * request (x) and Ground Truth (y)
     * - is supposed to be added to the Batch File
    */
    virtual SampleInterface *GetSampleData(){
        return nullptr; 
    }

protected:
    

protected:
    bool taskStarted = false;
    bool taskCompleted = true;

    void UpdateTaskStartedFlags();
};