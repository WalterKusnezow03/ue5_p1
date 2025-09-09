#include "SlipLiftOffFrameFinder.h"
#include "CoreMath/Matrix/MMatrix.h"

SlipLiftOffFrameFinder::SlipLiftOffFrameFinder(){

}


SlipLiftOffFrameFinder::~SlipLiftOffFrameFinder(){

}


void SlipLiftOffFrameFinder::LogResult(
    FString prefix,
    float closerTSkalar,
    FVector &localResult //local space to hip
){
    float distanceHipLiftOff = FVector::Dist(FVector(0,0,0), localResult);

    FString message = TEXT("SlipLiftOffFrameFinder Result: ");
    message += prefix;
    message += FString::Printf(TEXT("tScalar(%.2f)"), closerTSkalar);
    message += TEXT("Lift off frame local: ");
    message += localResult.ToString();
    message += FString::Printf(TEXT("Distance: %.2f"), distanceHipLiftOff);
    

    DebugHelper::logMessage(message);
}

void SlipLiftOffFrameFinder::LogResultLocalSpace(
    FString prefixInner,
    FVector &direction,
    float closerTSkalar,
    FVector &localResult //local space to hip
){
    //just yaw derotation
    FVector dir2D(direction.X, direction.Y, 0.0f);
    MMatrix R;
    R.setRotation(dir2D);
    MMatrix R1 = R.transposedRotation();

    FVector resultNoRotation = R1 * localResult;

    FString prefix = FString::Printf(TEXT("Result DeRotated: %s"), *prefixInner);

    LogResult(
        prefix,
        closerTSkalar,
        resultNoRotation // local space to hip
    );
}




void SlipLiftOffFrameFinder::Test(){
    SlipLiftOffFrameFinder finder;
    FVector worldCurrentFootPosTarget(0, 0, 0); // Target of Leg 1, currently for forward motion reached!
    FVector worldNextFootPosTarget(60, 0, 0);  // Target of Leg 2, next world Trajectory
    float boneLength = 100;
    float epsilonHeelOff = 10.0f; // 10.0f;


    //debug
    DebugHelper::logMessage("SlipLiftOffFrameFinder TEST -2- STARTED");
    worldCurrentFootPosTarget = FVector(0, 0, 0); // Target of Leg 1, currently for forward motion reached!
    worldNextFootPosTarget = FVector(60, 0, 0);  // Target of Leg 2, next world Trajectory
    for (int i = 0; i < 10; i++){
        worldCurrentFootPosTarget.X += 60;
        worldNextFootPosTarget.X += 60;
        finder.FindRelativeLiftOffFrameBasedOnHeight(
            worldCurrentFootPosTarget, 
            worldNextFootPosTarget, 
            boneLength, 
            epsilonHeelOff
        );
    }
    DebugHelper::logMessage("SlipLiftOffFrameFinder TEST -2- ENDED");




    DebugHelper::logMessage("SlipLiftOffFrameFinder TEST -3- STARTED");
    worldCurrentFootPosTarget = FVector(0, 0, 0); // Target of Leg 1, currently for forward motion reached!
    worldNextFootPosTarget = FVector(60, 0, 0);  // Target of Leg 2, next world Trajectory
    for (int i = 0; i < 10; i++){
        worldCurrentFootPosTarget.X += 60;
        worldNextFootPosTarget.X += 60;
        worldNextFootPosTarget.Z += worldCurrentFootPosTarget.Z;

        worldNextFootPosTarget.Z += 2 * i; //some step up.
        finder.FindLiftOffFrameRelativeLocalToNextHipFromWorldTrajectories(
            worldCurrentFootPosTarget, 
            worldNextFootPosTarget, 
            boneLength, 
            epsilonHeelOff
        );
    }
    DebugHelper::logMessage("SlipLiftOffFrameFinder TEST -3- ENDED");



    
}

FVector SlipLiftOffFrameFinder::FindRelativeLiftOffFrameBasedOnHeight(
    FVector &worldFootA,
    FVector &worldFootB,
    float lengthOfBone,
    float heelOffEpsilon
){
    FVector A0 = worldFootA;
    FVector B0 = worldFootB;
    /*
    $$
    B_0hip = B_0 + (0,0,l_0)
    $$
    $$
    g(t)_{floor} = A_0 + t(B_0-A_0)
    $$
    Gesucht ist \(t\) für die Gereade \(g(t)\), sodass die Vertikale distanz kleiner ist als das heel off epsilon:
    $$
    B_0hip_z - g(t)_z <= \epsilon
    $$
    $$
    B_0hip_z - A_{0z} + t(B_0-A_0)_z <= \epsilon
    $$
    $$
    t <= \frac{\epsilon - B_0hip_z + A_{0z}}{(B_0-A_0)_z}
    $$
    */
    FVector localHip = FVector(0, 0, lengthOfBone);
    FVector B0Hip = worldFootB + localHip;
    FVector AB = B0 - A0;

    float nominator = heelOffEpsilon - B0Hip.Z + A0.Z;
    float denominator = B0.Z - A0.Z;
    float tMin = 0.0f;
    if (std::abs(denominator) <= 1.0f)
    {
        //local hip - dir: local lift off
        FVector result = -localHip - AB; //nach unten hinten
        tMin = 1.0f;
    }else{
        tMin = nominator / denominator;
    }

    //float tMin = nominator / denominator;
    FVector result = -localHip - AB * tMin; //nach unten hinten

    LogResult(
        TEXT("Result Based on Z height->t "),
        tMin,
        result
    );

    return result;
}



