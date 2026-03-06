#include "DynamicMaterialWrapperBase.h"
#include "Materials/MaterialInstanceDynamic.h"


void FDynamicMaterialWrapperBase::Init(UMaterial *material, UObject *outer){
    if(!outer || !material){
        return;
    }
    if (!IsValid()){
        dynamicMaterial = UMaterialInstanceDynamic::Create(material, outer);
    }
}

bool FDynamicMaterialWrapperBase::IsValid(){
    return dynamicMaterial != nullptr;
}

void FDynamicMaterialWrapperBase::SetScalarParameterValue(FString name, float value){
    if(dynamicMaterial){
        dynamicMaterial->SetScalarParameterValue(FName(*name), value);
    }
}

void FDynamicMaterialWrapperBase::SetVectorParameterValue(FString name, const FVector &value){
    if(dynamicMaterial){
        dynamicMaterial->SetVectorParameterValue(FName(*name), value);
    }
}

void FDynamicMaterialWrapperBase::SetTextureParameterValue(FString name, UTexture *texture){
    if(dynamicMaterial){
        dynamicMaterial->SetTextureParameterValue(FName(*name), texture);
    }
}
