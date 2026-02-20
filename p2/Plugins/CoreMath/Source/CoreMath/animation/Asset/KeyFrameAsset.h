#pragma once

#include "CoreMinimal.h"
#include "KeyFrameAsset.generated.h"

/// @brief struct to save keyframe properties
USTRUCT(BlueprintType)
struct FKeyFrameAsset
{
    GENERATED_BODY()

public:
    FKeyFrameAsset() {};

    UPROPERTY(EditAnywhere, Category="Keyframe")
    FVector Position = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, Category="Keyframe")
    FRotator Rotation = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere, Category="Keyframe")
    float TimeToFrame = 0.0f;

    UPROPERTY(EditAnywhere, Category="Keyframe")
    bool bMustBeGrounded = false;
};