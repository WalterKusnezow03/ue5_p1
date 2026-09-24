#pragma once

#include "CoreMinimal.h"


/// @brief shape to be fitted inside the meshed surface grid 
/// polygon bounds.
/// in out task: in verts, out pivot and rotator making a transform M = T * R <-- lese richtung --
class TERRAINROADPLUGIN_API ShapeFitTask {

public:
    void Setup(int x, int y);

    void SetPivot(const FVector &pivotIn);
    void UpdatePivotPickLowestZ(const TArray<FVector> &array);
    void UpdateRotation(const FRotator &in);

    const FVector &GetPivot() const;
    const FRotator &GetRotation() const;

    FRotator &GetRotationRef();

    void GetSize(int &outX, int &outY);

private:
    //todo make verts instead
    int sizeX = 0;
    int sizeY = 0;

    bool wasPivotSetOnce = false;
    FVector pivot;
    FRotator rotator;
};