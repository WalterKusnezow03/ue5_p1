#pragma once

#include "CoreMinimal.h"
#include "PlueckerCore/Interface/FJointKinematicPropagatePackage.h"

//might have more than one joint to propagate
class PLUECKERCORE_API IJointInterface {

public: 

    //Todo: Implement Impulse Propagation!


    // propagate angualar and linear veloicty
    virtual void UpstreamPropagate(FJointKinematicPropagatePackage &package) {};

    virtual void DownstreamPropagate(FJointKinematicPropagatePackage &package) {};

    

    //set parent interface for upstream propagation
    void SetParentInterface(IJointInterface *parent){
        parentInterface = parent;
    }

    void ResetParentInterface(){
        parentInterface = nullptr;
    }

    bool HasParentInterface(){
        return parentInterface != nullptr;
    }

    virtual void SetStateCollapse(bool flag){
        collapseEnabledFlag = flag;
    }
    

protected:
    IJointInterface *parentInterface = nullptr;
    bool collapseEnabledFlag = false;
    // external impulse (?)
};