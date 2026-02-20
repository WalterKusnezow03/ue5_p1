#pragma once

#include "CoreMinimal.h"
#include "LayeredTwoJointBone.h"
#include "CoreMath/Matrix/MMatrix.h"

class IIkCarryInterface;
class AIKCarryInterfaceAnimatedActor;

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

    void defaultSetupHands(UWorld *worldIn);

    void ResetAndRebuild(
        MMatrix &actorTranslation,
        MMatrix &actorRotation
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

    //api for get actors:apply damaged owner casted mesh actor
    void getActors(TArray<AActor *> &outArray);


private:
    //arms
    LayeredTwoJointBone partLeft;
    LayeredTwoJointBone partRight;

    
};
