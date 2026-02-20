#include "IKCarryInterfaceAnimatedActor.h"
#include "IkHumanoidModell/carryItems/Interface/AnimationTableBp/IKAnimationAssetLoader.h"

AIKCarryInterfaceAnimatedActor::AIKCarryInterfaceAnimatedActor() : Super() {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("USceneRoot"));
}


AIKCarryInterfaceAnimatedActor* AIKCarryInterfaceAnimatedActor::makeInstance(UWorld *world){
    if(world){
        UClass *toSpawn = AIKCarryInterfaceAnimatedActor::StaticClass();
        if(toSpawn){
            
            FActorSpawnParameters SpawnParams;
            FVector Location;
            AIKCarryInterfaceAnimatedActor *spawned = 
                world->SpawnActor<AIKCarryInterfaceAnimatedActor>(
                    toSpawn, Location, FRotator::ZeroRotator, SpawnParams
                );
            return spawned;
        }
    }
    return nullptr;
}



void AIKCarryInterfaceAnimatedActor::BeginPlay(){
    Super::BeginPlay();
    InitComponents();
    InitAnimationTable();
    InitAxisConstraintEmpty();
}


void AIKCarryInterfaceAnimatedActor::InitAxisConstraintEmpty(){
    axisConstraintNone.SetupNone();
}

FIKCarryInterfaceAxisConstraint &AIKCarryInterfaceAnimatedActor::getAxisConstraint(){
    if(animationActiveFlag){
        return activeAnimation.getAxisConstraint();
    }
    return axisConstraintNone;
}

void AIKCarryInterfaceAnimatedActor::InitComponents(){
    //create components by default
    // -> just hand locations for now
    if(RootComponent){
        //create scenes
        RightHandComponent = CarriedItemFingerPositionManager::CreateComponent(
            EArmType::ERight, 
            RootComponent,
            this
        );
        LefttHandComponent = CarriedItemFingerPositionManager::CreateComponent(
            EArmType::ELeft, 
            RootComponent,
            this
        );

        //override hand target position manager with created components
        handAndFingerPositionManager.OverridePermanentTargetComponent(
            EArmType::ERight,
            RightHandComponent
        );
        handAndFingerPositionManager.OverridePermanentTargetComponent(
            EArmType::ELeft,
            LefttHandComponent
        );

    }
}

void AIKCarryInterfaceAnimatedActor::InitAnimationTable(){
    IKAnimationAssetLoader animationAssetLoader;
    LoadAnimationToAnimationTable(EArmAnimationEnum::running);
    LoadAnimationToAnimationTable(EArmAnimationEnum::throwItem);
}

void AIKCarryInterfaceAnimatedActor::LoadAnimationToAnimationTable(
    EArmAnimationEnum typeAnimation
){
    IKAnimationAssetLoader animationAssetLoader; //can be temporary
    FArmAnimationPair pair;
    if(animationAssetLoader.Load(pair, typeAnimation)){
        ReplaceAnimationPair(pair, typeAnimation);
    }
}


USceneComponent *AIKCarryInterfaceAnimatedActor::FindHand(EArmType type){
    if(type == EArmType::ELeft){
        return LefttHandComponent;
    }
    if(type == EArmType::ERight){
        return RightHandComponent;
    }
    return nullptr;
}





/// @brief must provide this package with end effector hands position and more
/// when asked!
CarriedItemPositionData &AIKCarryInterfaceAnimatedActor::getItemPositionDataRef(){

    //do update -> (updated scene components)
    handAndFingerPositionManager.UpdateContainer(itemPositionData);

    return itemPositionData;
}




bool AIKCarryInterfaceAnimatedActor::HasAnimation(EArmAnimationEnum type){
    return AnimationTable.find(type) != AnimationTable.end();
}

FArmAnimationPair &AIKCarryInterfaceAnimatedActor::FindAnimationPair(EArmAnimationEnum type){
    if(HasAnimation(type)){
        return AnimationTable[type];
    }
    return fallback;
}

