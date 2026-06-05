#pragma once

#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"
#include "PolygonPlugin/Public/Polygons/MergedPolygonRaytracable/MeshedPolygonRaytracable.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/Container/Trajectory.h"


#include "StoragePlugin/Storage/ImageData/Image/Image.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Color/MeshedPolygonColorAttributes.h"



class FVisionCone;

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
    void AppendResultMapAsFloat(TArray<uint8> &buffer) const;
    
    int ResultGridSizeBytes();

    //deprecated
    void EmbedEnemyPositions(const TArray<FVector> &enemies);

    //new
    void EmbedEnemyVision(const TArray<FVisionCone *> &cones);

    // ---- REQUEST TO NN SIMPLE ACCESS ----
    bool PrepareAppendRequestBinary(TArray<uint8> &buffer);
    bool PrepareRequestAndResultBatchBinary(TArray<uint8> &buffer);
    // ---- REQUEST TO NN SIMPLE ACCESS ----
    

    // ---- Paste result from nn ----
    void GenerateMapFromPredicitontBytes(const TArray<uint8> &buffer);

    
    void ColoredHeatMap(
        Image &image,
        FColor colorMin,
        FColor colorMax,
        FColor colorPolygonFlagged,
        FColor colorViewGrid,
        FColor colorTrjacetory,
        FColor playerPosResult
    );

    void GenerateResultPositions(
        TArray<FVector> &positions
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

private:
    bool TimeGridIsValid() const;
    /*
    rasterize
    to index buffer
    time.
    */

    void OverrideTime(Trajectory &trajectory);
    void OverrideTime(int i, int j, float time);
    void OverrideTimeGaussian(int i, int j, float time, int size, float sigma);

    void CreateOrClearTrajectoryGrid();
    TArray<TArray<float>> timeGrid;

    void AppendFloatMapToBuffer(
        TArray<uint8> &buffer,
        const TArray<TArray<float>> &someMap
    ) const;



    TArray<TArray<float>> resultGrid;
    bool ResultGridIsValid() const;
    void CreateOrClearResultGrid();
    void ClearResultGrid();

    void GaussianResultGrid(int x, int y, int size, float sigma);
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



    float Gaussian(
        int i, 
        int j, 
        int xCenter, 
        int yCenter, 
        float twoSigma2
    );


    // --- new ---
    TArray<TArray<float>> enemyPositions;
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

    /// ---- heatmap prediction ----

    TArray<TArray<float>> heatMap;
    void NormalizeHeatMap();
    void NormalizeHeatMapThroshold(float threshold);
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