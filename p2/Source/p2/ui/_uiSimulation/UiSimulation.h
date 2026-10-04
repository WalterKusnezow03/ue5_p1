#pragma once

#include "CoreMinimal.h"
#include "p2/ui/_uiSimulation/EUiEvent.h"
#include "MinimapPlugin/Public/Widget/MinimapWidgetData/EMarkerType.h"


class P2_API UiSimulation {

public:
    UiSimulation();
    ~UiSimulation();

    bool Notify(EUiEvent event);
    bool Notify(EUiEvent event, FString message);
    bool Notify(EUiEvent event, AActor *actor);
    bool Notify(EUiEvent event, AActor *actor, EMarkerType type);

    bool MiniMapInitialized();

private:

};