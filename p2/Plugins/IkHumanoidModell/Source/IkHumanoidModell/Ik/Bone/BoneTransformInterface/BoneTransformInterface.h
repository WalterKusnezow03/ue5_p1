#pragma once


#include "CoreMinimal.h"


/// interface to allow a bone scene component to update
/// its transform on actor tick, by limb id

class IKHUMANOIDMODELL_API IBoneTransformInterface {

public:
    virtual bool GetTransform(
        FVector &location,
        FRotator &rotation,
        int limbId) = 0;

protected:
    virtual bool IdIsValid(int id) = 0;
};