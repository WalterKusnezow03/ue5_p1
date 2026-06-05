#include "MeshedPolygonColorAttributes.h"


FMeshedPolygonColorAttributes::FMeshedPolygonColorAttributes(){

}

FMeshedPolygonColorAttributes::~FMeshedPolygonColorAttributes(){

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