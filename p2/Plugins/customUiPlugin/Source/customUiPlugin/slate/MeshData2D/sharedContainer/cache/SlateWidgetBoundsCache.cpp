#include "SlateWidgetBoundsCache.h"
#include "customUiPlugin/slate/MeshData2D/BaseMeshData/MeshDataWrap/SlateMeshDataPolygon.h"
#include "customUiPlugin/Private/Debug/UiDebugHelper.h"


SlateWidgetBoundsCache::SlateWidgetBoundsCache(){
    updateNeededFlag = true;
    countModified = 1;
}

SlateWidgetBoundsCache::~SlateWidgetBoundsCache(){

}

void SlateWidgetBoundsCache::SetUpdateNeededTrue(){
    updateNeededFlag = true;
    countModified++;
}

bool SlateWidgetBoundsCache::UpdateNeeded() const {
    //return updateNeededFlag;
    return countModified > 0;
}

void SlateWidgetBoundsCache::Recreate(TArray<SlateMeshDataPolygon*> &polygons){
    boundsInternal.Reset();
    updateNeededFlag = false;
    countModified--;
    
    UiDebugHelper::logMessage(FString::Printf(TEXT("SlateWidgetBoundsCache update (a) %d"), polygons.Num()));

    //make new bounding box.
    for (int i = 0; i < polygons.Num(); i++)
    {
        SlateMeshDataPolygon *current = polygons[i];
        if(current){
            boundsInternal.Update(*current);
            UiDebugHelper::logMessage("SlateWidgetBoundsCache update (b)");
        }
    }
}

FVector2D SlateWidgetBoundsCache::DesiredSize(float scalar) const{
    FVector2D scaled = boundsInternal.size() * scalar;
    if(hasExtension){
        scaled += extension;
    }

    return scaled;
}

FVector2D SlateWidgetBoundsCache::Size() const{
    return DesiredSize(1.0f);
}





// extension of bounds
// --- symetrical extension to center a widget
void SlateWidgetBoundsCache::SetExtension(int size){
    hasExtension = size > 0;
    extension = FVector2D(size, size);
    if(prevSize != size){
        prevSize = size;
        SetUpdateNeededTrue();
    }
}

void SlateWidgetBoundsCache::RemoveExtension(){
    SetExtension(0);
}
