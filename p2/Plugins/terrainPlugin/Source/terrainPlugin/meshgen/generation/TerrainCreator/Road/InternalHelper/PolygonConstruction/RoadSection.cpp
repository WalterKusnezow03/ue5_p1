#include "RoadSection.h"

#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/InternalHelper/RoadDataBase/RoadData.h"

FRoadSection::FRoadSection(){

}

FRoadSection::FRoadSection(int roadIdIn, int startIndexIn, int endIndexIn){
    Setup(roadIdIn, startIndexIn, endIndexIn);
}

FRoadSection::~FRoadSection(){

}

FRoadSection::FRoadSection(const FRoadSection &other){
    if(this != &other){
        *this = other;
    }
}
FRoadSection &FRoadSection::operator=(const FRoadSection &other){
    if(this != &other){
        roadId = other.roadId;
        startIndex = other.startIndex;
        endIndex = other.endIndex;
    }
    return *this;
}

void FRoadSection::Setup(int roadIdIn, int startIndexIn, int endIndexIn){
    roadId = roadIdIn;
    startIndex = startIndexIn;
    endIndex = endIndexIn;
}




//extraction of road parts
int FRoadSection::RoadId(){
    return roadId;
}




void FRoadSection::extractPart(RoadData &roadData, TArray<FVector2D> &outAppend){
    TArray<FVector2D> roadRaw = roadData.getCurveAsTArray();
    extractPart(roadRaw, outAppend);
}

void FRoadSection::extractPart(const TArray<FVector2D> &road, TArray<FVector2D> &outAppend){
    
    int32 start = std::max(0, startIndex);
    int32 end = std::min(endIndex, road.Num());
    int32 delta = end - start;
    if(delta <= 0){
        return;
    }
    
    if(startIndex <= endIndex){
        //upwards iteration
        int32 offset = outAppend.Num();
        outAppend.SetNumUninitialized(offset + delta);
        for (int32 i = start, j = 0; i < end; i++, j++)
        {
            outAppend[offset + j] = road[i];
        }
    }else{
        //downwards iteration
        int32 offset = outAppend.Num();
        outAppend.SetNumUninitialized(offset + delta);
        for (int32 i = end, j = 0; i >= start; i--, j++)
        {
            if(i >= 0 && i < road.Num()){
                outAppend[offset + j] = road[i];
            }
            
        }
    }
}