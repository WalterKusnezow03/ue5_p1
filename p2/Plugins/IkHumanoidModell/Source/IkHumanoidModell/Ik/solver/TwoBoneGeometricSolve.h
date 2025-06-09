#pragma once

#include "CoreMinimal.h"
#include "CoreMath/Matrix/MMatrix.h"

/**
 * copied from my abstract kinematic Bipedal locomotion skelleton.
 */
class IKHUMANOIDMODELL_API TwoBoneGeometricSolve {

public:
    static void createPitchAnglesFor(
        float distanceToTarget, 
        float &firstOuput, 
        float &secondOutput,
        MMatrix &middle,
        MMatrix &end
    ){
        float m1 = middle.getTranslation().Size();
        float m2 = end.getTranslation().Size();
        createPitchAnglesFor(
            distanceToTarget,
            firstOuput,
            secondOutput,
            m1,
            m2
        );
    }

    static void createPitchAnglesFor(
        float distanceTotarget, 
        float &firstOuput, 
        float &secondOutput, 
        float translationToMiddle,
        float translationToEnd
    ){
        
        float _c = std::abs(distanceTotarget);
        float _b = std::abs(translationToMiddle);
        float _a = std::abs(translationToEnd);
    
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

        firstOuput = alpha * -1.0f;
        secondOutput = MMatrix::degToRadian(180.0f - std::abs(MMatrix::radToDegree(gamma)));

    }

    TwoBoneGeometricSolve() {};
    ~TwoBoneGeometricSolve() {};

private:

};