#pragma once

#include "CoreMinimal.h"
#include "LayeredTwoJointBone.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "PlueckerCore/Interface/IJointInterface.h"
#include "IkHumanoidModell/Ik/Controller/TorsoController/HeadController/HeadController.h"

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

    //connect hip
    Joint *GetTopJoint();


    void SetHeadRotation(const FRotator &rotation);
    void SetHeadRotationLookAt(const FVector &lookAt);

private:
    //connect spine to layered bones.
    TArray<Joint *> GetTopJointsOfLayeredArms();

public:
    //enable collapse physics
    virtual void SetStateCollapse(bool flag) override;


protected:
    //tick
    void BuildSpine(
        const MMatrix &actorTranslation,
        const MMatrix &actorRotation,
        float deltatime
    );

private:
    //spine
    TwoJointBone spine;
    FVector spineTarget;

    //arms from chest
    LayeredTwoJointBone partLeft;
    LayeredTwoJointBone partRight;

    //head
    HeadController headController;

    // ---- pluecker joints ----
public:


    virtual void ReactToDamage(const FCustomHitResult &hitResult) override;

protected:
    void SetupJointParents();
   







};
