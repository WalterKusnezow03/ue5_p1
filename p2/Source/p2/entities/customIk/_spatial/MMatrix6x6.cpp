#include "MMatrix6x6.h"
#include "MMatrix3x3.h"
#include "FVector6D.h"


MMatrix6x6::MMatrix6x6(){

}
MMatrix6x6::~MMatrix6x6(){

}

MMatrix6x6::MMatrix6x6(const MMatrix6x6 &other){

}

MMatrix6x6 MMatrix6x6::operator*(MMatrix6x6 &other){
    
    MMatrix6x6 result;
    
    MMatrix3x3 r1xr2 = rotationSaved * other.rotationSaved;
    MMatrix3x3 r1skew2 = rotationSaved * other.translationSkewMatrix;
    MMatrix3x3 s1r1skew2 = translationSkewMatrix + r1skew2;

    result.translationSkewMatrix = s1r1skew2;
    result.rotationSaved = r1xr2;
    return result;
}


MMatrix6x6 &MMatrix6x6::operator=(const MMatrix6x6 &other){
    if(this != &other){
        rotationSaved = other.rotationSaved;
        translationSkewMatrix = other.translationSkewMatrix;
    }
    return *this;
}

//main traverse
FVector6D MMatrix6x6::operator*(FVector6D &other){//traverses

    /*
    
    VA = XAB * VB //VA und VB jeweils FVector6D

    VA = |R          0| * |w| //eigen rotation velocity
         |tSkew * R  R|   |v| //velocity
    
    */
    FVector6D VAOut;


    /*
         |x|
         |y|
    |a b| ax + by
    |c d| cx + dy

    */
    //todo: implementieren!
    FVector x = other.getRotationVelocity();
    FVector y = other.getVelocity();
    FVector ax = rotationSaved * x;

    MMatrix3x3 tskewR = skewTranslationMulitpliedWithRotation();
    FVector cx = tskewR * x;
    FVector dy = rotationSaved * y;

    FVector axby_rotationVelocity = ax;
    //+by;
    VAOut.updateRotationVelocity(axby_rotationVelocity);

    FVector cxdy_linearVelocity = cx + dy;
    VAOut.updateLiniearVelocity(cxdy_linearVelocity);

    //show change
    DebugHelper::logMessage("updated liniearvelocity ", cxdy_linearVelocity);

    return VAOut;
}

/// @brief tSkew * R
/// @return 
MMatrix3x3 MMatrix6x6::skewTranslationMulitpliedWithRotation(){
    MMatrix3x3 tskewR = translationSkewMatrix * rotationSaved;
    return tskewR;
}

//bone control 
void MMatrix6x6::setTranslation(FVector other){
    translationSkewMatrix = MMatrix3x3::skewMatrix(other);
    bottomLeft = skewTranslationMulitpliedWithRotation();   
}

void MMatrix6x6::setRotation(FRotator &other){
    MMatrix3x3 rotMat(other);
    setRotation(rotMat);
}

void MMatrix6x6::setRotation(MMatrix3x3 &other){
    rotationSaved = other;

    bottomLeft = skewTranslationMulitpliedWithRotation();
}