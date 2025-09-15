#pragma once

#include "CoreMinimal.h"
#include "IkHumanoidModell/Ik/Controller/SLIP/SlipForceCache/SlipForceCache.h"

class IKHUMANOIDMODELL_API SlipContainer {

public:
    SlipContainer();
    ~SlipContainer();

    SlipContainer(SlipContainer &other);
    SlipContainer &operator=(SlipContainer &other);

  

    void setup(float defaultBoneSize, FVector &currentStartToEndEffector);

private:
    bool bLogEnabled = false; //disable.

    FVector forceUnscaled(
        FVector &legdir_featherCurrent,
        FVector &movedir
    );

    FVector forceUnscaled(
        FVector &legdir_featherCurrent,
        FVector &movedir,
        bool allowSlipSineScalar
    );

    float slipSineScalar(
        const FVector &legdir_featherCurrent,
        FVector &movedir
    );

    float featherComplete; //ausrichtung scheint eine rolle zu spielen
    FVector featherCurrent;


    bool wasSetup = false;

    bool debugSkipToCloseframes(FVector a, FVector b);

public:
    void setupInterpolatedD(
        FVector &endA, //start LOCAL, With rotation
        FVector &endB, //lift off, LOCAL, with rotation
        FVector &movedir,
        float time,
        float velocity,
        float mass
    );
    void setupInterpolatedD(
        FVector &endA, // start LOCAL, With rotation
        FVector &endB, // lift off, Local, with rotation
        FVector &movedir,
        float B1,
        float F1,
        float B2,
        float velocityDown,
        float mass
    );

    FVector velocityInterpolated(
        float deltatime,
        float mass,
        FVector &movedir
    );


    //new.
    FVector StaticSlipVelocity(
        FVector &moveDir,
        FVector &currentFootRelative,
        bool isGrounded,
        float velocityDown,
        float mass,
        float deltatime,
        bool isInStance
    );

private:
    //new
    FVector A;
    FVector B;
    
    float timeForInterpolation;
    float Dcurrent = 1.0f; //feder konstante D
    
    //D limit for prevention of bad values
    float DLimit = 9999999999.0f;
    bool bDLimitSetup = false;
    void validateD(float &dIn);

    bool slipForceAllowedDotProductUp(FVector &leg);


   
    float avoidDivisionByZero(float value);

    FVector forceIntegrated(
        float time,
        float deltaTime,
        FVector &moveDir,
        FVector &a,
        FVector &b
    );

    FVector forceIntegrated(
        float B1,
        float F1,
        float B2,
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

    SlipForceCache forceCache;
};