#pragma once

#include "CoreMinimal.h"
#include "GameCore/util/FVectorTouple.h"
#include "AssetEnumCollection/assetEnums/ETerrainType.h"

/// @brief set up package provided by terrain creator: chunk setup inside chunk parser
class TERRAINPLUGIN_API TerrainChunkSetup{

public:
    TerrainChunkSetup(
        std::vector<std::vector<FVector>> &refIn,
        ETerrainType typeIn,
        bool createOutpostIn,
        FVector &outpostLocationIn,
        bool hasTrees
    );

    TerrainChunkSetup(TerrainChunkSetup &other);
    TerrainChunkSetup &operator=(TerrainChunkSetup &other);
    ~TerrainChunkSetup();

    std::vector<std::vector<FVector>> &mapReference();

    bool createTrees();
    ETerrainType getTerrainType();
    float treeDensitySkalar();

    //outpost creating needed
    bool OutPostFlagged();

    

    void createOutPostIfFlagged(UWorld *world);

    TArray<FVectorTouple> &freeFoliagePositionsRef();

private:
    float scaleUpFractionByLeftOverValidPositions();


    void setMapReference(std::vector<std::vector<FVector>> &refIn);
    std::vector<std::vector<FVector>> *map2D = nullptr;
    std::vector<std::vector<FVector>> none;

    TArray<FVectorTouple> freeFoliagePositions;

    ETerrainType savedTerrainType = ETerrainType::ETropical;
    bool createOutpost = false;
    bool blockTrees = false;
    

    FVector outpostLocation;
};