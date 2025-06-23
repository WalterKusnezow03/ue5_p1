#pragma once

#include "CoreMinimal.h"

class IKHUMANOIDMODELL_API SlipContainer {

public:
    SlipContainer();
    ~SlipContainer();

    SlipContainer(SlipContainer &other);
    SlipContainer &operator=(SlipContainer &other);


    void setup(float defaultBoneSize, FVector &currentStartToEndEffector);
    FVector force(FVector &movedir);
    FVector acceleration(float mass, FVector &movedir);

private:
    bool bLogEnabled = false;

    FVector forceUnscaled(
        FVector &legdir_featherCurrent,
        FVector &movedir
    );

    float featherComplete; //ausrichtung scheint eine rolle zu spielen
    FVector featherCurrent;

    float federKonstanteD = 10.0f;

    bool wasSetup = false;

    //depracted
    float findMinDForGravityBlock(float mass);






public:
    void setupInterpolatedD(
        FVector &endA,
        FVector &endB,
        FVector &movedir,
        float time,
        float velocity,
        float defaultBoneSize,
        float mass
    );
    FVector accelerationInterpolated(
        float deltatime,
        float mass,
        FVector &movedir
    );

private:
    //new
    FVector A;
    FVector B;
    
    float timeForInterpolation;
    float Dcurrent = 1.0f;

    float estimateVmin(float timeOfAnimation);
    float avoidDivisionByZero(float value);

    FVector forceIntegrated(
        float time,
        float deltaTime,
        FVector &moveDir,
        FVector &a,
        FVector &b
    );
};