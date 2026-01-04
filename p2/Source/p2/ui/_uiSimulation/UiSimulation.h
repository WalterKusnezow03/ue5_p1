#pragma once

#include "CoreMinimal.h"
#include "p2/ui/_uiSimulation/EUiEvent.h"


class P2_API UiSimulation {

public:
    UiSimulation();
    ~UiSimulation();

    void Notify(EUiEvent event, FString message);

    void RayCast3DUi(
        UWorld *world, 
        const FVector &pos, 
        const FVector &dir
    );

private:

};