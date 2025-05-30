#pragma once

#include "CoreMinimal.h"
#include "CoreMath/Matrix/MMatrix.h"

/**
 * copied from my abstract kinematic Bipedal locomotion skelleton.
 */
class HUMANOID_API TwoBoneGeometricSolve {

public:
    static void createPitchAnglesFor(
        float distanceTotarget, 
        float &firstOuput, 
        float &secondOutput, 
        bool forwardIK,
        MMatrix &middle,
        MMatrix &end
    ){
        float m1 = middle.getTranslation().Size();
        float m2 = end.getTranslation().Size();
        createPitchAnglesFor(
            distanceToTarget,
            firstOuput,
            secondOutput,
            forwardIK,
            m1,
            m2
        );
    }

    static void createPitchAnglesFor(
        float distanceTotarget, 
        float &firstOuput, 
        float &secondOutput, 
        bool forwardIK,
        float translationToMiddle,
        float translationToEnd
    ){
        
        float _c = std::abs(distanceTotarget);
        float _b = std::abs(middle.getTranslation().Size());
        float _a = std::abs(end.getTranslation().Size());
    
        float a2 = _a * _a;
        float b2 = _b * _b;
        float c2 = _c * _c;
    
        /*
        float alpha = std::acosf(((b2 + c2 - a2) / (2 * _b * _c)));
        float beta = std::acosf(((a2 + c2 - b2) / (2 * _a * _c)));
        float gamma = std::acosf(((a2 + b2 - c2) / (2 * _a * _b)));
        */
       
    
        float alpha = std::acosf(FMath::Clamp((b2 + c2 - a2) / (2 * _b * _c), -1.0f, 1.0f));
        float beta = std::acosf(FMath::Clamp((a2 + c2 - b2) / (2 * _a * _c), -1.0f, 1.0f));
        float gamma = std::acosf(FMath::Clamp((a2 + b2 - c2) / (2 * _a * _b), -1.0f, 1.0f));
    
    
    
        firstOuput = -1 * alpha;
        //secondOutput = alpha * 2; //*-2 um den winkel einfach zu flippen bei gleichgrossen knochen
        
        //debug disable
        gamma = MMatrix::degToRadian(180.0f - std::abs(MMatrix::radToDegree(gamma)));
        secondOutput = gamma;
    
        /*FString debugAngleString = FString::Printf(
            TEXT("AngleDebug alpha HIP %.2f, gamma KNEE %.2f"),
            MMatrix::radToDegree(alpha),
            MMatrix::radToDegree(gamma)
        );*/
    
    
    }

    TwoBoneGeometricSolve() {};
    ~TwoBoneGeometricSolve() {};

private:

};