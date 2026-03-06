#include "BuildingBlockOutFloor.h"


#include "StoragePlugin/Storage/ImageData/Image/Image.h"
#include "terrainBuildingPlugin/BlockoutBuildings/AssetLoading/AssetData/ColorMap/BuildingColorMapData.h"


void BuildingBlockOutFloor::Clear(){
    blockGrid.Empty();
}

void BuildingBlockOutFloor::CreateFrom(
    Image &image, 
    const FBuildingColorMapData &colorMap,
    float sizePerQuad
){
    if(image.IsValid()){
        Clear();
        CreateGrid(image);
        UpdateBlockoutFromColors(image, colorMap);
        UpdateBlockoutPivotsAndArea(sizePerQuad);
        UpdateBlockoutDesiredRotations();
    }
}

void BuildingBlockOutFloor::CreateGrid(Image &image){
    CreateGrid(image.widthX(), image.heightY());
}

void BuildingBlockOutFloor::CreateGrid(int x, int y){
    if(x > 0 && y > 0){
        blockGrid.SetNum(x);
        for (int i = 0; i < x; i++){
            TArray<BuildingBlockBase> &current = blockGrid[i];
            current.SetNum(y);
        }
    }
}


void BuildingBlockOutFloor::UpdateBlockoutFromColors(
    Image &image, 
    const FBuildingColorMapData &colorMap
){
    const TArray<TArray<FColor>> &ref = image.GetData();
    UpdateBlockoutFromColors(ref, colorMap);
}

void BuildingBlockOutFloor::UpdateBlockoutFromColors(
    const TArray<TArray<FColor>> &data,
    const FBuildingColorMapData &colorMap
){
    //for all colors: override property found from color map
    for (int i = 0; i < data.Num(); i++){
        const TArray<FColor> &colorColumn = data[i];
        for (int j = 0; j < colorColumn.Num(); j++){
            UpdateBuildingBlock(i, j, colorColumn[j], colorMap);
        }
    }
}

void BuildingBlockOutFloor::UpdateBuildingBlock(
    int i, 
    int j, 
    const FColor &color,
    const FBuildingColorMapData &colorMap
){
    BuildingBlockBase &foundToUpdate = Find(i, j);
    FBlockProperty &propertyToUpdate = foundToUpdate.GetPropertyRef();

    float distanceAllowed = 10.0f; // in 0 to 255
    colorMap.FindProperty(color, propertyToUpdate, distanceAllowed);
}


bool BuildingBlockOutFloor::IndexValid(int x, int y){
    if(x >= 0 && x < blockGrid.Num()){
        TArray<BuildingBlockBase> &currentColumn = blockGrid[x];
        if(y >= 0 && y < currentColumn.Num()){
            return true;
        }
    }
    return false;
}

BuildingBlockBase &BuildingBlockOutFloor::Find(int x, int y){
    if(IndexValid(x,y)){
        return blockGrid[x][y];
    }
    return none;
}



//update pivots and areas
void BuildingBlockOutFloor::UpdateBlockoutPivotsAndArea(float sizePerQuad){
    //per grid step move with sizeQuad as Step size to create the pivots.
    for (int i = 0; i < blockGrid.Num(); i++){
        FVector offsetX(i * sizePerQuad, 0, 0);
        TArray<BuildingBlockBase> &currentColumn = blockGrid[i];
        for (int j = 0; j < currentColumn.Num(); j++){
            FVector offsetY(0, j * sizePerQuad, 0);
            BuildingBlockBase &current = currentColumn[j];

            //apply offset
            FVector totalOffset = offsetX + offsetY;
            current.SetOffsetAndQuadArea(totalOffset, sizePerQuad);

        }
    }
}

//update desired rotations for meshdata
void BuildingBlockOutFloor::UpdateBlockoutDesiredRotations(){
     for (int i = 0; i < blockGrid.Num(); i++){
        TArray<BuildingBlockBase> &currentColumn = blockGrid[i];
        for (int j = 0; j < currentColumn.Num(); j++){
            UpdateBlockoutDesiredRotationForBlock(i, j);
        }
    }
}

void BuildingBlockOutFloor::UpdateBlockoutDesiredRotationForBlock(int x, int y){
    if(IndexValid(x,y)){
        BuildingBlockBase &current = Find(x,y);

        current.UpdateRoationAndCornerFlag(
            TopOf(x,y),
            BottomOf(x,y),
            RightOf(x,y),
            LeftOf(x,y)
        );
    }
}









BuildingBlockBase &BuildingBlockOutFloor::RightOf(int x, int y){
    return Find(x - 1, y);
}
BuildingBlockBase &BuildingBlockOutFloor::LeftOf(int x, int y){
    return Find(x - 1, y);
}
BuildingBlockBase &BuildingBlockOutFloor::TopOf(int x, int y){
    return Find(x, y - 1);
}
BuildingBlockBase &BuildingBlockOutFloor::BottomOf(int x, int y){
    return Find(x, y - 1);
}
