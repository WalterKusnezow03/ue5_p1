#pragma once

#include "CoreMinimal.h"

#include "AssetPlugin/collection/MaterialCollection/MaterialPropertyAsset.h"
#include "MaterialAssetPack.generated.h"



/// @brief will store a set of material prefabs which can be spawned by name and uobject context
UCLASS(BlueprintType)
class ASSETPLUGIN_API UMaterialAssetPack : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
    
    void ValidatePropertiesOnInit();

    

    UPROPERTY(EditAnywhere, Category="MaterialDataAssets")
    TArray<FMaterialPropertyAsset> materials;

    UMaterialInstanceDynamic *MakeByName(FString name, UObject* OuterContext);

protected:
    
};