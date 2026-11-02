#pragma once

#include "CoreMinimal.h"

#include "BaseEnumPropertyAsset.h"
#include "AssetPlugin/gamestart/AssetLoader.h"
#include "EnumPropertyCollectionAsset.generated.h"



/// @brief will store a array of Enummed Property assets
/// which means they can be found by an enum and type (if the string is matched)
/// inside the data asset.
UCLASS(BlueprintType)
class ASSETPLUGIN_API UEnumPropertyCollectionAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

    //data array of the (enum data + path data) of the abstract asset
    UPROPERTY(EditAnywhere, Category="EnumPropertyDataAssets")
    TArray<FBaseEnumPropertyAsset> data;

    void ValidatePropertiesOnInit(){
        for (int i = 0; i < data.Num(); i++){
            FBaseEnumPropertyAsset &current = data[i];
            current.ValidatePropertiesOnInit();
        }
    }

    /// @brief safes compatible assets by enum type E to asset Manager
    /// with the given class T (UClass, Texture, etc)
    /// @tparam T Class
    /// @tparam E Enum
    template<typename E, typename T>
    void SafeCompatibleAssetsToAssetManager(){
        //check if enum is ok, if so: safe with asset loader

        for(int i = 0; i < data.Num(); i++){
            const FBaseEnumPropertyAsset &current = data[i];

            E extractedType;
            if(current.ExtractEnumValue<E>(extractedType)){
                UE_LOG(LogTemp, Log, TEXT("Try Load Enum asset: enumName=%s, enumValue=%s"),
                    *current.enumName,
                    *current.enumValue
                );

                AssetLoader::LoadAndSaveAssetToManager<E, T>(
                    extractedType,      // E enumValue,
                    current.pluginName, // FString pluginName,
                    current.innerPath,  // FString innerPath,
                    current.assetName   // FString assetName
                );
            }
        }
    }




protected:
    
};