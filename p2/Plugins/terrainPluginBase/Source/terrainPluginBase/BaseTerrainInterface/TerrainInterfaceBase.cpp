#include "TerrainInterfaceBase.h"

float TerrainInterfaceBase::getHeightFor(const FVector &position){
    return 0.0f;
}

bool TerrainInterfaceBase::getHeightForInBound(const FVector &position, float &outHeight){
    return 0.0f;
}


std::pair<int, int> TerrainInterfaceBase::Index2DFromWorldPosition(
    const FVector &worldPos
){
    std::pair<int, int> pair(0, 0);
    return pair;
}

bool TerrainInterfaceBase::ChunkPositionFromIndexPair(FVector &outPos, const std::pair<int, int> &index){
    return false;
}

bool TerrainInterfaceBase::IsInBound(const FVector &position){
    return true;
}