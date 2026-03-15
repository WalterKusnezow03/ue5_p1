#include "CarriedItemHandComponentManager.h"
 
/// ---- instancing ----

USceneComponent *CarriedItemHandComponentManager::CreateComponent(
    EArmType type, 
    USceneComponent *attachTo,
    AActor *actorOwner
){
    FString name = ToString(type);
    return CreateComponent(name, attachTo, actorOwner);
}

USceneComponent *CarriedItemHandComponentManager::CreateComponent(
    EArmType type, 
    HandBoneIndexEnum index, 
    USceneComponent *attachTo,
    AActor *actorOwner
){
    FString name = ToString(type, index);
    return CreateComponent(name, attachTo, actorOwner);
}

USceneComponent *CarriedItemHandComponentManager::CreateComponent(
    FString name, 
    USceneComponent *attachTo,
    AActor *actorOwner
){
    if(attachTo){
        USceneComponent *component = NewObject<USceneComponent>(actorOwner, FName(*name));
        if(component){
            component->SetupAttachment(attachTo); 
            component->RegisterComponent(); //it is not visible otherwise
        }
        return component;
    }
    return nullptr;
}

// ---- Manager ----


CarriedItemHandComponentManager::CarriedItemHandComponentManager(){

}

CarriedItemHandComponentManager::~CarriedItemHandComponentManager(){

}

bool CarriedItemHandComponentManager::HasType(FString message, EArmType &typeArm){
    if(message.Contains("lefthand")){
        typeArm = EArmType::ELeft;
        return true;
    }
    if(message.Contains("righthand")){
        typeArm = EArmType::ERight;
        return true;
    }
    return false;
}

FString CarriedItemHandComponentManager::ToString(EArmType typeArm){
    std::map<EArmType, FString> armName;
    armName[EArmType::ELeft] = "lefthand";
    armName[EArmType::ERight] = "righthand";

    if(armName.find(typeArm) != armName.end()){
        FString made = armName[typeArm];
        return made;
    }
    return TEXT("NONE");
}

FString CarriedItemHandComponentManager::ToString(HandBoneIndexEnum index){
    std::map<HandBoneIndexEnum, FString> fingerName;
    fingerName[HandBoneIndexEnum::thumb] = "thumb";
    fingerName[HandBoneIndexEnum::finger1] = "finger1";
    fingerName[HandBoneIndexEnum::finger2] = "finger2";
    fingerName[HandBoneIndexEnum::finger3] = "finger3";
    fingerName[HandBoneIndexEnum::finger4] = "finger4";

    if(fingerName.find(index) != fingerName.end()){
        FString made = fingerName[index];
        return made;
    }
    return TEXT("NONE");
}

FString CarriedItemHandComponentManager::ToString(EArmType typeArm, HandBoneIndexEnum index){
    return ToString(typeArm) + ToString(index);
}

FString CarriedItemHandComponentManager::ResetMessage(EArmType typeArm){
    return TEXT("reset") + ToString(typeArm);
}

void CarriedItemHandComponentManager::UpdateFrom(AActorBase *baseActor){
    OverridePermanentTargetComponent(EArmType::ELeft, HandBoneIndexEnum::thumb, baseActor);
    OverridePermanentTargetComponent(EArmType::ELeft, HandBoneIndexEnum::finger1, baseActor);
    OverridePermanentTargetComponent(EArmType::ELeft, HandBoneIndexEnum::finger2, baseActor);
    OverridePermanentTargetComponent(EArmType::ELeft, HandBoneIndexEnum::finger3, baseActor);
    OverridePermanentTargetComponent(EArmType::ELeft, HandBoneIndexEnum::finger4, baseActor);

    OverridePermanentTargetComponent(EArmType::ERight, HandBoneIndexEnum::thumb, baseActor);
    OverridePermanentTargetComponent(EArmType::ERight, HandBoneIndexEnum::finger1, baseActor);
    OverridePermanentTargetComponent(EArmType::ERight, HandBoneIndexEnum::finger2, baseActor);
    OverridePermanentTargetComponent(EArmType::ERight, HandBoneIndexEnum::finger3, baseActor);
    OverridePermanentTargetComponent(EArmType::ERight, HandBoneIndexEnum::finger4, baseActor);

    
    OverridePermanentTargetComponent(EArmType::ELeft, baseActor);
    OverridePermanentTargetComponent(EArmType::ERight, baseActor);
    
}



// --- package for skelleton ---
CarriedItemPositionData &CarriedItemHandComponentManager::getItemPositionDataRef(){
    UpdateContainer(updateDataForSkelleton);
    return updateDataForSkelleton;
}



void CarriedItemHandComponentManager::UpdateContainer(
    CarriedItemPositionData &data
){
    UpdateContainer(data, EArmType::ELeft);
    UpdateContainer(data, EArmType::ERight);
}

void CarriedItemHandComponentManager::UpdateContainer(
    CarriedItemPositionData &data,
    EArmType typeArm
){
    TArray<USceneComponent *> components;
    GetComponents(typeArm, components);
    data.UpdateHandAndFingersWorld(
        typeArm,
        FindLocation(typeArm),
        components,
        GetComponentStatic(typeArm)
    );
}

USceneComponent *CarriedItemHandComponentManager::GetComponentStatic(
    EArmType typeArm
){
    if(handMap.find(typeArm) != handMap.end()){
        return handMap[typeArm];
    }
    return nullptr;
}