FVector SlipLiftOffFrameFinder::FindLiftOffFrameRelativeLocalToNextHipFromWorldTrajectories(
    FVector &worldCurrentFootPosTarget, // A0 (current foot target / "A_1" in your notes)
    FVector &worldNextFootPosTarget,    // B0 (next foot target)
    float boneLength,
    float heelOffEpsilon /* = 10.0f */   // make epsilon configurable
){
    FVector A0 = worldCurrentFootPosTarget;
    FVector B0 = worldNextFootPosTarget;
    FVector v = B0 - A0;
    float l0 = boneLength;
    FVector h(0, 0, l0);
    h *= -1.0f; //This must stay here to invert to hip to foot space

    float l = l0 + heelOffEpsilon;

    /*
    Definitionen:
    $$
    l_0 = lengthOfBone
    \vec{h} = (0,0, l_0)
    v = (B_0- A_0);
    l = l_0 + \epsilon
    $$
    */

    /*
    $$
    PQ RAW - issues. many. -
    t_{1,2} = \frac{hv}{v^2} +-
    \sqrt{
    (\frac{-hv}{v^2})^2 - \frac{h^2 + l^2}{v^2}
    }
    $$

    $$
    t_{result} = min(t,t_1, t_2) ???
    $$
    */

    /*
    //\frac{hv}{v^2}
    //(\frac{-hv}{v^2})^2
    //\frac{h^2 + l^2}{v^2}
    float hCdotV = FVector::DotProduct(h, v);
    float VCdotV = FVector::DotProduct(v, v);
    float HCdotH = FVector::DotProduct(h, h);
    float lSquared = l * l;

    //early quit here.
    if(std::abs(VCdotV) < 0.0001f){
        //invalid
        return A0;
    }

    float p = hCdotV / VCdotV;
    float pSquared = p * p;
    float q = (HCdotH + lSquared) / VCdotV;

    float sqrtInner = pSquared - q;
    if(sqrtInner < 0.0001f){
        //invalid
        return A0;
    }

    //early quit here
    float sqrt = std::sqrt(sqrtInner);

    float t1 = p + sqrt;
    float t2 = p - sqrt;

    float tResult = std::min(t1, t2);
    */

    /*
    PQ Common
    $$
    \frac
    {
    2hv +- \sqrt{(2hv)^2 - 4\cdot v^2 \cdot (h^2 - l^2)}
    }
    {
    2 \cdot v^2
    }
    $$
    */
    float hCdotV_times2 = 2.0f * FVector::DotProduct(h, v);
    float hCdotV_times2squared = hCdotV_times2 * hCdotV_times2;

    float VCdotV = FVector::DotProduct(v, v);
    float HCdotH = FVector::DotProduct(h, h);
    float lSquared = l * l;

    float sqrtInner = hCdotV_times2squared - 4 * VCdotV * (HCdotH - lSquared);
    float nominator1 = hCdotV_times2 + std::sqrt(sqrtInner);
    float nominator2 = hCdotV_times2 - std::sqrt(sqrtInner);
    float denominator = 2 * VCdotV;

    float t1 = nominator1 / denominator;
    float t2 = nominator2 / denominator;

    //float tResult = std::min(t1, t2);

    //liftoff = \vec{h} - t * v
    FVector result1 = h - t1 * v;
    FVector result2 = h - t2 * v;

    //result position muss hinter forward richtung liegen / anti paralell!
    FVector resultA2D(result1.X, result1.Y, 0.0f);
    FVector resultB2D(result2.X, result2.Y, 0.0f);

    FVector dir2D(v.X, v.Y, 0.0f);

    //das was negativ ist gewinnt
    float dot1 = FVector::DotProduct(resultA2D, dir2D);
    float dot2 = FVector::DotProduct(resultB2D, dir2D);

    float tResult = dot1 < dot2 ? t1 : t2;
    FVector result = dot1 < dot2 ? result1 : result2;
    
    float tResultOther = dot1 > dot2 ? t1 : t2;
    FVector resultOther = dot1 > dot2 ? result1 : result2;


    float debugStepHeight = v.Z;
    LogResult(
        FString::Printf(TEXT("Result Based New Equation (stepheight: %.2f)"), debugStepHeight),
        tResult,
        result
    );


    LogResult(
        TEXT("Other Result Based New Equation (stepheight: %.2f)"),
        tResultOther,
        resultOther
    );

    LogResultLocalSpace(
        TEXT("Result Based New Equation Additional Debug: "),
        v, // FVector &direction,
        tResult,
        result // local space to hip
    );

    return result;
}
