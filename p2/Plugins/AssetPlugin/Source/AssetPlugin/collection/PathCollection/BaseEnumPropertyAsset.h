#pragma once

#include "CoreMinimal.h"
#include "AssetPlugin/EnumTools/EnumTool.h"
#include "BaseEnumPropertyAsset.generated.h"

/// @brief struct to save any asset by path (from plugin name, pathname, and asset name)
/// connected to an enum name and value
USTRUCT(BlueprintType)
struct ASSETPLUGIN_API FBaseEnumPropertyAsset
{
    GENERATED_BODY()

public:

    FBaseEnumPropertyAsset() {};

    // --- enum data ---

    //class name as enum for example
    UPROPERTY(EditAnywhere, Category="AssetData")
    FString enumName;

    UPROPERTY(EditAnywhere, Category="AssetData")
    FString enumValue;



    // --- path data ---
    UPROPERTY(EditAnywhere, Category="AssetData")
    FString pluginName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetData")
    FString innerPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetData")
    FString assetName;

    //to mark spawnable Blueprints
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AssetData")
    bool isUclass_BP = false;

    //tries to convert the enum class text, and enum value text
    //to the given Enum Type E, and finds the value (index) if possible.
    template <typename E>
    bool ExtractEnumValue(E &outValue) const {
        //if class name matched, find value
        if(EnumTool::IsEnumOfName<E>(enumName)){
            //find value of the enum
            if(EnumTool::ExtractEnumValue(outValue, enumValue)){
                return true;
            }
        }
        return false;
    }

    virtual void ValidatePropertiesOnInit(){
        
    }





};