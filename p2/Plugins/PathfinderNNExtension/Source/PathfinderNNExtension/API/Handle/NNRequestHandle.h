#pragma once
#include "CoreMinimal.h"


#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/NActorTrajectoryTracker.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/MeshedPolygonTrajectoryLayered.h"
#include "PathfinderNNExtension/DataCollection/Task/PredictionTask.h"

 
  
#include "PathfinderNNExtension/Interface/PathfinderNNInterface.h"


#include "StoragePlugin/Storage/ImageData/Image/Image.h"
#include "PathfinderNNExtension/Interface/HeatMapReceiverCollection/HeatMapReceivers.h"

#include "PathfinderNNExtension/Storage/Template/TPathFinderNNSampleSet.h"

#include "NNCommunicationPlugin/API/Handle/NNRequestHandleBase.h"

//Base Handle for requests: register actors / prepare data
//and process prediction data
//will also store the selected model type by "EPolygonSampleType"
class PATHFINDERNNEXTENSION_API NNRequestHandle : public NNRequestHandleBase{

public:
    void BeginPlay() override;
    void Tick(FNNRequestHandleTickDataBase &tickData) override;
    void EndPlay() override;

    void FlagVisible(AActor *actor);

    void PredictData(
        IBaseNNInterface *interfaceNotify,
        FString nameTaskTarget,
        UObject *targetObject
    ) override;

    void EnableHeatMapSaveOnEnd(bool flag);
    bool HeatMapSaveOnEndEnabled();

    //todo: add sample type swicther
    EPolygonSampleType GetSampleType();

    void UpdateSampleType(EPolygonSampleType type);

    //widget related coloring, external call
    //also provides the pivot of the requested data in world space
    void ColorizedWidgetImage(Image &image, FVector &worldPosPivot);

    HeatMapReceivers &GetHeatMapReceivers();

protected:
    virtual void OnBeginPlaySetupTask() override;
    void OnBeginPlaySetupBatchTask() override;
    void OnTick() override;
    void OnTickTaskWaitForGroundTruth() override;
    void OnTickTaskArrivedGroundTruth() override;
    //void OnTickTaskArrivedGroundTruthValid() override;
    void OnPredictionArrive() override;
    

    void OnEndPlayBatchTaskSave() override;

    PredictionTask *GetTask();
    virtual FNNSampleSetBase *GetBatchTask() override;

    //batch data for different sample types
    TPathFinderNNSampleSet<FMeshedPolygonTrajectoryLayered> unetSamples;
    TPathFinderNNSampleSet<FMeshedPolygonTrajectoryRayModel> raymodelSamples;


protected:
    
    NActorTrajectoryTracker actorTracker;

    virtual bool PrepareTaskFor(FNNRequestPackage *package) override;

    bool saveHeatMapsEnabled = false; //is set default to false: no heatmaps output by default
    TArray<Image> heatMaps;
    void AddHeatMapSampleToStorage(Image &image);
    void SaveHeatMapsOnEndPlay();

    //interface for heatmap listening (widget)
    HeatMapReceivers heatMapReceivers;
    void NotifyHeatMapReceivers(Image &image);
    void GeneratePredictionImage();

    //widget
    void GenerateLiveImage();

    //GT PROCESS
    void GenerateResultImage();
    void GenerateResultImageChannels();
};