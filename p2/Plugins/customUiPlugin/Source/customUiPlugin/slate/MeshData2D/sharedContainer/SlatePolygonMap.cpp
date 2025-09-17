#include "SlatePolygonMap.h"
#include "customUiPlugin/slate/MeshData2D/BaseMeshData/SlateMeshData.h"
#include "CoreMath/Matrix/2D/MMatrix2D.h"


SlatePolygonMap::SlatePolygonMap(){

}

SlatePolygonMap::~SlatePolygonMap(){

}


SlateMeshDataPolygon &SlatePolygonMap::FindPolygonByLayerInternal(int layerId){
    if(!HasLayer(layerId)){
        polygonMap[layerId] = SlateMeshDataPolygon();
        UpdateSortedLayers(); // sort layers again.

        //FLAG UPDATE NEEDED BECAUSE UNTERNAL MESH DATA MAY GET MODIFIED
        boundsCache.SetUpdateNeededTrue();
       
    }
    return polygonMap[layerId];
}

bool SlatePolygonMap::HasLayer(int layerId) const {
    return (polygonMap.find(layerId) != polygonMap.end());
    //&&layersSorted.Contains(layerId);
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
    polygon.AppendClosedShape(shape, 10); //20 freeze, 5 to low! (10 recursion depth triangle split)

    //polygon.SetColor(FLinearColor::Blue);
    polygon.SetCursorColor(FLinearColor::White);

    SlateMeshData &meshdata = polygon.MeshDataRef();
    meshdata.AddAmbientUvColor(FVector2D(0, 0), FLinearColor::Red);
    meshdata.AddAmbientUvColor(FVector2D(1, 1), FLinearColor::Blue);

    //debug
    if(true){
        int sizeMap = polygonMap.size();
        UiDebugHelper::logMessage(
            FString::Printf(TEXT("SlateWidget: size map construct: %d "), sizeMap)
        );
    }

    ForceUpdateBoundsForSizeCalculation();
}



void SlatePolygonMap::SetScaleExtionsion(int extension){
    boundsCache.SetExtension(extension); //flags update bounds needed internally!
}

bool SlatePolygonMap::BoundsUpdated(){
    return boundsCache.UpdateNeeded();
}

FVector2D SlatePolygonMap::Bounds(){
    if(boundsCache.UpdateNeeded()){
        ForceUpdateBoundsForSizeCalculation();
    }
    return boundsCache.Size();
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






void SlatePolygonMap::UpdateSortedLayers(){
    TArray<int> allLayers;
    for (auto &pair : polygonMap)
    {
        allLayers.Add(pair.first);
    }
    allLayers.Sort();
    layersSorted = allLayers;
}




// --- immidate transform ---
void SlatePolygonMap::ApplyTransformImmidiate(MMatrix2D &transform){
    TArray<SlateMeshDataPolygon *> array = allPolygonsSorted(); //marked dirty bounds once ref is get
    for (int i = 0; i < array.Num(); i++){
        if(SlateMeshDataPolygon *ptr = array[i]){
            ptr->ApplyTransformImmidiate(transform);
        }
    }
    boundsCache.SetUpdateNeededTrue();
}


void SlatePolygonMap::ScaleToResolutionImmidiate(FVector2D &res){
    FVector2D currentSize = Bounds();

    // num * x = target
    // x = target / num
    MMatrix2D scaleMat;
    scaleMat.makeIdentity();

    if(false){
        UiDebugHelper::logMessage(
            FString::Printf(
                TEXT("SlatePolygonMap (%d): ScaleToResolutionImmidiate(%.2f,%.2f)->(%.2f,%.2f) with (%.2f,%.2f) "),
                allPolygonsSorted().Num(),
                currentSize.X, currentSize.Y,
                res.X, res.Y,
                res.X / currentSize.X,
                res.Y / currentSize.Y
            )
        );
    }
    



    scaleMat.Scale(
        res.X / currentSize.X,
        res.Y / currentSize.Y
    );
    ApplyTransformImmidiate(scaleMat);
}