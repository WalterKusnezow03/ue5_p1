
#pragma once

#include "CoreMinimal.h"
#include "GameCore/util/ActorBase/ActorBase.h"
#include <map>

#include "IkHumanoidModell/carryItems/Interface/ComponentTracking/HandBoneIndexEnum.h"
#include "IkHumanoidModell/Ik/Controller/TorsoController/EArmType.h"
#include "IkHumanoidModell/carryItems/container/CarriedItemPositionData.h"

#include "CoreMath/animation/timer/PayloadInterface/IPayloadInterface.h"
#include "CoreMath/animation/timer/PayloadInterface/Payload.h"

#include "IkHumanoidModell/carryItems/Interface/EmptyActor/FingerPositions/boneTargetPair/HandBoneTargetPair.h"

/// @brief will track all components. If a component is comapatible as bone, it will be animated.
class IKHUMANOIDMODELL_API CarriedItemFingerPositionManager : public IPayloadInterface{

public:
    // ---- instancing components helper, no BP needed ----
    static USceneComponent *CreateComponent(
        EArmType type, 
        USceneComponent *attachTo,
        AActor *actorOwner
    );
    static USceneComponent *CreateComponent(
        EArmType type, 
        HandBoneIndexEnum index, 
        USceneComponent *attachTo,
        AActor *actorOwner
    );
    static USceneComponent *CreateComponent(
        FString name, 
        USceneComponent *attachTo,
        AActor *actorOwner
    );
    


    // ---- data ----

    CarriedItemFingerPositionManager();
    ~CarriedItemFingerPositionManager();

    void UpdateFrom(AActorBase *baseActor);
    
    void UpdateContainer(
        CarriedItemPositionData &data
    );

    static FString ToString(EArmType typeArm);
    static FString ToString(HandBoneIndexEnum index);
    static FString ToString(EArmType typeArm, HandBoneIndexEnum index);

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


    //used for components which have different names than expected in this class,
    //or not derived by AActorBase from GameCore
    //Allows manual target override with scene Components.
    void OverridePermanentTargetComponent(
        EArmType typeArm,
        USceneComponent *scene
    );

    void OverridePermanentTargetComponent(
        EArmType typeArm,
        HandBoneIndexEnum index,
        USceneComponent *scene
    );




private:
    

    bool HasTemporaryTarget(EArmType typeArm);
    void CreateTemporaryTargetIfNeeded(EArmType typeArm);

    void UpdateContainer(
        CarriedItemPositionData &data,
        EArmType typeArm
    );

    FVector FindLocation(EArmType typeArm, HandBoneIndexEnum index);
    FVector FindLocation(EArmType typeArm);

    void OverridePermanentTargetComponent(
        EArmType typeArm,
        HandBoneIndexEnum index,
        AActorBase *baseActor
    );
    

    void OverridePermanentTargetComponent(
        EArmType typeArm,
        AActorBase *baseActor
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

    //not dynamic override
    USceneComponent *GetComponentStatic(
        EArmType typeArm
    );



    //notify stuff
    bool HasType(FString message, EArmType &typeArm);
    bool MessageIsTargetReset(FString message);
    FString ResetMessage(EArmType type);
};