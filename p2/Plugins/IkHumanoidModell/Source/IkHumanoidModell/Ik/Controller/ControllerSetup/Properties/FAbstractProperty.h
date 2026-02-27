#pragma once

class IKHUMANOIDMODELL_API FAbstractProperty {

public:
    float Validate(float size){
        return std::max(std::abs(size), 1.0f);
    }

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