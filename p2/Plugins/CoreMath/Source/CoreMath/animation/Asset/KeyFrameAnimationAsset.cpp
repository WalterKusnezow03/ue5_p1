#include "KeyFrameAnimationAsset.h"
#include "CoreMath/animation/KeyFrames/KeyFrameAnimation.h"





void UKeyFrameAnimationAsset::CopyDataTo(KeyFrameAnimation &animationIn) const {
    //copy keyframes
    animationIn.addAll(keyframes);

    //deprecated
    /*
    for (int i = 0; i < keyframes.Num(); i++){
        const FKeyFrameAsset &current = keyframes[i];
        animationIn.addFrame(
            current.Position, 
            current.TimeToFrame, 
            current.bMustBeGrounded
        );
    }*/

    //set looping animation flag
    animationIn.SetLoopFlag(loopAnimation);

}