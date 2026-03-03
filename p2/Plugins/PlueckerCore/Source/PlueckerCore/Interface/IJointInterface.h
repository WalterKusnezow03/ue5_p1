#pragma once

#include "CoreMinimal.h"
#include "PlueckerCore/Interface/FJointKinematicPropagatePackage.h"
#include "GameCore/interfaces/DamageInterface/CustomHitResult.h"

//might have more than one joint to propagate
class PLUECKERCORE_API IJointInterface {

public: 

    //new simplified joint chains by ptr childs
    virtual Joint *GetTopJoint(){
        return nullptr;
    }

    //Todo: Implement Impulse Propagation!


    // propagate angualar and linear veloicty
    
    

    //force propagation, find closest joint / attached actor
    virtual void ReactToDamage(const FCustomHitResult &hitResult) {};

    
    

    virtual void SetStateCollapse(bool flag){
        collapseEnabledFlag = flag;
    }
    

protected:
    
    bool collapseEnabledFlag = false;
    // external impulse (?)
};