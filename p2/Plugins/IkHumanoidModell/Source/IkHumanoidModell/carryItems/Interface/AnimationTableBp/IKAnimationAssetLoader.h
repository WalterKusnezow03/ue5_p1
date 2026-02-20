#pragma once

#include "CoreMinimal.h"
#include "IkHumanoidModell/carryItems/Interface/EmptyActor/EArmAnimationEnum.h"
#include "IkHumanoidModell/Ik/Controller/TorsoController/EArmType.h"

class FArmAnimationPair;
class KeyFrameAnimation;
class UArmKeyFrameAnimationAsset;

//assets stored in assetmanager plugin. But also aceccible here for simplicity.
//animations will be converted
//-> PluginName: IkHumanoidModell
//-> PathNameBase: Animations/ 
//-> AnimationName: somenameAnimation
class IKHUMANOIDMODELL_API IKAnimationAssetLoader {


public:
    IKAnimationAssetLoader();
    ~IKAnimationAssetLoader();

    bool Load(FArmAnimationPair &pair, EArmAnimationEnum type);
    bool Load(KeyFrameAnimation &animation, EArmAnimationEnum type, EArmType typeArm);

    static FString StaticAnimationToString(EArmAnimationEnum type);

private:
    //asset either found or loaded and found
    UArmKeyFrameAnimationAsset *LoadAsset(EArmAnimationEnum type);
    //explicit find asset
    UArmKeyFrameAnimationAsset *FindAssetFromAssetManager(
        EArmAnimationEnum typeAnimation
    );

    FString PluginName();
    FString InnerPath();

    void LoadAnimationToAssetManager(EArmAnimationEnum typeAnimation);
    static bool AnimationToString(EArmAnimationEnum type, FString &outString);
    
};
