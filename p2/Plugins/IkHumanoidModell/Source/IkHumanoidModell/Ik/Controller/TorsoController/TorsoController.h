#pragma once

#include "CoreMinimal.h"
#include "LayeredTwoJointBone.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "PlueckerCore/Interface/IJointInterface.h"

class IIkCarryInterface;
class AIKCarryInterfaceAnimatedActor;
class FHumanoidControllerSetupPackage;

/// @brief controls two layered two bones to create a torso
/// will also apply constrains so a torso split doesnt occur
/// and the other torso part gets moved if a layered Ik case occurs
class IKHUMANOIDMODELL_API TorsoController : public IJointInterface{

public:
    TorsoController();
    ~TorsoController();

    void setup(FHumanoidControllerSetupPackage &setupPackage);

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

    

    // ---- pluecker joints ----
public:
    virtual void UpstreamPropagate(FJointKinematicPropagatePackage &package) override;

    virtual void DownstreamPropagate(FJointKinematicPropagatePackage &package) override;

protected:
    void SetupJointParents();
    void DownstreamPropagateTo(
        LayeredTwoJointBone &attachment,
        FJointKinematicPropagatePackage &package
    );







};