FVector CarriedItemHandComponentManager::FindLocation(EArmType type){
    if(HasTemporaryTarget(type)){
        return handMapTemporary[type].boneLocation();
    }

    if(handMap.find(type) != handMap.end()){
        if(USceneComponent *comp = handMap[type]){
            return comp->GetComponentLocation();
        }
    }
    return FVector(0,0,0);
}




FVector CarriedItemHandComponentManager::FindLocation(
    EArmType typeArm, 
    HandBoneIndexEnum index
){
    if(fingerMap.find(typeArm) != fingerMap.end()){
        std::map<HandBoneIndexEnum, USceneComponent *> &map = fingerMap[typeArm];
        if(map.find(index) != map.end()){
            USceneComponent *comp = map[index];
            if(comp){
                return comp->GetComponentLocation();
            }
        }
    }
    return FVector(0, 0, 0);
}



void CarriedItemHandComponentManager::OverridePermanentTargetComponent(
    EArmType typeArm, 
    HandBoneIndexEnum index,
    AActorBase *baseActor
){
    if(baseActor){
        USceneComponent *scene = nullptr;
        baseActor->TTryAssignByNameExact<USceneComponent>(
            ToString(typeArm, index),
            scene
        );
        if(scene){
            OverridePermanentTargetComponent(typeArm, index, scene);
        }
    }
}


void CarriedItemHandComponentManager::OverridePermanentTargetComponent(
    EArmType typeArm, 
    HandBoneIndexEnum index,
    USceneComponent *scene
){
    if(fingerMap.find(typeArm) == fingerMap.end()){
        fingerMap[typeArm] = std::map<HandBoneIndexEnum, USceneComponent *>();
    }
    std::map<HandBoneIndexEnum, USceneComponent *> &map = fingerMap[typeArm];
    map[index] = scene;
}




void CarriedItemHandComponentManager::GetComponents(
    EArmType type, 
    TArray<USceneComponent*> &outArray
){
    outArray.Add(GetComponent(type, HandBoneIndexEnum::thumb));
    outArray.Add(GetComponent(type, HandBoneIndexEnum::finger1));
    outArray.Add(GetComponent(type, HandBoneIndexEnum::finger2));
    outArray.Add(GetComponent(type, HandBoneIndexEnum::finger3));
    outArray.Add(GetComponent(type, HandBoneIndexEnum::finger4));
}


USceneComponent *CarriedItemHandComponentManager::GetComponent(
    EArmType typeArm, 
    HandBoneIndexEnum index
){
    if(fingerMap.find(typeArm) != fingerMap.end()){
        std::map<HandBoneIndexEnum, USceneComponent *> &map = fingerMap[typeArm];
        if(map.find(index) != map.end()){
            return map[index];
        }
    }
    return nullptr;
}



//hand 

void CarriedItemHandComponentManager::OverridePermanentTargetComponent(
    EArmType typeArm, 
    AActorBase *baseActor
){
    if(baseActor){
        USceneComponent *scene = nullptr;
        baseActor->TTryAssignByNameExact<USceneComponent>(
            ToString(typeArm),
            scene
        );
        if(scene){
            OverridePermanentTargetComponent(typeArm, scene);
        }
    }
}


void CarriedItemHandComponentManager::OverridePermanentTargetComponent(
    EArmType typeArm, 
    USceneComponent *scene
){
    handMap[typeArm] = scene;
}

USceneComponent *CarriedItemHandComponentManager::findPermanentTargetComponent(EArmType typeArm){
    if(handMap.find(typeArm) != handMap.end()){
        return handMap[typeArm];
    }
    return nullptr;
}

/// ---- temporary follow handle ----
Payload CarriedItemHandComponentManager::UpdateTemporaryTargetWithPaylaod(
    EArmType typearm, 
    USkeletalMeshComponent *comp,
    FString boneName
){
    FString message = ResetMessage(typearm);
    UpdateTemporaryTarget(typearm, comp, boneName);
    Payload payload(this, message);
    return payload;
}

void CarriedItemHandComponentManager::UpdateTemporaryTarget(
    EArmType typeArm, 
    USkeletalMeshComponent *comp,
    FString boneName
){
    if(comp){
        CreateTemporaryTargetIfNeeded(typeArm);
        handMapTemporary[typeArm].UpdateAndEnable(comp, boneName);
    }
}

void CarriedItemHandComponentManager::ResetTemporaryTarget(EArmType typeArm){
    if(HasTemporaryTarget(typeArm)){
        handMapTemporary[typeArm].disable();
    }
}



bool CarriedItemHandComponentManager::HasTemporaryTarget(EArmType typeArm){
    CreateTemporaryTargetIfNeeded(typeArm);
    return handMapTemporary[typeArm].isEnabled();
}

void CarriedItemHandComponentManager::CreateTemporaryTargetIfNeeded(EArmType typeArm){
    if(handMapTemporary.find(typeArm) == handMapTemporary.end()){
        handMapTemporary[typeArm] = HandBoneTargetPair();
    }
}


//notify reset target
void CarriedItemHandComponentManager::Notify(FString message){
    //react to socket change
    EArmType armFound;
    if (HasType(message, armFound) && MessageIsTargetReset(message)){
        ResetTemporaryTarget(armFound);
    }
}

bool CarriedItemHandComponentManager::MessageIsTargetReset(FString message){
    return message.Contains("reset");
}