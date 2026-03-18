#pragma once

#include "CoreMinimal.h"
#include "p2/ui/_uiSimulation/EUiEvent.h"
#include "p2/ui/3Dui/HUD/Widget/MinimapWidgetData/EMarkerType.h"


class P2_API UiSimulation {

public:
    UiSimulation();
    ~UiSimulation();


    void Notify(EUiEvent event, FString message);
    void Notify(EUiEvent event, AActor *actor);
    void Notify(EUiEvent event, AActor *actor, EMarkerType type);
    
private:

};