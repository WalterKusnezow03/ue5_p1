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

    FColor ColorMinHeat();
    FColor ColorMaxHeat();
    FColor ColorPolygon();
    FColor ColorView();
    FColor ColorTrajectory();
    FColor ColorPlayerResult();

private:


    FColor colorMinHeat;
    FColor colorMaxHeat;
    FColor colorPolygonFlagged;
    FColor colorViewGrid;
    FColor colorTrjacetory;
    FColor playerPosResult;



};