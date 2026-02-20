#pragma once

#include "CoreMinimal.h"
#include "CoreMath/animation/Asset/KeyFrameAsset.h"
#include "IkHumanoidModell/Ik/Controller/TorsoController/EArmType.h"
#include "CoreMath/animation/Asset/KeyFrameAssetArray.h"
#include "IkHumanoidModell/carryItems/Interface/EmptyActor/AxisConstraint/IKCarryInterfaceAxisConstraint.h"
#include "ArmKeyFrameAnimationAsset.generated.h"

class KeyFrameAnimation;

/// @brief will store arm animations, relative to hip!
/// recommended: flag position constraint not allowed
/// for pure animations relatie to hip!
/// (see p2 implementation)
UCLASS(BlueprintType)
class COREMATH_API UArmKeyFrameAnimationAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public: 

    //animation map.
    UPROPERTY(EditAnywhere, Category="KeyframeAnimationOption")
    TMap<EArmType, FKeyFrameAssetArray> KeyFrameAnimations;

    //shared loop animation flag
    UPROPERTY(EditAnywhere, Category="KeyframeAnimationOption")
    bool loopAnimation = true;

    //rotation axis restraint for carried object / local animation
    UPROPERTY(EditAnywhere, Category="KeyframeAnimationOption")
    FIKCarryInterfaceAxisConstraint axisConstraintForObject;

    void CopyDataTo(EArmType type, KeyFrameAnimation &animationIn) const;

    TArray<EArmType> GetKeyList() const;


};