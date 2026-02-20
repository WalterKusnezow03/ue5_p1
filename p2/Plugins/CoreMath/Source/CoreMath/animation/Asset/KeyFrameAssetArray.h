#pragma once

#include "CoreMinimal.h"
#include "CoreMath/animation/Asset/KeyFrameAsset.h"
#include "KeyFrameAssetArray.generated.h"


USTRUCT(BlueprintType)
struct FKeyFrameAssetArray
{
    GENERATED_BODY()
public:
    FKeyFrameAssetArray() {};

    UPROPERTY(EditAnywhere, Category="Keyframe")
    TArray<FKeyFrameAsset> Frames;

    UPROPERTY(EditAnywhere, Category="Keyframe")
    bool loopToFirst = false;

    bool CopyAt(int index, FKeyFrameAsset &outCopy) const {
        if(index >= 0 && index < Frames.Num()){
            outCopy = Frames[index];
            return true;
        }
        return false;
    }
};