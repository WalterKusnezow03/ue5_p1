#pragma once

#include "CoreMinimal.h"
#include "terrainBuildingPlugin/BlockoutBuildings/AssetLoading/AssetData/ColorMap/BuildingColorMapData.h"
#include "terrainBuildingPlugin/BlockoutBuildings/Generation/Building/BlockOut/BuildingBlockOutFloor.h"
#include "BuildingAssetData.generated.h"


/// @brief will store image paths for a single building
/// and color codings for decoding the images
/// designed to be stored in asset manager, hold decoded data
/// inside this asset
UCLASS(BlueprintType)
class TERRAINBUILDINGPLUGIN_API UBuildingAssetData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
    virtual void BeginDestroy() override;

    //image path array by floor
    UPROPERTY(EditAnywhere, Category="BuildingAssetData")
    TArray<FString> pathsByFloor;

    UPROPERTY(EditAnywhere, Category="BuildingAssetData")
    FBuildingColorMapData colorMap;

    //cube height each
    UPROPERTY(EditAnywhere, Category="BuildingAssetData")
    float heightPerFloor = 200.0f;

    //cube width and length each (bottom quad)
    UPROPERTY(EditAnywhere, Category="BuildingAssetData")
    float sizeUniformPerBlockFloor = 100.0f;

    void LoadAll();

private:

    bool isLoaded = false;
    TArray<BuildingBlockOutFloor> loadedBlockouts;

    void Load(FString path);
};