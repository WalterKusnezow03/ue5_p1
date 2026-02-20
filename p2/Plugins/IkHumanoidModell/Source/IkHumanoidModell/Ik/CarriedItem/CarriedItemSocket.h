#pragma once

#include "CoreMinimal.h"
#include "IkHumanoidModell/carryItems/Interface/IkCarryInterface.h"
#include "CoreMath/Matrix/MMatrix.h"

class IKHUMANOIDMODELL_API CarriedItemSocket {

public:
    CarriedItemSocket();
    virtual ~CarriedItemSocket();

    void attachOrReplaceCarriedItem(IIkCarryInterface *itemIn);
    void dropCarriedItem();

    virtual void Tick(
        float deltatime,
        MMatrix &translation,
        MMatrix &orientation //orientation of actor or even combined with limb or camera look direction.
    );

    virtual void Tick(
        float deltatime,
        MMatrix &translation,
        MMatrix &orientationRaw, //orientation of skelleton
        FRotator &orientationCamera //orientation of camera
    );

    /// @brief local translation when updating the socket and carried item position and rotation
    /// as (M = T * R <--R doesnt affect translation)
    /// @param offset 
    void setLocalLocation(FVector &offset);

    /// @brief sets the local rotation as (M = T * R <--R doesnt affect translation)
    /// @param rotation 
    void setLocalRotation(FRotator &rotation);

    IIkCarryInterface *attachedItemPointer();

protected:
    //just as the method name sais.
    MMatrix AddTemporaryIIkCarryInterfaceAnimationOffsetToInnerTransformOnTick();

    IIkCarryInterface *attachedItem = nullptr;

    /// @brief M = T * R <--R doesnt affect translation when multiplying, direction writing to 
    /// translation and rotation
    MMatrix TransformInner;

    //TODO IN MIND:
    //might need an interpolator to switch between states.

    bool itemIsAttached();
};