#pragma once

#include "CoreMinimal.h"
#include "AssetPlugin/EnumTools/EnumTool.h"
#include "BaseEnumPropertyAsset.h"
#include "BaseEnum2PropertyAsset.generated.h"

/// @brief struct to save any asset by path (from plugin name, pathname, and asset name)
/// connected to an enum name and value
USTRUCT(BlueprintType)
struct ASSETPLUGIN_API FBaseEnum2PropertyAsset : public FBaseEnumPropertyAsset
{
    GENERATED_BODY()

public:

    FBaseEnum2PropertyAsset() {};

    // --- enum data ---

    //class name as enum for example
    UPROPERTY(EditAnywhere, Category="AssetData")
    FString enumName_2;

    UPROPERTY(EditAnywhere, Category="AssetData")
    FString enumValue_2;

    //tries to convert the enum class text, and enum value text
    //to the given Enum Type E, and finds the value (index) if possible.
    template <typename E>
    bool ExtractEnum2Value(E &outValue) const {
        //if class name matched, find value
        if(EnumTool::IsEnumOfName<E>(enumName_2)){
            //find value of the enum
            if(EnumTool::ExtractEnumValue(outValue, enumValue_2)){
                return true;
            }
        }
        return false;
    }

    virtual void ValidatePropertiesOnInit() override{
        Super::ValidatePropertiesOnInit();
    }
};