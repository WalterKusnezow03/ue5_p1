#pragma once

#include "CoreMinimal.h"

/// @brief tracks a local target of a finger and enabled / disabled state
class IKHUMANOIDMODELL_API FingerTargetPair {

public:
    FingerTargetPair();
    ~FingerTargetPair();

    FingerTargetPair(const FingerTargetPair &other);
    FingerTargetPair &operator=(const FingerTargetPair &other);

    void OverrideTarget(FVector &target);

    bool bIsEnabled();
    void enable(bool flag);

    FVector &GetTarget();

private:
    FVector localTarget;
    bool bIsEnabledFlag = false;
};