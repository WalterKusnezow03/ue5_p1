#include "MiniMapWidget.h"
#include "customuipluginbase/Dispatcher/Filter/WidgetFilter.h"

#include "Components/CanvasPanel.h"
#include "Components/SizeBox.h"

#include "p2/ui/3Dui/HUD/Widget/MinimapWidgetData/container/FMiniMapMarkerTransform.h"



void UMiniMapWidget::AddMarker(EMarkerType type, AActor *actor){
    miniMapData.AddMarker(type, actor);
}


void UMiniMapWidget::RemoveMarker(AActor *actor){
    miniMapData.RemoveMarker(actor);
}

void UMiniMapWidget::SetPlayerReference(AActor *playerIn){
    if(player){
        RemoveMarker(player); //move old player
    }
    player = playerIn;
    AddMarker(EMarkerType::EPlayer, player);
}

void UMiniMapWidget::Tick(float DeltaTime){
    UpdatePlayerTransformToMinimapData();
    UpdateMarkers();
}



void UMiniMapWidget::UpdatePlayerTransformToMinimapData(){
    if(player){
        miniMapData.UpdatePlayerTransform(player);
    }
}

void UMiniMapWidget::UpdateMarkers(){
    //mark all free, and create "new" ones from the cache (none are really deleted.)
    MarkAllMarkersFree();
    FVector2D res(0, 0);
    if(GetMiniMapResolution(res)){

        //get All markers needed
            //childs blind override including position
                //mark free false
                //UpdateTypeAndLocation(EMarkerType type, const FVector2D &target)

            //marker iteration map exceded, but childs still there: mark free!

        UpdateMarkers(miniMapData.MapFromCollectMarkersCanvasSpace(res));
    }
}

//only call after mark all markersfree
void UMiniMapWidget::UpdateMarkers(
    const std::map<EMarkerType, TArray<FMiniMapMarkerTransform>> &mapIn
){
    for(auto &pair : mapIn){
        EMarkerType type = pair.first;
        const TArray<FMiniMapMarkerTransform> &array = pair.second;
        UpdateMarkers(type, array);
    }
}

//only call from map
void UMiniMapWidget::UpdateMarkers(
    EMarkerType type,
    const TArray<FMiniMapMarkerTransform> &array
){
    //just get since all markers were marked free before 
    for(int i = 0; i < array.Num(); i++){
        const FMiniMapMarkerTransform &targetTransform = array[i];
        UMiniMapMarker *current = CreateMarker(type, targetTransform);
    }
}

UCanvasPanel *UMiniMapWidget::GetBaseCanvasCasted(){
    if(UWidget *raw = GetBaseCanvas()){
        if(UCanvasPanel *panel = Cast<UCanvasPanel>(raw)){
            return panel;
        }
    }
    return nullptr;
}

USizeBox *UMiniMapWidget::GetSizeBoxCasted(){
    if(UWidget *raw = GetSizeBox()){
        if(USizeBox *casted = Cast<USizeBox>(raw)){
            return casted;
        }
    }
    return nullptr;
}

bool UMiniMapWidget::GetMiniMapResolution(FVector2D &outRes){
    if(USizeBox *box = GetSizeBoxCasted()){
        //better if no additional scaling is present.
        outRes = box->GetCachedGeometry().GetLocalSize();   
        return true;
    }
    return false;
}

//// ----> TODO: GENERATE MARKER FROM T SUBCLASS OR FIND FROM MAP!
//// ----> Marker must self switch to any type?

void UMiniMapWidget::MarkAllMarkersFree(){
    TArray<UMiniMapMarker *> markers = GetAllMarkerChildren();
    for (int i = 0; i < markers.Num(); i++){
        if(UMiniMapMarker *current = markers[i]){
            current->MarkFree(true);
        }
    }
}

UMiniMapMarker *UMiniMapWidget::CreateMarker(
    EMarkerType type,
    const FMiniMapMarkerTransform &transformIn
){
    //create marker and after that set pos
    if(UMiniMapMarker *item = CreateMarker(type)){
        item->UpdateTransform(transformIn);

        return item;
    }
    return nullptr;
}

UMiniMapMarker *UMiniMapWidget::CreateMarker(EMarkerType type){
    if(UMiniMapMarker *marker = FindMarker()){
        marker->SetType(type);
        marker->MarkFree(false);
        return marker;
    }
    UMiniMapMarker* created = CreateWidget<UMiniMapMarker>(GetWorld(), widgetClassForItem);
    if(created){

        //add to canvas!
        AddMarkerToCanvasAndInit(created);
        created->SetType(type);
        created->MarkFree(false);
        return created;
    }
    return nullptr;
}

void UMiniMapWidget::AddMarkerToCanvasAndInit(UMiniMapMarker *marker){
    if(marker){
        if(UCanvasPanel *panel = GetBaseCanvasCasted()){
            //UCanvasPanelSlot * UcanvasPanel::AddChildToCanvas ( UWidget* Content)
            UCanvasPanelSlot *slot = panel->AddChildToCanvas(marker);
            marker->Init(slot);
            marker->MarkFree(false);
        }
    }
}




UMiniMapMarker* UMiniMapWidget::FindMarker(){
    TArray<UMiniMapMarker *> allMarkers = GetAllMarkerChildren();
    for (int i = 0; i < allMarkers.Num(); i++){
        if(UMiniMapMarker *current = allMarkers[i]){
            if(current->MarkedFree()){
                return current;
            }
        }
    }
    return nullptr;
}

TArray<UMiniMapMarker*> UMiniMapWidget::GetAllMarkerChildren(){
    TArray<UMiniMapMarker *> array;
    
    if (UCanvasPanel *panel = GetBaseCanvasCasted())
    {
        WidgetFilter filter;
        filter.AppendDirectChildrenFromPanel(panel, array);
    }
    return array;
}

