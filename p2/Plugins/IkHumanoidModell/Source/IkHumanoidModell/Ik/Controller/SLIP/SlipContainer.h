#pragma once

#include "CoreMinimal.h"

class IKHUMANOIDMODELL_API SlipContainer {

public:
    SlipContainer();
    ~SlipContainer();

    SlipContainer(SlipContainer &other);
    SlipContainer operator=(SlipContainer &other);


    void setup(float defaultBoneSize, FVector &currentStartToEndEffector);
    FVector force();
    FVector acceleration(float mass);

private:
    
    float featherComplete; //ausrichtung scheint eine rolle zu spielen
    FVector featherCurrent;

    float federKonstanteD = 1.0f;

    bool wasSetup = false;
};