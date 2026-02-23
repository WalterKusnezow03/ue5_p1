#pragma once

#include "FTwoLimbProperty.h"


class IKHUMANOIDMODELL_API FTwoLimbHipProperty : public FTwoLimbProperty{

public:
    void SetHipOffset(float widthOffset){
        offsetSaved = widthOffset;
    }

    //x forward, y size
    FVector GetOffset(){
        FVector offset(0.0f, offsetSaved, 0.0f); // always on y axis
        return offset;
    }

private:
    float offsetSaved = 1.0f;
};