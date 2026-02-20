#include "TerrainInterfaceBase.h"

float TerrainInterfaceBase::getHeightFor(FVector &position){
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