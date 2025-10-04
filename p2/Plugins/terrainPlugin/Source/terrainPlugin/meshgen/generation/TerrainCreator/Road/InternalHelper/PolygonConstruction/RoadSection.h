#pragma once

#include "CoreMinimal.h"

/// @brief stores a road section by index start,end and road id.
class TERRAINPLUGIN_API FRoadSection {

public:
    FRoadSection();
    FRoadSection(int roadIdIn, int startIndexIn, int endIndexIn);

    ~FRoadSection();
    FRoadSection(const FRoadSection &other);
    FRoadSection &operator=(const FRoadSection &other);

    void Setup(int roadIdIn, int startIndexIn, int endIndexIn);

private:
    int roadId = 0;
    int startIndex = 0;
    int endIndex = 0;
};