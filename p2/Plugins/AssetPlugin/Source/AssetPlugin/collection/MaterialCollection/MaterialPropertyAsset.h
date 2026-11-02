#pragma once

#include "CoreMinimal.h"
#include "MaterialPropertyAsset.generated.h"

/// @brief struct to save material 
USTRUCT(BlueprintType)
struct ASSETPLUGIN_API FMaterialPropertyAsset
{
    GENERATED_BODY()

public:

    FMaterialPropertyAsset() {};

    UPROPERTY(EditAnywhere, Category="MaterialData")
    FString uiMaterialName;

    // NEU: Referenz auf das Basis-Material oder eine Instanz
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MaterialData")
    class UMaterialInterface* BaseMaterial = nullptr;
    
    UMaterialInstanceDynamic *MakeMaterial(UObject* OuterContext){
        if(BaseMaterial && OuterContext){
            UMaterialInstanceDynamic* DynamicMat = UMaterialInstanceDynamic::Create(BaseMaterial, OuterContext);
            return DynamicMat;
        }
        return nullptr;
    }

    void ValidatePropertiesOnInit(){
        
    }

    bool MatchesName(FString nameIn){
        return nameIn.Equals(uiMaterialName);
    }
};