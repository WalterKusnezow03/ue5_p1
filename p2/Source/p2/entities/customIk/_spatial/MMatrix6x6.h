#pragma once

#include "CoreMinimal.h"
#include "MMatrix3x3.h"
#include "FVector6D.h"

class P2_API MMatrix6x6 {

public:
    MMatrix6x6();
    ~MMatrix6x6();

    MMatrix6x6(const MMatrix6x6 &other);
    MMatrix6x6 &operator=(const MMatrix6x6 &other);
    MMatrix6x6 operator*(MMatrix6x6 &other);

    FVector6D operator*(FVector6D &other); //traverses

    //bone control 
    void setTranslation(FVector other);
    void setRotation(FRotator &other);
    void setRotation(MMatrix3x3 &other);

private:
    MMatrix3x3 rotationSaved;
    MMatrix3x3 translationSkewMatrix;

    MMatrix3x3 bottomLeft;

    MMatrix3x3 skewTranslationMulitpliedWithRotation();
};