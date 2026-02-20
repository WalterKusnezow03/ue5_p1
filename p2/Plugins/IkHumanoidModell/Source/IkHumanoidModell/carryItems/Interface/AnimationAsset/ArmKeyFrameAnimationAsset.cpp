#include "ArmKeyFrameAnimationAsset.h"


void UArmKeyFrameAnimationAsset::CopyDataTo(EArmType type, KeyFrameAnimation &animationIn) const {
    if(KeyFrameAnimations.Contains(type)){
        const FKeyFrameAssetArray* array = KeyFrameAnimations.Find(type);
        if(array){
            animationIn.addAll(*array);

            //set looping animation flag
            animationIn.SetLoopFlag(loopAnimation);
        }
        
    
    }
}



TArray<EArmType> UArmKeyFrameAnimationAsset::GetKeyList() const {
    TArray<EArmType> types;
    for (const TPair<EArmType, FKeyFrameAssetArray>& Pair : KeyFrameAnimations){
        types.Add(Pair.Key);
    }
    return types;
}
    