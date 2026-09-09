#pragma once

#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"
#include "PolygonPlugin/Public/Polygons/MergedPolygonRaytracable/MeshedPolygonRaytracable.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/Container/Trajectory.h"


#include "StoragePlugin/Storage/ImageData/Image/Image.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Color/MeshedPolygonColorAttributes.h"

#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Base/MeshedPolygonTrajectoryLayeredInterface.h"

class FVisionCone;
class FMeshedPolygonColorAttributes;
class IPathfinderNNInterface;


// ---- EXPERIMENTAL ----
//encodes ray model and keeps the hitpoint vector for 
//net type NetC

class PATHFINDERNNEXTENSION_API FMeshedPolygonTrajectoryRayModel : public FMeshedPolygonTrajectoryLayeredInterface {

//class PATHFINDERNNEXTENSION_API FMeshedPolygonTrajectoryRayModel : public FMeshedPolygonRaytracable {

public:
    void Reset() override;

    //todo:
    //to binary
    //embed player - multi sport raycast model across a radius?
    //inject map

    //inject enemy model

    //question: could it be derived from TrajectoryLayered Model 
    //but methods changed?
    //data could stay more or less the same,
    //trajectories are still needed
    //player pos is still needed,
    //the output is a single spot,
    //the gt is a single spot
    
    //--> debug image could be renedered the same

    ///----> ray model würde die daten dichte von 142 * 142 * 4 auf 360 * 4 + x reduzieren
    ///      also von n^3 daten auf n, sehr viel dünnerer datensatz, schneller zu trainieren
    ///      sparsamer, daten reichen auch für andere netze, z.b. verschiedene netz outputs generieren


    //hier auf vision cones einen festen strahlen count
    //aufteilen
    void EmbedEnemyVision(const TArray<FVisionCone *> &cones) override;

    //player trajectories embedding, keine player pos, soll predicted werden
    virtual void EmbedTrajectories(TArray<Trajectory> &trajectories) override;

    //player ground truth embedding
    void EmbedResultPosition(FVector &position) override;

    // ---- REQUEST TO NN SIMPLE ACCESS ----
    bool PrepareAppendRequestBinary(TArray<uint8> &buffer) override;
    bool PrepareRequestAndResultBatchBinary(TArray<uint8> &buffer) override;

    int ResultDataSizeBytes() override;

    virtual void AppendGroundTruth(TArray<uint8> &buffer) override;

    // ---- REQUEST TO NN SIMPLE ACCESS ----

    // ---- Response process ----
    virtual void ProcessFromPredictionBytes(const TArray<uint8> &buffer) override;
    virtual void ProcessFromPredictionFloats(const TArray<float> &buffer) override;

    virtual void NotifyVisiblePositionsFor(
        IPathfinderNNInterface *interfaceIn,
        bool useVisiblity
    ) override;

    // ---- Response process ----



    // ---- Storage Interface ----
    void AppendAsBinary(TArray<uint8> &buffer) override;
    bool LoadFromBinary(TArray<uint8> &buffer, uint8 *&Ptr) override;

    EPolygonSampleType GetType() override;

    

    
    float SimilarityOfSample(FMeshedPolygonTrajectoryLayeredInterface &other) override;

protected:
    void ClearTrajectoryData();
    void ClearEnemyVisionData();

    int maxRaysPlayerVision = 360;
    int maxPlayerTrajectories = 20;

    void EmbedRayModelFromTrajectories(TArray<Trajectory> &trajectories);

    void EmbedEnemyVision(FVisionCone *cone, int rays);

    void ValidateEnemyHitsBuffer();
    void ValidatePlayerHitsBuffer();
    void ValidatePlayerTrajectoryBuffer();
    void ValidateAllBuffers();

    void OverrideTime(TArray<Trajectory> &trajectories);
    void OverrideTime(Trajectory &current);
    void OverrideTime(int x, int y, float time);

private:
    //converts the buffer of (x,y) pairs to (x...x)(y..y) buffer
    void ToFloatBufferChannels(const TArray<FIntPoint> &bufferIn, TArray<float> &outBuffer);
    void ToFloatBufferChannels(const TArray<FVector> &bufferIn, TArray<float> &outBuffer);

    

    TArray<FIntPoint> playerHits;
    TArray<FIntPoint> enemyHits;
    TArray<FVector> playerTrajectories; //converted to Vector3 as (pos, time)

    FIntPoint playerGroundTruth;

    //nn output prediction
    FVector localPrediciton;
    FVector playerPrediction;

    TArray<FVector> GetPlayerResultPositions();
};