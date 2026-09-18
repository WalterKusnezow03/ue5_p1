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

const FColor & FMeshedPolygonColorAttributes::ColorMinHeat(){
    return colorMinHeat;
}
const FColor & FMeshedPolygonColorAttributes::ColorMaxHeat(){
    return colorMaxHeat;
}
const FColor & FMeshedPolygonColorAttributes::ColorPolygon(){
    return colorPolygonFlagged;
}
const FColor & FMeshedPolygonColorAttributes::ColorView(){
    return colorViewGrid;
}
const FColor & FMeshedPolygonColorAttributes::ColorTrajectory(){
    return colorTrjacetory;
}
const FColor & FMeshedPolygonColorAttributes::ColorPlayerResult(){
    return playerPosResult;
}