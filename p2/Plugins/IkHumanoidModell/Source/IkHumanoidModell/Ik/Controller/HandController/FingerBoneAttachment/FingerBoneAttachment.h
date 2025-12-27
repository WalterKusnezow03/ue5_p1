#pragma once

#include "IkHumanoidModell/Ik/Controller/HipController/BoneAttachment.h"
#include "IkHumanoidModell/Ik/Controller/HandController/gesture/FingerTargetPair.h"


class IKHUMANOIDMODELL_API FingerBoneAttachment : public BoneAttachment {

public:
    FingerBoneAttachment();
    virtual ~FingerBoneAttachment();

    void OverrideTarget(FingerTargetPair &targetIn);

    void TickAutoBasedOnTarget(MMatrix &transform, float deltatime);

private:
    FingerTargetPair target;
};