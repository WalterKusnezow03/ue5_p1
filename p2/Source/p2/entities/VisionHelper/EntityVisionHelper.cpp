#include "EntityVisionHelper.h"
#include "DebugPlugin/DebugHelper.h"

FEntityVisionHelper::FEntityVisionHelper(){

}

FEntityVisionHelper::~FEntityVisionHelper(){

}


void FEntityVisionHelper::setup(float visionAngleDegreeMaxIn){
    visionAngleDegree = std::abs(visionAngleDegreeMaxIn);
    visionAngleRad = FMath::DegreesToRadians(visionAngleDegree);

    //half angle for dot product! orthogonal: pi / 2, nicht pi!
    float halfRad = visionAngleRad / 2.0f; //for dot correct calculation

    //cos(theta) = a dot b //wenn a und b normalisiert sind.
    visionAngleDotMin = std::cos(halfRad);

    //wenn das skalarprodukt zweier vektoren 0 ergibt sind sie orthogonal zu einander
    //wenn das skalarprodukt zweier normalisierter vektoren 1 ergibt, sind sie paralell
    //zu einander 
    //wenn das skalarprodukt zweier normalisierter vektoren -1 ergibt, sind sie anti paralell
    //zu einander 


    DebugHelper::logMessage(
        FString::Printf(
            TEXT("FEntityVisionHelper::Setup deg %.2f  rad %.2f  dotMin %.2f"),
            visionAngleDegree,
            visionAngleRad,
            visionAngleDotMin
        )
    );
    //eample
    //FEntityVisionHelper::Setup deg 180.00  rad 3.14  dotMin -0.00
    // dot product must be greater than 0.0, which is a
    // expected and very good number.

}

float FEntityVisionHelper::VisionAngleDegree(){
    return visionAngleDegree;
}

bool FEntityVisionHelper::DotProductWithinVisionAngle(float dotIn){
    return dotIn >= visionAngleDotMin; //larger than min Dot Product
}

float FEntityVisionHelper::VisionAngleDot(){
    return visionAngleDotMin;
}
