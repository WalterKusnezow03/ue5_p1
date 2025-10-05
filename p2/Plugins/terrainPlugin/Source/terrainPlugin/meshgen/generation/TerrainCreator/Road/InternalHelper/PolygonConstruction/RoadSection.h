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

    int RoadId();


    /// @brief extracts the part of road by start and end index in correct order and appends into the array.
    void extractPart(class RoadData &roadData, TArray<FVector2D> &outAppend);


    /// @brief extracts the part of Array (raw road) by start and end index in correct order and appends into the array.
    void extractPart(const TArray<FVector2D> &road, TArray<FVector2D> &outAppend);

private:
    int roadId = 0;
    int startIndex = 0;
    int endIndex = 0;
};