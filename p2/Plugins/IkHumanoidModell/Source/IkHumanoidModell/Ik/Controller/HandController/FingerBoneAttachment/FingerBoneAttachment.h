#pragma once

#include "IkHumanoidModell/Ik/Controller/HipController/BoneAttachment.h"
#include "IkHumanoidModell/Ik/Controller/HandController/gesture/FingerTargetPair.h"
#include "IkHumanoidModell/Ik/Controller/ControllerSetup/Properties/FHandFingerProperty.h"


class IKHUMANOIDMODELL_API FingerBoneAttachment : public BoneAttachment {

public:
    FingerBoneAttachment();
    virtual ~FingerBoneAttachment();

    void OverrideTarget(FingerTargetPair &targetIn);

    void TickAutoBasedOnTarget(MMatrix &transform, float deltatime);

    using BoneAttachment::setupBone;
    void setupBone(FHandFingerProperty &property);

private:
    FingerTargetPair target;
};