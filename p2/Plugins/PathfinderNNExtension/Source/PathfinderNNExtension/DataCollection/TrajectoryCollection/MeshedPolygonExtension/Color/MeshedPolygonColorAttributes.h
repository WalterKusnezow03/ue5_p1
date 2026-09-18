#pragma once

#include "CoreMinimal.h"

class PATHFINDERNNEXTENSION_API FMeshedPolygonColorAttributes {

public:
    FMeshedPolygonColorAttributes();
    ~FMeshedPolygonColorAttributes();

    FMeshedPolygonColorAttributes(
        FColor colorMinHeatIn,
        FColor colorMaxHeatIn,
        FColor colorPolygonFlaggedIn,
        FColor colorViewGridIn,
        FColor colorTrjacetoryIn,
        FColor playerPosResultIn
    );

    void SetColors(
        FColor colorMinHeatIn,
        FColor colorMaxHeatIn,
        FColor colorPolygonFlaggedIn,
        FColor colorViewGridIn,
        FColor colorTrjacetoryIn,
        FColor playerPosResultIn
    );

    const FColor &ColorMinHeat();
    const FColor &ColorMaxHeat();
    const FColor &ColorPolygon();
    const FColor &ColorView();
    const FColor &ColorTrajectory();
    const FColor &ColorPlayerResult();

private:


    FColor colorMinHeat;
    FColor colorMaxHeat;
    FColor colorPolygonFlagged;
    FColor colorViewGrid;
    FColor colorTrjacetory;
    FColor playerPosResult;



};