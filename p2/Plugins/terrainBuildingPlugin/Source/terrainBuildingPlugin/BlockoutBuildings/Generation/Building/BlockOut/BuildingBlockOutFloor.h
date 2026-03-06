#pragma once

#include "CoreMinimal.h"
#include "terrainBuildingPlugin/BlockoutBuildings/Generation/Building/Block/BuildingBlockBase.h"

class Image;
struct FBuildingColorMapData;

class TERRAINBUILDINGPLUGIN_API BuildingBlockOutFloor
{

public:
    void CreateFrom(
        Image &image, 
        const FBuildingColorMapData &colorMap, 
        float sizePerQuad
    );

    //offset z coordinate of all pivots for mesh generation
    void OffsetHeigth(float upByScale);

private:
    void Clear();
    void CreateGrid(Image &image);
    void CreateGrid(int x, int y);

    void UpdateBlockoutFromColors(Image &image, const FBuildingColorMapData &colorMap);
    void UpdateBlockoutFromColors(
        const TArray<TArray<FColor>> &data,
        const FBuildingColorMapData &colorMap
    );

    void UpdateBuildingBlock(
        int i, 
        int j,
        const FColor &color,
        const FBuildingColorMapData &colorMap
    );

    void UpdateBlockoutPivotsAndArea(float sizePerQuad);
    void UpdateBlockoutDesiredRotations();
    void UpdateBlockoutDesiredRotationForBlock(int x, int y);

    //rotation helper
    BuildingBlockBase &LeftOf(int x, int y);
    BuildingBlockBase &BottomOf(int x, int y);
    BuildingBlockBase &TopOf(int x, int y);
    BuildingBlockBase &RightOf(int x, int y);
    // rotation helper

    bool IndexValid(int x, int y);
    BuildingBlockBase &Find(int x, int y);
    BuildingBlockBase none;
    TArray<TArray<BuildingBlockBase>> blockGrid;
};
