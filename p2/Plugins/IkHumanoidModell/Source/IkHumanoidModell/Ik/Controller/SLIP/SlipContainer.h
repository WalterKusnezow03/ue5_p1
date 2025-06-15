#pragma once

#include "CoreMinimal.h"

class IKHUMANOIDMODELL_API SlipContainer {

public:
    SlipContainer();
    ~SlipContainer();

    SlipContainer(SlipContainer &other);
    SlipContainer operator=(SlipContainer &other);


    void setup(float defaultBoneSize, FVector &currentStartToEndEffector);
    FVector force(FVector &movedir);
    FVector acceleration(float mass, FVector &movedir);

private:
    
    float featherComplete; //ausrichtung scheint eine rolle zu spielen
    FVector featherCurrent;

    float federKonstanteD = 10.0f;

    bool wasSetup = false;

    void findMinDForGravityBlock(float mass);
};