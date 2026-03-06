#pragma once

#include "CoreMinimal.h"

#include "terrainBuildingPlugin/BlockoutBuildings/AssetLoading/AssetData/ColorMap/BlockProperty.h"





/// @brief will store building information for one square unit
/// information will be sued to generate the needed meshdata
class TERRAINBUILDINGPLUGIN_API BuildingBlockBase {

public:
    void SetProperty(FBlockProperty &propertyIn);
    FBlockProperty &GetPropertyRef();

    //set area where mesh data generation is allowed
    void SetOffsetAndQuadArea(const FVector &pivotIn, float sizeQuad);

    EBuildingTypeBlock GetTypeOfBlock() const;
    
    void UpdateRoationAndCornerFlag(
        const BuildingBlockBase &topOf,
        const BuildingBlockBase &bottomOf,
        const BuildingBlockBase &rightOf,
        const BuildingBlockBase &leftOf
    );

private:
    FBlockProperty property;
    bool CanHaveRotation() const; //rotatable block type
    bool IsInsideType() const; //is inside block(needed for rotation alignment)

    FVector areaPivot;
    float sizeArea = 100.0f;

    int rotation = 0; //in [0,3]
    void FaceRight();
    void FaceLeft();
    void FaceUp();
    void FaceDown();

    bool isCorner = false;
    bool isTCorner = false;
};