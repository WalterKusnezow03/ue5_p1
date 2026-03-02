#pragma once
#include "FAbstractPropertyBase.h"

class IKHUMANOIDMODELL_API FAbstractProperty : public FAbstractPropertyBase {

public:
    
    virtual void SetActor(AActor *ownerIn){
        actorOwner = ownerIn;
    }

    AActor *GetActor()const{
        return actorOwner;
    }

    UWorld *GetWorld()const{
        if(actorOwner){
            return actorOwner->GetWorld();
        }
        return nullptr;
    }

protected:
    AActor *actorOwner = nullptr;
};