FArmAnimationPair &AIKCarryInterfaceAnimatedActor::FindAnimationPairAndResetProgress(EArmAnimationEnum type){
    FArmAnimationPair &pair = FindAnimationPair(type);
    pair.ResetAnimations();
    return pair;
}

void AIKCarryInterfaceAnimatedActor::ReplaceAnimationPair(
    FArmAnimationPair &anim,
    EArmAnimationEnum type
){
    anim.SetType(type); //copy type inside for later checks (easier to maintain)
    AnimationTable[type] = anim; //copy to table

    bool debug = true;
    if(debug){
        anim.LogInfo();
    }
}

/// @brief must provide this api for outside updates of the actor (but this is just an interface.)
void AIKCarryInterfaceAnimatedActor::UpdateActorTransform(FVector &location, FRotator &rotation){
    UpdateHasMovedFlag(location);

    SetActorLocation(location);
    SetActorRotation(rotation);
    internalTransform.setTranslation(location);
    internalTransform.setRotation(rotation);
}

// called by UpdateActorTransform, By Skelleton, on Tick.
void AIKCarryInterfaceAnimatedActor::UpdateHasMovedFlag(const FVector &location){
    hasMovedFlag = false;
    FVector current = internalTransform.getTranslation();
    if(FVector::DistSquared(current, location) >= 1.0f){
        hasMovedFlag = true;
    }
}

//updates for attached item rotation
void AIKCarryInterfaceAnimatedActor::UpdateLowerArm(EArmType typeArm, const FVector &direction){

}


void AIKCarryInterfaceAnimatedActor::FireAnimation(EArmAnimationEnum id){
    FString anim = IKAnimationAssetLoader::StaticAnimationToString(id);
    DebugHelper::logMessage("AIKCarryInterfaceAnimatedActor::Try Fire Animation", anim);
    if(HasAnimation(id)){
        DebugHelper::logMessage("AIKCarryInterfaceAnimatedActor::Has Animation, Fire", anim);
        //play anim
        
        if(animationActiveFlag){
            //queue animation?
            //replace start from end? / overlap?
        }
        activeAnimation = FindAnimationPairAndResetProgress(id);
        animationActiveFlag = true;
    }
}

void AIKCarryInterfaceAnimatedActor::StopAnimation(){

}






/// ----- TICK -----

void AIKCarryInterfaceAnimatedActor::Tick(float deltatime){
    Super::Tick(deltatime);
    TickAnimation(deltatime);
}

void AIKCarryInterfaceAnimatedActor::TickAnimation(float deltatime){
    if(CurrentAnimationCanBeTicked()){
        bool armLeftFinished = TickAnimationFor(EArmType::ELeft, deltatime);
        bool armRigthFinished = TickAnimationFor(EArmType::ERight, deltatime);

        if(armLeftFinished && armRigthFinished){
            animationActiveFlag = false;
        }
    }
}

bool AIKCarryInterfaceAnimatedActor::CurrentAnimationCanBeTicked(){
    //tick by default excpect in cases:
    bool canBeTicked = true;

    //if the actor has moved and must run -> tick
    if(!hasMovedFlag && activeAnimation.IsType(EArmAnimationEnum::running)){
        
        canBeTicked = false;
        

        //DEBUG
        canBeTicked = true;
    }

    // other cases to be added.

    return animationActiveFlag && canBeTicked;
}






bool AIKCarryInterfaceAnimatedActor::TickAnimationFor(EArmType type, float deltatime){
    //loop finished: disable
    FVector outPosLocal;
    //BUG HERE
    bool finished = activeAnimation.Tick(
        type,
        deltatime,
        internalTransform,
        outPosLocal
    );
    //Update component location
    if(USceneComponent *component = FindHand(type)){
        component->SetRelativeLocation(outPosLocal);
        
        //DEBUG
        DebugHelper::showScreenMessage("IkCarryAnimPos ", outPosLocal);
        DebugHelper::showLineBetween(
            GetWorld(),
            component->GetComponentLocation(),
            component->GetComponentLocation() + FVector(0, 0, 10000),
            FColor::Red,
            deltatime * 2.0f
        );
    }
    return finished;
}