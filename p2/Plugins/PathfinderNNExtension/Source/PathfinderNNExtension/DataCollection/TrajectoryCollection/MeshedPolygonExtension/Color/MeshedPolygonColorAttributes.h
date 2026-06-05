#pragma once

#include "CoreMinimal.h"

class PATHFINDERNNEXTENSION_API FMeshedPolygonColorAttributes {

public:
    FMeshedPolygonColorAttributes();
    ~FMeshedPolygonColorAttributes();

    void SetColors(
        FColor colorMinHeatIn,
        FColor colorMaxHeatIn,
        FColor colorPolygonFlaggedIn,
        FColor colorViewGridIn,
        FColor colorTrjacetoryIn,
        FColor playerPosResultIn
    );




private:


    FColor colorMinHeat;
    FColor colorMaxHeat;
    FColor colorPolygonFlagged;
    FColor colorViewGrid;
    FColor colorTrjacetory;
    FColor playerPosResult;



};