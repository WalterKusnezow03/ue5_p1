#include "MiniMapMarker.h"
#include "customuipluginbase/baseInterface/WidgetHelper.h"
#include "Components/CanvasPanelSlot.h"


//unklar ob das so bestehen bleibt - oder anders hinzugefüht wird
void UMiniMapMarker::Init(UCanvasPanelSlot *slotIn){
    if(slotIn){
        slot = slotIn;
        slot->SetAlignment(FVector2D(0.5f, 0.5f)); //set pivot center
    }
}


void UMiniMapMarker::UpdateTypeAndLocation(EMarkerType type, const FVector2D &target){
    SetType(type);
    UpdateLocation(target);
}

void UMiniMapMarker::UpdateLocation(const FVector2D &target){
    if(slot){
        slot->SetPosition(target);
    }
}

//hide unhide, cached by minimap widget to reassign
bool UMiniMapMarker::MarkedFree(){
    return bMarkedFree;
}

void UMiniMapMarker::MarkFree(bool flag){
    bMarkedFree = flag;
    WidgetHelper::SetVisible(this, !flag);
}

void UMiniMapMarker::SetType(EMarkerType type){
   
    /*
    EMarkerType::
    EEnemy,
    EPlayer,
    EWeapon,
    */

    std::map<EMarkerType, UWidget *> markerMap;
    markerMap[EMarkerType::ENeutralEntity] = GetNeutralMarker();
    markerMap[EMarkerType::EEnemy] = GetEnemyMarker();
    markerMap[EMarkerType::EPlayer] = GetPlayerMarker();
    markerMap[EMarkerType::EWeapon] = GetWeaponMarker();

    WidgetHelper helper;
    for (auto &pair : markerMap)
    {
        bool visible = (pair.first == type); //same enum type.
        if (UWidget *widget = pair.second)
        {
            helper.SetVisible(widget, visible);
        }
    }
}
