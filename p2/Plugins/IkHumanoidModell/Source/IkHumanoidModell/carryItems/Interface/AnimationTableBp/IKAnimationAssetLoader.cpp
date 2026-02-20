#include "IKAnimationAssetLoader.h"
#include "AssetPlugin/gamestart/assetManager.h"
#include "IkHumanoidModell/carryItems/Interface/AnimationAsset/ArmKeyFrameAnimationAsset.h"
#include "CoreMath/animation/KeyFrames/KeyFrameAnimation.h"
#include "IkHumanoidModell/carryItems/Interface/EmptyActor/FArmAnimationPair.h"
#include "AssetPlugin/gamestart/AssetLoader.h"

IKAnimationAssetLoader::IKAnimationAssetLoader(){

}

IKAnimationAssetLoader::~IKAnimationAssetLoader(){

}


bool IKAnimationAssetLoader::Load(
    FArmAnimationPair &pair, 
    EArmAnimationEnum typeAnimation
){
    if(UArmKeyFrameAnimationAsset *asset = LoadAsset(typeAnimation)){
        pair.SetupFrom(asset);
        return true;
    }
    return false;
}

bool IKAnimationAssetLoader::Load(
    KeyFrameAnimation &animation, 
    EArmAnimationEnum typeAnimation, 
    EArmType typeArm
){
    if(UArmKeyFrameAnimationAsset *asset = LoadAsset(typeAnimation)){
        //animation.ConstructFrom(asset);
        asset->CopyDataTo(typeArm, animation);
        
        return true;
    }
    return false;
}

UArmKeyFrameAnimationAsset *IKAnimationAssetLoader::LoadAsset(EArmAnimationEnum typeAnimation){
    if(assetManager *manager = assetManager::instance()){
        //find asset, or load and find asset.
        UArmKeyFrameAnimationAsset *asset = FindAssetFromAssetManager(typeAnimation);

        if(asset){
            return asset;
        }
        //try load if finding failed    
        else{
            LoadAnimationToAssetManager(typeAnimation);
        }
        //try find again
        asset = FindAssetFromAssetManager(typeAnimation);
        return asset;
    }
    return nullptr;
}

UArmKeyFrameAnimationAsset* IKAnimationAssetLoader::FindAssetFromAssetManager(
    EArmAnimationEnum typeAnimation
){
    if(assetManager *manager = assetManager::instance()){
        //find asset, or load and find asset.
        UArmKeyFrameAnimationAsset *asset = 
            manager->Find<EArmAnimationEnum, UArmKeyFrameAnimationAsset>(typeAnimation);
        return asset;
    }
    return nullptr;
}

void IKAnimationAssetLoader::LoadAnimationToAssetManager(
    EArmAnimationEnum typeAnimation
){
    FString assetNameBuilded;
    if(AnimationToString(typeAnimation, assetNameBuilded)){
        //// ----- TODO ------
        AssetLoader::LoadAndSavePrimaryDataAssetToAssetToManager<EArmAnimationEnum, UArmKeyFrameAnimationAsset>(
            typeAnimation,// track in asset manager
            PluginName(), // like "Game" for game or any other plugin name
            InnerPath(),  // like: "Prefabs/Weapons/attachments", no trailing slash, found inside the last folder
            assetNameBuilded   // Just the file name as displayed
        );
    }
}

FString IKAnimationAssetLoader::PluginName(){
    return "IkHumanoidModell";
}

FString IKAnimationAssetLoader::InnerPath(){
    return "Animations/";
}

bool IKAnimationAssetLoader::AnimationToString(EArmAnimationEnum type, FString &outString){
    std::map<EArmAnimationEnum, FString> animationStrings;
    animationStrings[EArmAnimationEnum::running] = "RunningAnimation";
    animationStrings[EArmAnimationEnum::throwItem] = "ThrowItemAnimation";
    if(animationStrings.find(type) != animationStrings.end()){
        outString = animationStrings[type];
        return true;
    }
    return false;
}

FString IKAnimationAssetLoader::StaticAnimationToString(EArmAnimationEnum type){
    FString s;
    if(AnimationToString(type, s)){
        return s;
    }
    return TEXT("NONE");
}
