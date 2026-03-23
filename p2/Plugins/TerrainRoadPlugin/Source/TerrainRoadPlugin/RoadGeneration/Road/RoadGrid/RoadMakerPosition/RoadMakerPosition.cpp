#include "RoadMakerPosition.h"

FRoadMakerPosition::FRoadMakerPosition(){

}
FRoadMakerPosition::~FRoadMakerPosition(){
    
}

FRoadMakerPosition::FRoadMakerPosition(const FRoadMakerPosition &other){
    if(this != &other){
        *this = other;
    }
}
FRoadMakerPosition &FRoadMakerPosition::operator=(const FRoadMakerPosition &other){
    if(this != &other){
        position = other.position;
    }
    return *this;
}

FRoadMakerPosition::FRoadMakerPosition(
    const FVector &pos
){
    position = pos;
    isInsideTerrain = true;
}

void FRoadMakerPosition::UpdateHeightAndMarkInBoundStatus(TerrainInterfaceBase *creator, float offset){
    if(creator){
        float heightOut = 0.0f;
        isInsideTerrain = creator->getHeightForInBound(position, heightOut);
        if (isInsideTerrain)
        {
            position.Z = heightOut + offset;
        }else{
            position.Z = 0.0;
        }
    }
}

