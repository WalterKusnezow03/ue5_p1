#pragma once

#include "CoreMinimal.h"
#include "FAbstractProperty.h"
#include "FSingleLimbProperty.h"

class IKHUMANOIDMODELL_API FTwoLimbProperty : public FAbstractProperty{

public:
    virtual void SetActor(AActor *actor) override {
        FAbstractProperty::SetActor(actor);
        firstProperty.SetActor(actor);
        secondProperty.SetActor(actor);
    }

    float GetSizeFirst(){
        return firstProperty.GetSize();
    }

    float GetSizeSecond(){
        return secondProperty.GetSize();
    }

    float GetFirstAndSecondSize(){
        return GetSizeFirst() + GetSizeSecond();
    }

    float GetWidth(){
        return firstProperty.GetWidth();
    }

    FSingleLimbProperty &GetFirstProperty(){
        return firstProperty;
    }
    FSingleLimbProperty &GetSecondProperty(){
        return secondProperty;
    }

    void Setup(float a, float b){
        Setup(a, b, 10);
    }

    void Setup(float a, float b, float desiredWidth){
        firstProperty.Setup(a, desiredWidth);
        secondProperty.Setup(b, desiredWidth);
    }

protected:
    
    FSingleLimbProperty firstProperty;
    FSingleLimbProperty secondProperty;
};
