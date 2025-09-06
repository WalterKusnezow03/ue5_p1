#include "SlateWidgetBoundsCache.h"
#include "customUiPlugin/slate/MeshData2D/SlateMeshDataPolygon.h"
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
    return scaled;
}

FVector2D SlateWidgetBoundsCache::Size() const{
    return DesiredSize(1.0f);
}