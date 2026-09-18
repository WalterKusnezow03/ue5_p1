#pragma once

#include "CoreMinimal.h"
#include "PathfinderNNExtension/DataCollection/PolygonCollection/NNPathFinderProxy.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/ActorTrajectoryTracker.h"
#include "StoragePlugin/Storage/ImageData/Image/Image.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Base/EPolygonSampleType.h"

#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/MeshedPolygonTrajectoryLayered.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/MeshedPolygonTrajectoryRayModel.h"
#include "PathfinderNNExtension/GameNN/ModelInput/FONNXModelInput.h"

class FPathFinderNNRequestPackage;
class FMeshedPolygonColorAttributes;

/// Will store task information about the tracked actor
/// and 
/// store the map once the player reappears in the
/// map, with a single pixel drawn
/// ---- will enable asynchronus prediction ----
class PATHFINDERNNEXTENSION_API PredictionTask {

private:
    float radiusMeter = 100.0f; //stays the same! Do not change!
    //maybe 50

public:
    void Reset();

    //is reusable.
    void Setup(ActorTrajectoryTracker *trackedActor); //needed for visibilty check

    void PrepareRequestBinary(TArray<uint8> &bytes); //binary for shared mem python nn
    void PrepareRequestBinary(FONNXModelInput &model); //onnx input for ONNX Model

    //returns true if result is ready for NN Backward pass!
    bool TickVisiblityCheckAndPrepareGroundTruthBinary(TArray<uint8> &resultbytes);

    bool IsValid();

    bool IsSameActor(AActor *actorCheck);

    

    bool WaitingForGroundTruth();
    bool TaskCompleted();

    int ResultDataSizeBytes();

    // heat map generation from result
    void ProcessFromPredictionBytes(const TArray<uint8> &buffer);

    //for onnx handler
    void ProcessFromPredictionFloats(const TArray<float> &buffer);

    void ColoredHeatMap(
        Image &image,
        FMeshedPolygonColorAttributes &attributes
    );
    void ColoredLayersMap(
        TArray<Image> &images,
        FMeshedPolygonColorAttributes &attributes
    );

    void GenerateAndNotifyResultPositions(FPathFinderNNRequestPackage *queue);
    

    void EndSave(){
        polygonProxy.EndSave();
    }


    // --- erstmal so ---
    //embed enemy positions who
    //are listening for the given actor


    void EmbedEnemyPositionsAndVision(const TArray<FVisionCone*> &enemies);
    void EmbedEnemyPositionsAndVision(FPathFinderNNRequestPackage &queue);


    //todo: set type for prediction task / can be swicthed
    //todo: get type of prediction task


private:
    bool taskStarted = false;
    bool taskCompleted = true;

    void StartTaskBeforePrepareBinary(FMeshedPolygonTrajectoryLayeredInterface &polygonDataCache);

    void PrepareRequestMap(FMeshedPolygonTrajectoryLayeredInterface &polygonData);
    void PrepareResultMap();

    NNPathFinderProxy polygonProxy;

    //targeted actor
    ActorTrajectoryTracker *trackedActorPtr = nullptr;

    FVector locationOfRequest;

    

    //Get Interface reference !
public:
    FMeshedPolygonTrajectoryLayeredInterface &GetPolygonData(); //by set type
    void UpdateSampleType(EPolygonSampleType type);
    EPolygonSampleType GetSampleType();

private:

    FMeshedPolygonTrajectoryLayeredInterface &GetPolygonData(EPolygonSampleType type); //by set type
    EPolygonSampleType sampleType = EPolygonSampleType::EMeshedPolygonTrajectoryLayered;

    FMeshedPolygonTrajectoryLayered polygonDataUnetModel; //switch to ptr
    FMeshedPolygonTrajectoryRayModel polygonRayModel;


    

    //Unet input as 144 144, do not change, first check py script!!
    void ResizeToNNSize(FMeshedPolygonTrajectoryLayered &polygonData);
    void Resize(FMeshedPolygonTrajectoryLayered &polygonData, int x, int y); 
};