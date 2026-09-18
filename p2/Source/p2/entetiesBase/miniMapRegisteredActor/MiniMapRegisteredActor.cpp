#include "MiniMapRegisteredActor.h"
#include "p2/ui/_uiSimulation/UiSimulation.h"
#include "p2/ui/_uiSimulation/EUiEvent.h"
#include "p2/_world/worldLevel.h"

AMiniMapRegisteredActor::AMiniMapRegisteredActor() : Super() {

};


void AMiniMapRegisteredActor::UnRegisterFromMiniMap(){
    if(AworldLevel::uiSimulation.Notify(
        EUiEvent::HudMiniMapUnRegisterActor, 
        this
    )){
        queuedForRemoveFromMinimap = false;
    }else{
        queuedForRemoveFromMinimap = true;
    }

    queuedForAddToMinimap = false;
}

void AMiniMapRegisteredActor::RegisterToMiniMap(){
    if(AworldLevel::uiSimulation.Notify(
        EUiEvent::HudMiniMapRegisterActor, 
        this,
        GetMarkerType() //must be overriden by subclasses.
    )){
        queuedForAddToMinimap = false;
    }else{
        queuedForAddToMinimap = true;
    }
    queuedForRemoveFromMinimap = false;
}

// ----- mini map ------
void AMiniMapRegisteredActor::UpdateMiniMapRegistration(){
	UnRegisterFromMiniMap();
	RegisterToMiniMap();
}

void AMiniMapRegisteredActor::EndPlay(const EEndPlayReason::Type EndPlayReason){
    UnRegisterFromMiniMap();
    Super::EndPlay(EndPlayReason);
}

void AMiniMapRegisteredActor::Tick(float deltatime){
    Super::Tick(deltatime);
    if(queuedForAddToMinimap){
        RegisterToMiniMap();
    }
    if(queuedForRemoveFromMinimap){
        UnRegisterFromMiniMap();
    }
}

