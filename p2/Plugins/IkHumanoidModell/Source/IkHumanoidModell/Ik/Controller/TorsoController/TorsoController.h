#pragma once

#include "CoreMinimal.h"
#include "LayeredTwoJointBone.h"
#include "CoreMath/Matrix/MMatrix.h"

class IIkCarryInterface;

/// @brief controls two layered two bones to create a torso
/// will also apply constrains so a torso split doesnt occur
/// and the other torso part gets moved if a layered Ik case occurs
class IKHUMANOIDMODELL_API TorsoController {

public:
    TorsoController();
    ~TorsoController();

    /// @brief will setup both layered ik bones symetrically
    /// @param hipBreast 
    /// @param breastShoulder 
    /// @param upperArm 
    /// @param lowerArm 
    /// @param world 
    void setup(
        float hipBreast,
        float breastShoulder,
        float upperArm,
        float lowerArm,
        UWorld *world
    );

    void Tick(
        MMatrix &actorTranslation,
        MMatrix &actorRotation,
        float deltatime
    );

    void SetWorldTargetLeft(FVector &target);
    void SetWorldTargetRight(FVector &target);

    void attachOrReplaceCarriedItem(IIkCarryInterface *ptr);
    void dropCarriedItem();

private:
    

    //unclear how but it must be somehow shared between the bones.
    //will maybe be copied every frame.
    MMatrix hipPitch;

    LayeredTwoJointBone partLeft;
    LayeredTwoJointBone partRight;
};