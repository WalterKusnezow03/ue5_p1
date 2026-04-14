#pragma once

#include "CoreMinimal.h"


class POLYGONPLUGIN_API CurveRasterizer {
public:
    CurveRasterizer();
    ~CurveRasterizer();
    
    void RasterizeVerteciesTo(
        TArray<FVector> &polygon,
        float widthOfInsideStep
    );


private:

    FVector ToModCoordinate(FVector &pos, int mod, int dir);

    FVector& ChooseRightOffCoordinateRelativeTo(
        FVector &modLower,
        FVector &modHigher,
        FVector &normal,
        FVector &relativePoint
    );


};