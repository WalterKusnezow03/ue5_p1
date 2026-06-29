#include "MeshedPolygonColorAttributes.h"


FMeshedPolygonColorAttributes::FMeshedPolygonColorAttributes(){

}

FMeshedPolygonColorAttributes::~FMeshedPolygonColorAttributes(){

}

FMeshedPolygonColorAttributes::FMeshedPolygonColorAttributes(
    FColor colorMinHeatIn,
    FColor colorMaxHeatIn,
    FColor colorPolygonFlaggedIn,
    FColor colorViewGridIn,
    FColor colorTrjacetoryIn,
    FColor playerPosResultIn
){
    SetColors(
        colorMinHeatIn,
        colorMaxHeatIn,
        colorPolygonFlaggedIn,
        colorViewGridIn,
        colorTrjacetoryIn,
        playerPosResultIn
    );
}

void FMeshedPolygonColorAttributes::SetColors(
    FColor colorMinHeatIn,
    FColor colorMaxHeatIn,
    FColor colorPolygonFlaggedIn,
    FColor colorViewGridIn,
    FColor colorTrjacetoryIn,
    FColor playerPosResultIn
){
    colorMinHeat = colorMinHeatIn;
    colorMaxHeat = colorMaxHeatIn;
    colorPolygonFlagged = colorPolygonFlaggedIn;
    colorViewGrid = colorViewGridIn;
    colorTrjacetory = colorTrjacetoryIn;
    playerPosResult = playerPosResultIn;
}

FColor FMeshedPolygonColorAttributes::ColorMinHeat(){
    return colorMinHeat;
}
FColor FMeshedPolygonColorAttributes::ColorMaxHeat(){
    return colorMaxHeat;
}
FColor FMeshedPolygonColorAttributes::ColorPolygon(){
    return colorPolygonFlagged;
}
FColor FMeshedPolygonColorAttributes::ColorView(){
    return colorViewGrid;
}
FColor FMeshedPolygonColorAttributes::ColorTrajectory(){
    return colorTrjacetory;
}
FColor FMeshedPolygonColorAttributes::ColorPlayerResult(){
    return playerPosResult;
}