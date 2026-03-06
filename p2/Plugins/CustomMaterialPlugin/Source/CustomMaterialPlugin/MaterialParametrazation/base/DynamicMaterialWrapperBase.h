#pragma once

#include "CoreMinimal.h"
#include <map>

/// --- testing needed ---

/// @brief wrapping umaterial to abstract parameter values
/// holds the material instance dynamic, 
/// is needed for dynamic material parameters where the material
/// is applied to more than one object.
/// will also handle timing when to change a value.
/// Subclass will allow to register params by template type T
class CUSTOMMATERIALPLUGIN_API FDynamicMaterialWrapperBase {

   

protected:
    class UMaterialInstanceDynamic *dynamicMaterial = nullptr;
   

public:
    void Init(UMaterial *material, UObject *outer);

    void SetScalarParameterValue(FString name, float value);
    void SetVectorParameterValue(FString name, const FVector &value);
    void SetTextureParameterValue(FString name, UTexture *texture);

    bool IsValid();


};
