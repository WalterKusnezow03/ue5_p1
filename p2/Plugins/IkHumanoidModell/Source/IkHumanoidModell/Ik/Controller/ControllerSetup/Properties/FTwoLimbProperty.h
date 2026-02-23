#pragma once

#include "CoreMinimal.h"
#include "FAbstractProperty.h"

class IKHUMANOIDMODELL_API FTwoLimbProperty : public FAbstractProperty{

public:
    
    
    float GetSizeFirst(){
        return sizeFirst;
    }

    float GetSizeSecond(){
        return sizeSecond;
    }

    float GetFirstAndSecondSize(){
        return GetSizeFirst() + GetSizeSecond();
    }

    float GetWidth(){
        return widthOptional;
    }



    

    void Setup(float a, float b){
        sizeFirst = Validate(a);
        sizeSecond = Validate(b);
    }

    void Setup(float a, float b, float desiredWidth){
        Setup(a, b);
        widthOptional = Validate(desiredWidth);
    }

    

protected:
    float sizeFirst = 1.0f;
    float sizeSecond = 1.0f;
    float widthOptional = 1.0f;

    
};
