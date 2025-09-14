#include "AnimationTime.h"
#include "GameCore/DebugHelper.h"

float AnimationTime::AnimationTimeBasedOnHorizontalVelocity(
    const FVector &localStart,
    const FVector &localEnd,
    float horizontalVelocity
){
    
    //vTarget = v
    //mGiven / x = vTarget
    //mGiven = vTarget * x
    //mGiven / vTarget = x

    float m = FVector::Dist(localStart, localEnd);
    float v = horizontalVelocity;

    float epsilonLower = 0.001f;
    v = std::max(v, epsilonLower);


    float time = m / v;
    
    float tEpislonLower = 0.001f;
    time = std::max(tEpislonLower, time);

    float tEpsilonHigher = 1.0f;
    time = std::min(tEpsilonHigher, time);

    DebugHelper::showScreenMessage("hipcontroller dynamic time backward ", (float)time);
    DebugHelper::logMessage(FString::Printf(TEXT("hipcontroller dynamic time backward %.2f"), time));
    return time;
}



float AnimationTime::AnimationTimeBasedOnHorizontalAndVerticalVelocity(
    const FVector &localStart,
    const FVector &localEnd,
    float horizontalVelocity,
    float verticalVelocity
){

    /*
    t_{1,2} =
    \frac
    {
    -(v_{0z})+- \sqrt{(v_{0z})^2 - 2(a_{z})(p_{1z} - p_{0z})}
    }
    {
    a_{z}
    }
    */
    float tVertical = 0.0f;
    bool tVerticalResultValid = false;
    float deltaP = localEnd.Z - localStart.Z;
    float a = -981.0f;
    float innerSqrt = (verticalVelocity * verticalVelocity) - (2 * a * deltaP);
    if(innerSqrt > 0.0f && innerSqrt < 1000000.0f){
        float sqrt = std::sqrt(innerSqrt);
        float t1 = (-verticalVelocity + sqrt) / a;
        float t2 = (-verticalVelocity - sqrt) / a;

        t1 = std::max(t1, 0.0f);
        t2 = std::max(t2, 0.0f);
        tVertical = std::max(t1, t2);

        tVerticalResultValid = true;

        DebugHelper::showScreenMessage("hipcontroller dynamic time forward gravity ", float(tVertical));
        DebugHelper::logMessage(FString::Printf(
            TEXT("hipcontroller dynamic time forward gravity %.2f"), 
            tVertical
        ));
    }

    float tHorizontal = AnimationTimeBasedOnHorizontalVelocity(
        localStart,
        localEnd,
        horizontalVelocity
    );

    float tResultOutput = tHorizontal;
    if (tVerticalResultValid)
    {
        if(tHorizontal > tVertical){
            tResultOutput = tVertical;
        }
    }
    tResultOutput = std::min(tResultOutput, 3.0f);

    DebugHelper::logMessage(FString::Printf(
        TEXT("hipcontroller dynamic time forward gravity and horizontal output %.2f"), 
        tResultOutput
    ));


    return tResultOutput;
}