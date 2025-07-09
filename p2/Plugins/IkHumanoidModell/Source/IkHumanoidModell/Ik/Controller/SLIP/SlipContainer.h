#pragma once

#include "CoreMinimal.h"

class IKHUMANOIDMODELL_API SlipContainer {

public:
    SlipContainer();
    ~SlipContainer();

    SlipContainer(SlipContainer &other);
    SlipContainer &operator=(SlipContainer &other);


    void setup(float defaultBoneSize, FVector &currentStartToEndEffector);


private:
    bool bLogEnabled = true;

    FVector forceUnscaled(
        FVector &legdir_featherCurrent,
        FVector &movedir
    );

    float slipSineScalar(
        FVector &legdir_featherCurrent,
        FVector &movedir
    );

    float featherComplete; //ausrichtung scheint eine rolle zu spielen
    FVector featherCurrent;


    float federKonstanteD = 10.0f;

    bool wasSetup = false;

    bool debugSkipToCloseframes(FVector a, FVector b);

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

    bool slipForceAllowedDotProductUp(FVector &leg);

    //debug
    void showEstimationVersusRealVelocity();
    FVector slipVelocityEstimatedInternal;
    FVector slipVelocityIntegratedInternal;

    float estimateVmin(float timeOfAnimation);
    float avoidDivisionByZero(float value);

    FVector forceIntegrated(
        float time,
        float deltaTime,
        FVector &moveDir,
        FVector &a,
        FVector &b
    );



    //experimental
    void deRotateDirectionsForSlipSine(
        FVector &legDir,
        FVector &forward
    );
    void hackSigns(FVector &force, FVector &forward);
};