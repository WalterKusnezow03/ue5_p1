#pragma once

#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/Container/Trajectory.h"


//encode trajectory layer
class PATHFINDERNNEXTENSION_API FMeshedPolygonTrajectoryLayered : public FMeshedPolygon {

public:
    //clears corrosponding grid, object is reusable!
    void EmbedTrajectories(TArray<Trajectory> &trajectories);
    void EmbedResultPosition(FVector &position);

    void AppendFlagMapAsFloat(TArray<uint8> &buffer) const;
    void AppendTimeMap(TArray<uint8> &buffer) const;
    void AppendResultMapAsFloat(TArray<uint8> &buffer) const;


private:
    bool TimeGridIsValid() const;
    /*
    rasterize
    to index buffer
    time.
    */

    void OverrideTime(Trajectory &trajectory);
    void OverrideTime(int i, int j, float time);

    void CreateOrClearTrajectoryGrid();
    void ClearTimeGrid();
    TArray<TArray<float>> timeGrid;

    void AppendFloatMapToBuffer(
        TArray<uint8> &buffer,
        const TArray<TArray<float>> &someMap
    ) const;



    TArray<TArray<uint8>> resultGrid;
    bool ResultGridIsValid() const;
    void CreateOrClearResultGrid();
    void ClearResultGrid();



    //converts a uint 8 flag grid map to float map
    //expects inmap float to be empty
    void Uint8FlagMapToFloat(const TArray<TArray<uint8>> &refMap, TArray<TArray<float>> &converted) const;
};