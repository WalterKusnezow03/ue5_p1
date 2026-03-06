#include "BuildingAssetData.h"
#include "StoragePlugin/Storage/ImageData/Image/Image.h"


void UBuildingAssetData::BeginDestroy(){
    isLoaded = false;
    loadedBlockouts.Empty();
    Super::BeginDestroy();
}


void UBuildingAssetData::LoadAll(){
    if(colorMap.ColorMapIsValid() && !isLoaded){
        isLoaded = true; //will be stored in asset and asset manager.

        for (int i = 0; i < pathsByFloor.Num(); i++){
            Load(pathsByFloor[i]);
        }
    }
}


void UBuildingAssetData::Load(FString path){
    Image image;
    if(image.LoadFromPath(path) && image.SizeValid()){
        loadedBlockouts.SetNum(loadedBlockouts.Num() + 1);
        BuildingBlockOutFloor &last = loadedBlockouts.Last();
        last.CreateFrom(image, colorMap, sizeUniformPerBlockFloor);
    }
}