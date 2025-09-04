#include "SlatePolygonMap.h"
#include "customUiPlugin/slate/MeshData2D/SlateMeshData.h"


SlatePolygonMap::SlatePolygonMap(){

}

SlatePolygonMap::~SlatePolygonMap(){

}

SlateMeshDataPolygon &SlatePolygonMap::FindPolygonByLayerInternal(int layerId){
    if(!HasLayer(layerId)){
        polygonMap[layerId] = SlateMeshDataPolygon();
        SortLayers(); //sort layers again.

        //FLAG UPDATE NEEDED BECAUSE UNTERNAL MESH DATA MAY GET MODIFIED
        boundsCache.FlagUpdateNeededTrue();
       
    }
    return polygonMap[layerId];
}

bool SlatePolygonMap::HasLayer(int layerId) const {
    return polygonMap.find(layerId) != polygonMap.end();
}


void SlatePolygonMap::Tick(float deltatime){
    bool changesRegistered = false; //ignored.
    TArray<SlateMeshDataPolygon *> polygons = allPolygonsSorted();
    for (int i = 0; i < polygons.Num(); i++){
        
        if(SlateMeshDataPolygon *current = polygons[i]){
            //A & w = A, A & f = f
            //A | w = w, A | f = A
            bool copy = current->Tick(deltatime);
            changesRegistered = changesRegistered || copy;
        }
    }
    UpdateBoundsForSizeCalculation(); //update bounds if needed, modified mesh data.
}

void SlatePolygonMap::UpdateCursorPosition(FVector2D &cursorLocalSpace){
    TArray<SlateMeshDataPolygon *> polygons = allPolygonsSorted();
    for (int i = 0; i < polygons.Num(); i++){
        
        if(SlateMeshDataPolygon *current = polygons[i]){
            
            current->UpdateCursorPosition(cursorLocalSpace);
        }
    }
}




void SlatePolygonMap::DebugCreatePolygons(){

    SlateMeshDataPolygon &polygon = FindPolygonByLayerInternal(0);

    TArray<FVector2D> shape = {
        FVector2D(0, 0),
        FVector2D(100, 0),
        FVector2D(200, 0),
        FVector2D(300, 300),
        FVector2D(100, 200)
    };
    polygon.AppendClosedShape(shape, 10); //20 freeze, 5 to low!

    //polygon.SetColor(FLinearColor::Blue);
    polygon.SetCursorColor(FLinearColor::White);

    SlateMeshData &meshdata = polygon.MeshDataRef();
    meshdata.AddAmbientUvColor(FVector2D(0, 0), FLinearColor::Red);
    meshdata.AddAmbientUvColor(FVector2D(1, 1), FLinearColor::Blue);

    //debug
    if(true){
        int sizeMap = polygonMap.size();
        UiDebugHelper::logMessage(
            FString::Printf(TEXT("SlateWidget: size map construct: "), sizeMap)
        );
    }

    ForceUpdateBoundsForSizeCalculation();
}




FVector2D SlatePolygonMap::Bounds(){
    UpdateBoundsForSizeCalculation();
    return boundsCache.Size();
}

void SlatePolygonMap::UpdateBoundsForSizeCalculation(){
    //if new bounds needed, recreate:
    if(boundsCache.UpdateNeeded()){
        ForceUpdateBoundsForSizeCalculation();
    }
}

void SlatePolygonMap::ForceUpdateBoundsForSizeCalculation(){
    TArray<SlateMeshDataPolygon *> polygons = allPolygonsSorted();
    boundsCache.Recreate(polygons);

    //debug
    FVector2D size = boundsCache.Size();
    UiDebugHelper::logMessage(
        FString::Printf(
            TEXT("slate: SSlateWidgetBase bounds! %.2f %.2f"),
            size.X,
            size.Y
        )
    );
}




/// --- DATA MANAGEMENT ---
TArray<SlateMeshDataPolygon *> SlatePolygonMap::allPolygonsSorted(){
    TArray<SlateMeshDataPolygon *> outArray;
    for (int i = 0; i < layersSorted.Num(); i++){
        int layer = layersSorted[i];
        if (HasLayer(layer))
        {
            outArray.Add(&FindPolygonByLayerInternal(layer));
        }
    }
    return outArray;
}

TArray<SlateMeshDataPolygon *> SlatePolygonMap::allPolygonsSortedConst() const {
    TArray<SlateMeshDataPolygon *> outArray;
    for (int i = 0; i < layersSorted.Num(); i++){
        int layer = layersSorted[i];
        if (HasLayer(layer))
        {
            std::map<int, SlateMeshDataPolygon> &mutableMap = 
                const_cast<std::map<int, SlateMeshDataPolygon> &>(polygonMap);
            SlateMeshDataPolygon *ptr = &mutableMap[layer];
            outArray.Add(ptr);
            //outArray.Add(&polygonMap[layer]);
        }
    }
    return outArray;
}






void SlatePolygonMap::SortLayers(){
    TArray<int> allLayers;
    for(auto &pair : polygonMap){
        allLayers.Add(pair.first);
    }
    allLayers.Sort();
    layersSorted = allLayers;
}

