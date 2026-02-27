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

    virtual void DownstreamPropagate(
        float deltatime,
        FVector &w, // angular velocity
        FVector &v,  // linear velocity
        MMatrix &world
    ) {};

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

protected:
    IJointInterface *parentInterface = nullptr;

    //external impulse (?)
};