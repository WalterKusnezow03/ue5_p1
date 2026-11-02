#include "MaterialAssetPack.h"


void UMaterialAssetPack::ValidatePropertiesOnInit(){
    for (int i = 0; i < materials.Num(); i++){
        FMaterialPropertyAsset &current = materials[i];
        current.ValidatePropertiesOnInit();
    }
}

UMaterialInstanceDynamic *UMaterialAssetPack::MakeByName(FString name, UObject *OuterContext){
    for (int i = 0; i < materials.Num(); i++){
        FMaterialPropertyAsset &current = materials[i];
        if(current.MatchesName(name)){
            return current.MakeMaterial(OuterContext);
        }
    }
    return nullptr;
}