#pragma once

#include "CoreMinimal.h"

#include "CoreMath/Matrix/MMatrix.h"
#include "PlueckerCore/Interface/IJointInterface.h"


class FHumanoidControllerSetupPackage;

/// @brief controls two layered two bones to create a torso
/// will also apply constrains so a torso split doesnt occur
/// and the other torso part gets moved if a layered Ik case occurs
class IKHUMANOIDMODELL_API HeadController : public IJointInterface {


protected:
    bool autoCreateLimbs = true;
    UWorld *worldPointer = nullptr;

public:

    HeadController();
    ~HeadController();

    void setup(FHumanoidControllerSetupPackage &setupPackage);

    void Tick(
        const MMatrix &actorTranslation,
        const MMatrix &actorRotation,
        float deltatime
    );

    void getActors(TArray<AActor *> &outArray);

    //connect to spine
    Joint *GetTopJoint();

    virtual void ReactToDamage(const FCustomHitResult &hitResult) override;

    void SetHeadRotation(const FRotator &rotation);

protected:
    MMatrix neckTranslation;
    MMatrix headTranslation;
    MMatrix headRotation;
    MMatrix neckRotation;

    Joint p1; //neck
    Joint p2; //head

    AActor *neckActor = nullptr;
    AActor *headActor = nullptr;

    void setupJoints(
        FSingleLimbProperty &head,
        FSingleLimbProperty &neck
    );
    void setupNeckJoints(float height);
    void setupHeadJoint(float height);
    void SetupJointParents();
    void SetupActorLimbs(
        FSingleLimbProperty &headProperty,
        FSingleLimbProperty &neckProperty
    );

    void attachLimbs(AActor *top, AActor *bottom);
    void applyTransformToActors(MMatrix &world, MMatrix &top, MMatrix &bottom);
    void ReactToDamage(
        const FCustomHitResult &hitResult,
        Joint &affectedJoint
    );
};
