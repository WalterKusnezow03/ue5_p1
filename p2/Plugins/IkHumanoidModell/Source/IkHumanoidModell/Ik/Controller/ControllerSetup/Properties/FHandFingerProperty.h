#pragma once

#include "CoreMinimal.h"
#include "FTwoLimbProperty.h"

class IKHUMANOIDMODELL_API FHandFingerProperty : public FTwoLimbProperty{

public:
    void setup(FTwoLimbProperty &base, const FVector &pivotIn, AActor *actor){
        firstProperty = base.GetFirstProperty();
        secondProperty = base.GetSecondProperty();
        
        pivot = pivotIn;
        SetActor(actor);
    }

    FVector &GetPivot(){
        return pivot;
    }

private:
    FVector pivot;
};