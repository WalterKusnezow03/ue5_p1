#pragma once

#include "CoreMinimal.h"
#include "CoreMath/animation/timer/PayloadInterface/IPayloadInterface.h"
#include "CoreMath/animation/KeyFrames/Payloaded/PayloadKeyFrameAnimation.h"
#include "CoreMath/animation/KeyFrames/Chained/KeyFrameAnimationChained.h"


#include "p2/weapon/WeaponPartsOrganizer/weaponPartsCollection.h"


class UPlateCarrierMagSocket;

//will handle the mag swapping process
//from ejecting from mag pouch, swap to weapon
//and injecting the weapon mag back into the magsocket / pouch
//-> will hold all needed pointers and tick / setup the animation
class P2_API MagSwapProcess : public IPayloadInterface{

public:
    

    //call from weapon
    void PrepareSetHandComponent(USceneComponent *handIn);

    void PrepareSetEjectedWeaponMag(
        WeaponPartsCollection &weaponPartsCollectionIn //parenting reference replace needed after anim finished.
    );

    //call from palte carrier
    void SetupAndStartAnimation(UPlateCarrierMagSocket *swapSlotIn);

    //todo: swapped mag must notifed to weapon as scene object

    virtual void Notify(FString message) override;


    void TickAnimation(float deltatime);
    bool IsFinished();

private:
    void SetupAnimations();

    //components will be moved by the hand during the complete animation process
    USceneComponent *hand = nullptr;

    WeaponPartsCollection *weaponComponents = nullptr;
    USceneComponent *ejectedMag = nullptr;
    USceneComponent *ejectedMagOriginalParent = nullptr;

    //must be updated into weapon after swap - hand target pairs need notification
    USceneComponent *injectedMag = nullptr; //injected into weapon

    UPlateCarrierMagSocket *swapSlot = nullptr;

    

    FString payloadHandMovedToMagHolster = "handToHolster";
    FString payloadHandMovedToMagEjected = "handToEjectedMag";
    FString payloadHandReloadFinished = "handToReloadFinished";
    FString payloadHandToHolsterBack = "handToHolsterAfterReload";
    FString payloadHandToOriginal = "handToOriginalPos";


    KeyFrameAnimationChained animationQueue;

    int animationIndex = 0;

    float animationEinheitsValue = 0.5f; //0.5 cm
    void MakeSpline(
        FVector &start,
        FVector &end,
        TArray<FVector> &supportPoints,
        TArray<FVector> &outSpline
    );
    void MakeSpline(
        FVector &start,
        FVector &end,
        TArray<FVector> &supportPoints,
        TArray<FVector> &outSpline,
        float einheitsValue
    );

    void AttachHolsteredMagToHand();
    void AttachEjectedMagToHand();
    void AttachHolsteredMagToWeapon();
    void AttachEjectedMagToCarrier();

    FVector RelativeLocation(USceneComponent *from, USceneComponent *target);
    FVector RelativeLocation(USceneComponent *from, FVector &targetWorld);

    void ConstructAnimationHandTowards(
        PayloadKeyFrameAnimation &animation,
        FVector hand,
        FVector target,
        FString payloadName,
        float timeOfAnimation
    );

    void ConstructAnimationHandTowards(
        PayloadKeyFrameAnimation &animation,
        TArray<FVector> &keyframes,
        FString payloadName,
        float timeOfAnimation
    );

    void ConstructAnimationSplineHandTowards(
        PayloadKeyFrameAnimation &animation,
        FVector hand,
        TArray<FVector> &supportPoints,
        FVector target,
        FString payloadName,
        float timeOfAnimation
    );

    void AttachToKeepWorldSpace(USceneComponent *newChild, USceneComponent *newParent);
    void AttachToSnap(USceneComponent *newChild, USceneComponent *newParent);
    void Detach(USceneComponent *comp);

    void DrawAnimationDebug();

    bool pause = false;


    FVector handOriginal;
};