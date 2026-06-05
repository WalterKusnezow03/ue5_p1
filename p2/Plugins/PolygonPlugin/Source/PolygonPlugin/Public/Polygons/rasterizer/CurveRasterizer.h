#pragma once

#include "CoreMinimal.h"


class POLYGONPLUGIN_API CurveRasterizer {
public:
    CurveRasterizer();
    ~CurveRasterizer();


    // ---- filled with brensenham ----
    void RasterizeVerteciesIntBrensenham(
        TArray<FIntPoint> &polygon
    );



    // ---- to mod coordinates ----
    void RasterizeVerteciesInt(
        TArray<FIntPoint> &polygon
    );

    void RasterizeVerteciesToInt(
        TArray<FIntPoint> &polygon,
        int widthOfInsideStep
    );

    // --- rasterize to step raw ---
    void RasterizeVerteciesTo(
        TArray<FVector> &polygon,
        float widthOfStep
    );



    


private:

    FVector ToModCoordinate(FVector &pos, int mod, int dir);

    FVector& ChooseRightOffCoordinateRelativeTo(
        FVector &modLower,
        FVector &modHigher,
        FVector &normal,
        FVector &relativePoint
    );

    void Convert(
        const TArray<FIntPoint> &raw,
        TArray<FVector> &outData
    );
    void Convert(
        const TArray<FVector> &raw,
        TArray<FIntPoint> &outData
    );
};