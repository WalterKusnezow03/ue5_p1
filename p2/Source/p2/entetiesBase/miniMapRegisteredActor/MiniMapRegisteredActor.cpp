#include "MiniMapRegisteredActor.h"
#include "p2/ui/_uiSimulation/UiSimulation.h"
#include "p2/ui/_uiSimulation/EUiEvent.h"
#include "p2/_world/worldLevel.h"

AMiniMapRegisteredActor::AMiniMapRegisteredActor() : Super() {

};


void AMiniMapRegisteredActor::UnRegisterFromMiniMap(){
    AworldLevel::uiSimulation.Notify(
        EUiEvent::HudMiniMapUnRegisterActor, 
        this
    );
}

void AMiniMapRegisteredActor::RegisterToMiniMap(){
    AworldLevel::uiSimulation.Notify(
        EUiEvent::HudMiniMapRegisterActor, 
        this,
        GetMarkerType() //must be overriden by subclasses.
    );
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