#pragma once

#include "CoreMinimal.h"

#include "BaseEnumPropertyAsset.h"
#include "BaseEnum2PropertyAsset.h"
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

    //data array of the (enum data + path data) of the abstract asset
    UPROPERTY(EditAnywhere, Category="EnumPropertyDataAssets")
    TArray<FBaseEnum2PropertyAsset> dataTwoEnum;

    void ValidatePropertiesOnInit(){
        for (int i = 0; i < data.Num(); i++){
            FBaseEnumPropertyAsset &current = data[i];
            current.ValidatePropertiesOnInit();
        }
        for (int i = 0; i < dataTwoEnum.Num(); i++){
            FBaseEnum2PropertyAsset &current = dataTwoEnum[i];
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

    /// @brief safes compatible assets by enum type E and F to asset Manager
    /// with the given class T (UClass, Texture, etc)
    /// @tparam T Class
    /// @tparam E, F Enum
    template<typename E, typename F, typename T>
    void SafeCompatibleAssetsToAssetManager(){
        //check if enum is ok, if so: safe with asset loader

        for(int i = 0; i < dataTwoEnum.Num(); i++){
            const FBaseEnum2PropertyAsset &current = dataTwoEnum[i];

            UE_LOG(LogTemp, Log, TEXT("Try Load Enum 2 asset %d of %d"), 1 + i, dataTwoEnum.Num());

            bool found = false;

            E extractedType;
            F extractedType2;
            if(
                current.ExtractEnumValue<E>(extractedType) &&
                current.ExtractEnum2Value<F>(extractedType2)
            ){
                UE_LOG(LogTemp, Log, TEXT("Try Load Enum 2 asset: enumName=%s, enumValue=%s"),
                    *current.enumName,
                    *current.enumValue
                );

                AssetLoader::LoadAndSaveAssetToManager<E, F, T>(
                    extractedType,      // E enumValue,
                    extractedType2,     // F enumValue
                    current.pluginName, // FString pluginName,
                    current.innerPath,  // FString innerPath,
                    current.assetName   // FString assetName
                );
                //return;
                found = true;
            }else{
                //swap if needed, safety / caught error
                F extractedType_B;
                E extractedType2_B;
                if(
                    !found &&
                    current.ExtractEnumValue<F>(extractedType_B) &&
                    current.ExtractEnum2Value<E>(extractedType2_B)
                ){
                    // Kleiner Hinweis im Log, dass hier geflippt wurde
                    UE_LOG(LogTemp, Log, TEXT("Try Load Enum 2 asset (Flipped): enumName=%s, enumValue=%s"),
                        *current.enumName,
                        *current.enumValue
                    );

                    AssetLoader::LoadAndSaveAssetToManager<E, F, T>(
                        extractedType2_B,      // F enumValue,
                        extractedType_B,     // E enumValue
                        current.pluginName, // FString pluginName,
                        current.innerPath,  // FString innerPath,
                        current.assetName   // FString assetName
                    );
                }
            }

        }
    }




protected:
    
};