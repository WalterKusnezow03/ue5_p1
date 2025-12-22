
#pragma once

#include "CoreMinimal.h"
#include "GameCore/util/ActorBase/ActorBase.h"
#include <map>

#include "p2/entities/customIk/bonePackage/handPackage/HandBoneIndexEnum.h"
#include "IkHumanoidModell/Ik/Controller/TorsoController/EArmType.h"
#include "IkHumanoidModell/carryItems/container/CarriedItemPositionData.h"

#include "GameCore/util/timer/PayloadInterface/IPayloadInterface.h"
#include "GameCore/util/timer/PayloadInterface/Payload.h"

#include "p2/weapon/CarriedItemBase/FingerPositions/boneTargetPair/HandBoneTargetPair.h"


/// @brief will track all components. If a component is comapatible as bone, it will be animated.
class P2_API CarriedItemFingerPositionManager : public IPayloadInterface{

public:
    CarriedItemFingerPositionManager();
    ~CarriedItemFingerPositionManager();

    void UpdateFrom(AActorBase *baseActor);
    
    void UpdateContainer(
        CarriedItemPositionData &data,
        AActor *actor
    );

    FString ToString(EArmType typeArm);
    FString ToString(HandBoneIndexEnum index);
    FString ToString(EArmType typeArm, HandBoneIndexEnum index);

    //tempory change hands for animation
    Payload UpdateTemporaryTargetWithPaylaod(
        EArmType typearm,
        USkeletalMeshComponent *comp,
        FString boneName
    );

    
    void UpdateTemporaryTarget(
        EArmType typeArm,
        USkeletalMeshComponent *comp,
        FString boneName
    );

    void ResetTemporaryTarget(EArmType typeArm);

    virtual void Notify(FString message) override;

private:
    

    bool HasTemporaryTarget(EArmType typeArm);
    void CreateTemporaryTargetIfNeeded(EArmType typeArm);

    void UpdateContainer(
        CarriedItemPositionData &data,
        AActor *actor,
        EArmType typeArm
    );

    FVector FindLocation(EArmType typeArm, HandBoneIndexEnum index);
    FVector FindLocation(EArmType typeArm);

    void OverrideComponent(
        EArmType typeArm,
        HandBoneIndexEnum index,
        AActorBase *baseActor
    );
    void OverrideComponent(
        EArmType typeArm,
        HandBoneIndexEnum index,
        USceneComponent *scene
    );

    void OverrideComponent(
        EArmType typeArm,
        AActorBase *baseActor
    );
    void OverrideComponent(
        EArmType typeArm,
        USceneComponent *scene
    );

    std::map<EArmType, std::map<HandBoneIndexEnum, USceneComponent *>> fingerMap;
    std::map<EArmType, USceneComponent *> handMap;


    std::map<EArmType, HandBoneTargetPair> handMapTemporary;

    void GetComponents(
        EArmType type,
        TArray<USceneComponent *> &outArray
    );

    USceneComponent *GetComponent(
        EArmType typeArm,
        HandBoneIndexEnum index
    );



    //notify stuff
    bool HasType(FString message, EArmType &typeArm);
    bool MessageIsTargetReset(FString message);
    FString ResetMessage(EArmType type);
};