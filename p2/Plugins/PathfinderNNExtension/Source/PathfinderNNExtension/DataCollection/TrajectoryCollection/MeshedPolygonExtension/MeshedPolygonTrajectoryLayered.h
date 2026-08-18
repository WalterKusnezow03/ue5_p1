#pragma once

#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"
#include "PolygonPlugin/Public/Polygons/MergedPolygonRaytracable/MeshedPolygonRaytracable.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/Container/Trajectory.h"


#include "StoragePlugin/Storage/ImageData/Image/Image.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Color/MeshedPolygonColorAttributes.h"



class FVisionCone;
class FMeshedPolygonColorAttributes;
class IPathfinderNNInterface;

//encode trajectory layer
class PATHFINDERNNEXTENSION_API FMeshedPolygonTrajectoryLayered : public FMeshedPolygonRaytracable {

public:
    void Reset();

    bool FlagAndTimeDataValid() const;

    // ---- REQUEST TO NN ----
    //clears corrosponding grid, object is reusable!
    void EmbedTrajectories(TArray<Trajectory> &trajectories);
    void EmbedResultPosition(FVector &position);

    void AppendFlagMapAsFloat(TArray<uint8> &buffer) const;
    void AppendTimeMap(TArray<uint8> &buffer) const;
    void AppendViewMap(TArray<uint8> &buffer);
    void AppendTrajectoryConeMap(TArray<uint8> &buffer);
    void AppendResultMapAsFloat(TArray<uint8> &buffer) const;

    int ResultGridSizeBytes(); //ground truth grid size bytes

    //new
    void EmbedEnemyVision(const TArray<FVisionCone *> &cones);

    // ---- REQUEST TO NN SIMPLE ACCESS ----
    bool PrepareAppendRequestBinary(TArray<uint8> &buffer);
    bool PrepareRequestAndResultBatchBinary(TArray<uint8> &buffer);
    
    // ---- REQUEST TO NN SIMPLE ACCESS ----
    

    // ---- Paste result from nn ----
    void GenerateMapFromPredicitontBytes(const TArray<uint8> &buffer);
    void GenerateMapFromPredicitontFloats(const TArray<float> &buffer);

    void ColoredHeatMap(
        Image &image,
        FMeshedPolygonColorAttributes &attributes
    );

    void NotifyVisiblePositionsFor(
        IPathfinderNNInterface *interfaceIn,
        bool useVisiblity
    );

    //binary generation
    virtual void AppendAsBinary(
        TArray<uint8> &buffer
    ) override;

    virtual bool LoadFromBinary(
        TArray<uint8> &buffer,
        uint8 *& Ptr //reference to a pointer. Pointer by reference.
    ) override;
    //binary generation

    virtual void ResizeGrid(int x, int y) override;
    void ResizeGrid144();

private:
    void EmbedConeFromTrajectories(
        TArray<Trajectory> &trajectories
    );

    void GenerateResultPositions(
        TArray<FVector> &positions
    );

    void GenerateResultPositionsVisibleBy(
        IPathfinderNNInterface *interfaceIn,
        TArray<FVector> &outpositions
    );

    void GenerateResultPositionsVisibleBy(
        const FVector &lookFromPos,
        TArray<FVector> &outpositions
    );

    void GenerateResultPositionsVisibleBy(
        const FVector &lookFromPos,
        const TArray<FVector> &possibleSolutions,
        TArray<FVector> &outpositions
    );

    void ColoredHeatMap(
        Image &image,
        FColor colorMin,
        FColor colorMax,
        FColor colorPolygonFlagged,
        FColor colorViewGrid,
        FColor colorTrjacetory,
        FColor playerPosResult
    );




    // --- player trajectories ---
    bool TimeGridIsValid() const;
    void OverrideTime(Trajectory &trajectory);
    void OverrideTime(int i, int j, float time);
    void OverrideTimeGaussian(int i, int j, float time, int size, float sigma);

    void CreateOrClearTrajectoryGrid();
    TArray<TArray<float>> timeGrid; //trajectories from player

    void AppendFloatMapToBuffer(
        TArray<uint8> &buffer,
        const TArray<TArray<float>> &someMap
    ) const;

    // --- player trajectory prediction Cone ---
    TArray<TArray<float>> trajectoryConePrecited;
    bool TrajectoryConeGridIsValid();
    void CreateOrClearTrajectoryConeGrid();







    // --- ground truth ---

    //ground truth position . ground truth grid
    static constexpr float playerGroundTruthPeak = 1.0f;
    TArray<TArray<float>> groundTruthGrid; //ground truth grid
    bool GroundTruthGridIsValid() const;
    void CreateOrClearGroundTruthGrid();
    void ClearGroundTruthGrid();

    void GaussianGroundTruthGrid(int x, int y, int size, float sigma);
    void GaussianOnGrid(
        int x,
        int y,
        int size,
        float sigma,
        TArray<TArray<float>> &grid
    );

    void GaussianOnGrid(
        int x,
        int y,
        int size,
        float sigma,
        TArray<TArray<float>> &grid,
        float valueToScale
    );

    //converts a uint 8 flag grid map to float map
    //expects inmap float to be empty
    void Uint8FlagMapToFloat(const TArray<TArray<uint8>> &refMap, TArray<TArray<float>> &converted) const;



    


    // --- enemy positions - not used ---
    TArray<TArray<float>> enemyPositions;
    //TArray<FVector> enemyPositionsRaw; //needs to be saved to SampleSet - NO DOES NOT NEED TO BE, POST PROCESS
    bool EnemyPositionGridIsValid();
    void CreateOrClearEnemyPositionGrid();

    ///@brief checks if the given grid is the same size as the flag grid.
    template <typename T> 
    bool TGridIsValidToFlagGrid(TArray<TArray<T>> &grid){
        if(FlagGridIsValid()){ //buffer size at least one
            if(grid.Num() == flagGrid.Num()){
                if(grid[0].Num() == flagGrid[0].Num()){
                    return true;
                }
            }
        }
        return false;
    }
    void EmbedEnemyPosition(const FVector &position);

    //






    /// ---- heatmap prediction ----

    TArray<TArray<float>> heatMap;
    TArray<FVector> extractedHeatMapResults;
    void NormalizeHeatMap();
    void NormalizeHeatMapThroshold(float threshold);
    void CacheResultPositionsFromHeatMap();
    void RemoveHeatMapBorder(int sizeBorder);
    void ClusterHeatMapPositions();
    void LogHeatMap(FString prefix);
    FColor DirColor(
        const FColor &colorMin,
        const FColor &colorMax);
    FColor LerpColor(
        const FColor &colorMin,
        const FColor &colorDir,
        float scalar
    );
};