#pragma once

#include "CoreMinimal.h"
#include "terrainBuildingPlugin/BlockoutBuildings/Generation/Building/Enum/EBuildingTypeBlockAddon.h"
#include "terrainBuildingPlugin/BlockoutBuildings/Generation/Building/Enum/EBuildingTypeBlock.h"



#include "BlockProperty.generated.h"

/// @brief axis constraint info for CarriedItemSocket
USTRUCT(BlueprintType)
struct TERRAINBUILDINGPLUGIN_API FBlockProperty {
    GENERATED_BODY()


public:
    FBlockProperty(){}

    //none by default must be kept for blockout grid, fallback option
    UPROPERTY(EditAnywhere, Category="BlockProperty")
    EBuildingTypeBlock typeBlockProperty = EBuildingTypeBlock::ENone;

    UPROPERTY(EditAnywhere, Category="BlockProperty")
    EBuildingTypeBlockAddon addonProperty = EBuildingTypeBlockAddon::ENone; //none by default must be kept


    bool CanHaveRotation() const {
        return 
        typeBlockProperty == EBuildingTypeBlock::EOutsideWall ||
        typeBlockProperty == EBuildingTypeBlock::EInsideWall ||
        typeBlockProperty == EBuildingTypeBlock::ERoof;
    }

    bool IsInsideType() const {
        return 
        typeBlockProperty == EBuildingTypeBlock::EInsideFloor ||
        typeBlockProperty == EBuildingTypeBlock::EInsideWall;
    }





};
