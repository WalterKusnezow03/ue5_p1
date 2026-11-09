#pragma once

#include "CoreMinimal.h"
#include "PathfinderNNExtension/DataCollection/PolygonCollection/NNPathFinderProxy.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/ActorTrajectoryTracker.h"
#include "StoragePlugin/Storage/ImageData/Image/Image.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Base/EPolygonSampleType.h"

#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/MeshedPolygonTrajectoryLayered.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/MeshedPolygonTrajectoryRayModel.h"
#include "NNCommunicationPlugin/GameNN/ModelInput/FONNXModelInput.h"

#include "NNCommunicationPlugin/DataCollection/Task/PredictionTaskBase.h"

class FNNRequestPackage;
class FMeshedPolygonColorAttributes;

/// Will store task information about the tracked actor
/// and 
/// store the map once the player reappears in the
/// map, with a single pixel drawn
/// ---- will enable asynchronus prediction ----
class PATHFINDERNNEXTENSION_API PredictionTask : public PredictionTaskBase{

private:
    float radiusMeter = 100.0f; //stays the same! Do not change!
    //maybe 50

public:
    void Reset() override;

    //is reusable.
    void Setup(ActorTrajectoryTracker *trackedActor); //needed for visibilty check

    void PrepareRequestBinary(TArray<uint8> &bytes) override; //binary for shared mem python nn
    void PrepareRequestBinary(FONNXModelInput &model) override; //onnx input for ONNX Model

    //returns true if result is ready for NN Backward pass!
    bool TickVisiblityCheckAndPrepareGroundTruthBinary(TArray<uint8> &resultbytes) override;

    bool IsValid() override;

    bool IsSameActor(AActor *actorCheck);

    

    int ResultDataSizeBytes() override;

    // heat map generation from result
    void ProcessFromPredictionBytes(const TArray<uint8> &buffer) override;

    //for onnx handler
    void ProcessFromPredictionFloats(const TArray<float> &buffer) override;

    void ColoredHeatMapWithTemporaryPlayer(
        Image &image,
        FMeshedPolygonColorAttributes &attributes
    );
    void ColoredHeatMap(
        Image &image,
        FMeshedPolygonColorAttributes &attributes,
        bool tmpPlayerPos = false
    );
    void ColoredLayersMap(
        TArray<Image> &images,
        FMeshedPolygonColorAttributes &attributes
    );

    void GenerateAndNotifyResultData(FNNRequestPackage *queue) override;
    

    void EndSave() override {
        polygonProxy.EndSave();
    }


    // --- erstmal so ---
    //embed enemy positions who
    //are listening for the given actor


    void EmbedEnemyPositionsAndVision(const TArray<FVisionCone*> &enemies);
    void EmbedRequestData(FNNRequestPackage &queue) override;

    //todo: set type for prediction task / can be swicthed
    //todo: get type of prediction task

    ///@brief returns the sample data (the Trajectory Layered Polygon data sample)
    SampleInterface *GetSampleData() override;

private:
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