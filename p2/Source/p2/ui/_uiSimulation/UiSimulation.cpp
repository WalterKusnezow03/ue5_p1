#include "UiSimulation.h"
#include "p2/ui/3Dui/HUD/HudUiActor.h"
#include "GameCore/Ui3D/WidgetComponentModified/Actor/CustomMeshUIActor.h"

UiSimulation::UiSimulation(){

}

UiSimulation::~UiSimulation(){

}


bool UiSimulation::Notify(EUiEvent event){
    return false;
}

bool UiSimulation::Notify(EUiEvent event, FString message){
    if(event == EUiEvent::HudTopText){
        if(AHudUiActor *actor = AHudUiActor::GetInstance()){
            //actor->updateTopWaringElement(message); //warum ist das auskommentiert?
            return true;
        }
    }
    return false;
}

// todo: aufräumen: explizite hud mini map events, function calls.
bool UiSimulation::Notify(EUiEvent event, AActor *actor){
    if(event == EUiEvent::HudMiniMapUnRegisterActor){
        if(AHudUiActor *hudUi = AHudUiActor::GetInstance()){
            hudUi->UnRegisterActorFromMiniMap(actor);
            return true;
        }
    }

    return false;
}

bool UiSimulation::Notify(EUiEvent event, AActor *actor, EMarkerType type){
    if(actor){
        if(event == EUiEvent::HudMiniMapRegisterActor){
            if(AHudUiActor *hudUi = AHudUiActor::GetInstance()){
                hudUi->RegisterActorToMiniMap(actor, type);
                return true;
            }
        }
        if(event == EUiEvent::HudMiniMapUnRegisterActor){
            Notify(event, actor);
            return true;
        }
    }
    return false;
}


bool UiSimulation::MiniMapInitialized(){
    if(AHudUiActor *hudUi = AHudUiActor::GetInstance()){
        return true;
    }
    return false;
}