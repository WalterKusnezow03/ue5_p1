#pragma once

#include "CoreMinimal.h"
#include "FTwoLimbProperty.h"

class IKHUMANOIDMODELL_API FHandFingerProperty : public FTwoLimbProperty{

public:
    void setup(FTwoLimbProperty &base, const FVector &pivotIn, UWorld *world){
        sizeFirst = base.GetSizeFirst();
        sizeSecond = base.GetSizeSecond();
        widthOptional = base.GetWidth();
        pivot = pivotIn;
        SetWorld(world);
    }

    FVector &GetPivot(){
        return pivot;
    }

private:
    FVector pivot;
};