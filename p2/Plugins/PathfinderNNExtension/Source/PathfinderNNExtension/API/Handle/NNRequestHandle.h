#pragma once
#include "CoreMinimal.h"
#include "NNRequestHandleTickData.h"

#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/NActorTrajectoryTracker.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/MeshedPolygonTrajectoryLayered.h"
#include "PathfinderNNExtension/DataCollection/Task/PredictionTask.h"

#include "PathfinderNNExtension/Request/FPathFinderNNRequestPackage.h"
#include "PathfinderNNExtension/Request/FPathFinderNNRequestQueue.h"
#include "PathfinderNNExtension/Interface/PathfinderNNInterface.h"
#include "PathfinderNNExtension/Storage/FPathFinderNNSampleSet.h"

#include "StoragePlugin/Storage/ImageData/Image/Image.h"


//Base Handle for requests: register actors / prepare data
//and process prediction data
class PATHFINDERNNEXTENSION_API NNRequestHandle {

public:
    void BeginPlay();
    void Tick(FNNRequestHandleTickData &tickData);

    void FlagVisible(AActor *actor);
    void PredictNode(
        IPathfinderNNInterface *interfaceNotify,
        AActor *actor
    );

    void EndPlay();

    //only call from ONNX NN PathFinder ! - never anywhere else!
    void ReadDataResultImmidiate(TArray<float> &data);

    void EnableHeatMapSaveOnEnd(bool flag);
    bool HeatMapSaveOnEndEnabled();

private:
    FPathFinderNNSampleSet batchTask;
    FPathFinderNNRequestQueue requests;
    NActorTrajectoryTracker actorTracker;
    PredictionTask task;
    void TickTask();

    void LoadBatchIfNotDoneYet(FNNRequestHandleTickData &tickData);
    void PredictNextTask(FNNRequestHandleTickData &tickData);
    void PredictNode(
        FPathFinderNNRequestPackage *package,
        FNNRequestHandleTickData &tickData
    );
    void TickTask(FNNRequestHandleTickData &tickData);
    void TickReadDataResult(FNNRequestHandleTickData &tickData);
    
    void ReadDataResult(TArray<uint8> &bufferPrediction);

    bool saveHeatMapsEnabled = false; //is set default to false: no heatmaps output by default
    TArray<Image> heatMaps;
    void SaveHeatMapsOnEndPlay();

    bool TaskCompleted();

    void GenerateAndNotifyResultPositionsForRequestQueue();
    void GenerateResultImage();
};