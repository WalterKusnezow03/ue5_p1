#pragma once

#include "CoreMinimal.h"
#include "IkHumanoidModell/Ik/Bone/TwoJointBone.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "IkHumanoidModell/Ik/Controller/SLIP/SlipContainer.h"

class IKHUMANOIDMODELL_API BoneAttachment {

public:
    BoneAttachment();
    ~BoneAttachment();

    FVector defaultExtendedEndToStartLocal();
    void setupBone(float a, float b, UWorld *world, FVector offset);

    void setForwardTargetWorld(
        FVector &targetWorld,
        MMatrix &rootTranslation,
        MMatrix &rootOrientation
    );
    void setForwardTargetLocal(FVector &target);
    void setBackwardTargetLocal(FVector &target);

    void TickNone(MMatrix &worldRoot, float deltatime);
    void TickForwardKinematic(MMatrix &transform, float deltatime);
    void TickBackwardKinematic(
        MMatrix &translationRootToUpdate, 
        MMatrix &orientationRoot,
        float deltatime
    );
    void TickKeepEndInWorldPlace(
        MMatrix &translationRoot,
        MMatrix &orientationRoot,
        float deltatime
    );

    FVector inLocalSpace(
        FVector &worldFrame, 
        MMatrix &rootTranslation, 
        MMatrix &rootOrientation
    );
    FVector inWorldSpace(
        FVector &localFrame,
        MMatrix &rootTranslation,
        MMatrix &rootOrientation
    );
    FVector hipRelativeLocationToEndEffector(
        MMatrix &rootTranslation,
        MMatrix &rootOrientation
    );

    FVector endEffectorWorldLocation();

    SlipContainer slipData();

private:
    FVector extendedTranslationBottomToUp;

    MMatrix innerOffset;
    MMatrix innerOffsetInverse;

    TwoJointBone bone;

    FVector forwardTarget;
    FVector backwardTarget;

    MMatrix startEffectorTransformWorld(MMatrix &worldRoot);


};
