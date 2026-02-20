#pragma once

#include "CoreMinimal.h"
#include "CoreMath/animation/Asset/KeyFrameAsset.h"
#include "KeyFrameAnimationAsset.generated.h"

class KeyFrameAnimation;

/// @brief will store a Keyframe Animation of FKeyFrame assets
/// compatible with "KeyFrameAnimation" class.
UCLASS(BlueprintType)
class COREMATH_API UKeyFrameAnimationAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category="KeyframeAnimationOption")
    TArray<FKeyFrameAsset> keyframes;

    UPROPERTY(EditAnywhere, Category="KeyframeAnimationOption")
    bool loopAnimation = true;

    void CopyDataTo(KeyFrameAnimation &animationIn) const;
    
};