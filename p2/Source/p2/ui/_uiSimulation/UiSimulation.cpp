#include "UiSimulation.h"
#include "p2/ui/3Dui/HUD/HudUiActor.h"
#include "GameCore/Ui3D/WidgetComponentModified/Actor/CustomMeshUIActor.h"

UiSimulation::UiSimulation(){

}

UiSimulation::~UiSimulation(){

}

void UiSimulation::Notify(EUiEvent event, FString message){
    if(event == EUiEvent::HudTopText){
        if(AHudUiActor *actor = AHudUiActor::GetInstance()){
            //actor->updateTopWaringElement(message);
        }
    }
}




// todo: aufräumen: explizite hud mini map events, function calls.
void UiSimulation::Notify(EUiEvent event, AActor *actor){
    if(event == EUiEvent::HudMiniMapUnRegisterActor){
        if(AHudUiActor *hudUi = AHudUiActor::GetInstance()){
            hudUi->UnRegisterActorFromMiniMap(actor);
        }
    }

}

void UiSimulation::Notify(EUiEvent event, AActor *actor, EMarkerType type){
    if(actor){
        if(event == EUiEvent::HudMiniMapRegisterActor){
            if(AHudUiActor *hudUi = AHudUiActor::GetInstance()){
                hudUi->RegisterActorToMiniMap(actor, type);
            }
        }
        if(event == EUiEvent::HudMiniMapUnRegisterActor){
            Notify(event, actor);
        }
    }
}