#pragma once

#include "CoreMinimal.h"
#include "PathfinderNNExtension/GameNN/ModelInput/FONNXModelInput.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Base/EPolygonSampleType.h"

//data to share between a NN Manager / Python / ONNX 
//and request processing from the NNRequestHandle.
struct PATHFINDERNNEXTENSION_API FNNRequestHandleTickData {

    // ---- IN DATA ----
    float deltatime = 1.0f;

    //result data arrival -> must be written from outside to processed in request handle
    TArray<uint8> predictionData;
    bool bHasPredictionData = false;

    EPolygonSampleType sampletype = EPolygonSampleType::none;
    // ---- IN DATA ----

    // ---- OUT DATA ----
    //ground truth arrival
    TArray<uint8> groundTruthBinaryOut;
    bool bGroundTruthBinaryOutChanged = false;

    //new requestdata
    TArray<uint8> requestDataOut;
    bool bRequestBinaryOutChanged = false;
    int expectedResultBytes = 0;
    //new requestdata (onnx input)
    bool useOnnxInput = false;
    FONNXModelInput modelInput;


    //new batch train data
    bool bBatchBinaryDataNeeded = false; //-> IN DATA
    TArray<uint8> batchDataOut;
    bool bBatchBinaryOutChanged = false;
    // ---- OUT DATA ----


    void Reset(){
        bGroundTruthBinaryOutChanged = false;
        bHasPredictionData = false;
        bRequestBinaryOutChanged = false;
        bBatchBinaryDataNeeded = false;
        bBatchBinaryOutChanged = false;

        predictionData.Empty();
        groundTruthBinaryOut.Empty();
        requestDataOut.Empty();
        batchDataOut.Empty();
    }
};