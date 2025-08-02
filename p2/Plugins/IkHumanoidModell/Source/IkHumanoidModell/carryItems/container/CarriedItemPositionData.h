#pragma once

#include "CoreMinimal.h"

class IKHUMANOIDMODELL_API CarriedItemPositionData {

public:
    CarriedItemPositionData();
    ~CarriedItemPositionData();

    void updateHandTargets(
        FVector rightHand,
        FVector leftHand
    );

    FVector rightHandWorld();
    FVector leftHandWorld();

private:
    FVector rightHandWorldSaved;
    FVector leftHandWorldSaved;
};