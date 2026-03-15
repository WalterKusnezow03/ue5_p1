#include "IKCarryInterfaceAnimatedActor.h"
#include "IkHumanoidModell/carryItems/Interface/AnimationTableBp/IKAnimationAssetLoader.h"

AIKCarryInterfaceAnimatedActor::AIKCarryInterfaceAnimatedActor() : Super() {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("USceneRoot"));
}


void AIKCarryInterfaceAnimatedActor::SetDebugPlayerAnimatedActor(bool flag){
    isPickedUpByPlayerDebugFlag = flag;
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
    localAnimationOffset = FVector(0, 0, 0);
    InitComponents();
    InitAnimationTable();
    InitAxisConstraintEmpty();
}


void AIKCarryInterfaceAnimatedActor::InitAxisConstraintEmpty(){
    axisConstraintNone.SetupNoneButPositionLocked();
    
    //debugs
    axisConstraintNone.SetupRollPitchYaw(false, false, true);
}

FIKCarryInterfaceAxisConstraint &AIKCarryInterfaceAnimatedActor::getAxisConstraint(){
    if(animationActiveFlag){
        //return activeAnimation.getAxisConstraint();
    }
    return axisConstraintNone;
}

void AIKCarryInterfaceAnimatedActor::InitComponents(){
    
    
    //create components by default
    // -> just hand locations for now
    if(AutoInitComponents() && RootComponent){
        //create scenes
        RightHandComponent = CarriedItemHandComponentManager::CreateComponent(
            EArmType::ERight, 
            RootComponent,
            this
        );
        LefttHandComponent = CarriedItemHandComponentManager::CreateComponent(
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
    return handAndFingerPositionManager.getItemPositionDataRef();
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

    //is bugged! (not anymore)
    if(drawLineOnTransformUpdate){
        DebugHelper::showLineBetween(
            GetWorld(),
            location,
            location + FVector(0,0,100),
            FColor::Green,
            0.1f
        );
    }
    

    SetActorLocation(location);
    SetActorRotation(rotation);
    internalTransform.setTranslation(location);
    internalTransform.setRotation(rotation);



    ///// ---- BUG HERE ! ---- ROTATION GOES INTO VERY RANDOM DIRECTIONS ! ----

    //debug show rotation
    FVector forward(100, 0, 0);
    MMatrix r(rotation);
    forward = r * forward;

    if(drawLineOnTransformUpdate){
        DebugHelper::showLineBetween(
            GetWorld(),
            location,
            location + forward,
            FColor::Red,
            0.1f
        );
    }
    
}

// called by UpdateActorTransform, By Skelleton, on Tick.
void AIKCarryInterfaceAnimatedActor::UpdateHasMovedFlag(const FVector &location){
    hasMovedFlag = false;
    FVector current = internalTransform.getTranslation();
    if(FVector::DistSquared(current, location) >= distSquaredMovedFlag){
        hasMovedFlag = true;
        if(logEnabled){
            DebugHelper::showScreenMessage("AIKCarryInterfaceAnimatedActor::HAS MOVED", FColor::Green);
        }
    }
}

//updates for attached item rotation
void AIKCarryInterfaceAnimatedActor::UpdateLowerArm(EArmType typeArm, const FVector &direction){
    FVector upVectorRelative = OrthogonalLocalUpFor(direction);

    // TESTING NEEDED
    UpdateHandComponentRotation(typeArm, upVectorRelative);
    // TESTING NEEDED
}   

//generates the local up vector to a given direction
FVector AIKCarryInterfaceAnimatedActor::OrthogonalLocalUpFor(const FVector &vec){
    FVector side = FVectorUtil::BuildSideVectorRight(vec);
    FVector up = FVector::CrossProduct(vec, side); // up: a x b
    return up.GetSafeNormal();
}



void AIKCarryInterfaceAnimatedActor::FireAnimation(EArmAnimationEnum id){
    //animation might be blocked if incompatibel with logic.


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

        //hotfix?
        activeAnimation.ResetAnimations();
        DebugHelper::logMessage("AIKCarryInterfaceAnimatedActor::Has Animation, Fire And reset: ", anim);

        animationActiveFlag = true;
    }
}

void AIKCarryInterfaceAnimatedActor::StopAnimation(){
    //handle is unclear here: revert to some default state (?)
    animationActiveFlag = false;
}

bool AIKCarryInterfaceAnimatedActor::IsAnimationActive(EArmAnimationEnum id){
    if(animationActiveFlag){
        if(activeAnimation.IsType(id)){
            return true;
        }
    }
    return false;
}

/// ----- PICKUP FLAG ------

void AIKCarryInterfaceAnimatedActor::SetIsPickedUpFlag(bool flag){
    isPickedUpFlag = flag;
}


/// ----- HAND LOCAL TRANSFORM UPDATE -----

void AIKCarryInterfaceAnimatedActor::UpdateHandComponentRotation(EArmType typeArm, FVector &rotationVecIn){
    FRotator rotation = rotationVecIn.GetSafeNormal().Rotation();
    UpdateHandComponentRotation(typeArm, rotation);
}

void AIKCarryInterfaceAnimatedActor::UpdateHandComponentRotation(EArmType typeArm, FRotator &rotation){
    if(USceneComponent *hand = FindHand(typeArm)){
        hand->SetRelativeRotation(rotation);
    }
}

void AIKCarryInterfaceAnimatedActor::UpdateHandComponentLocation(
    EArmType typeArm,
    FVector &location
){
    if(USceneComponent *hand = FindHand(typeArm)){
        hand->SetRelativeLocation(location);
    }
}




/// ----- TICK -----

void AIKCarryInterfaceAnimatedActor::Tick(float deltatime){
    Super::Tick(deltatime);

    //if(isPickedUpFlag || true){
    if(isPickedUpFlag){
        TickAnimation(deltatime);
        TickUpdateAttachedItem();
    }

    ScreenLogPickedUpState();
}



void AIKCarryInterfaceAnimatedActor::ScreenLogPickedUpState(){
    if(isPickedUpByPlayerDebugFlag && logEnabled){

        FString message = isPickedUpFlag ? 
            TEXT("AIKCarryInterfaceAnimatedActor::IsPickedUp") :
            TEXT("AIKCarryInterfaceAnimatedActor::IsNOTPickedUp");

        FColor color = isPickedUpFlag ? FColor::Green : FColor::Red;

        DebugHelper::showScreenMessage(message, color);
    }
}












void AIKCarryInterfaceAnimatedActor::TickAnimation(float deltatime){
    if(CurrentAnimationCanBeTicked()){

        LogCurrentAnimationStatus();

        bool armLeftFinished = TickAnimationFor(EArmType::ELeft, deltatime);
        bool armRigthFinished = TickAnimationFor(EArmType::ERight, deltatime);

        if(armLeftFinished && armRigthFinished){
            ApplyImpulseToCarriedItemIfThrowFinished();
            
            //reset flag afterwards! (keep like this!)
            //animationActiveFlag = false;

            PickupQeuedItemIfThrowFinished(); //call to inject next item to be carried.
        }
    }
}

void AIKCarryInterfaceAnimatedActor::LogCurrentAnimationStatus(){
    if(logEnabled && isPickedUpByPlayerDebugFlag){
        if(animationActiveFlag){
            FString message = FString::Printf(
                TEXT("AIKCarryInterfaceAnimatedActor::ANIMTAION %s"),
                *activeAnimation.ToString()
            );
            DebugHelper::showScreenMessage(message, FColor::Cyan);
        }else{
            DebugHelper::showScreenMessage("AIKCarryInterfaceAnimatedActor::NO ANIMATION ACTIVE", FColor::Red);
        }
    }
}




// --- Tick update throwing and buffer qeued item for pickup ---
void AIKCarryInterfaceAnimatedActor::ApplyImpulseToCarriedItemIfThrowFinished(){
    if(ActiveAnimationIsThrowingItemAnimation()){
        if(attachedHandCarriedItem){

            // --- some camera rotation may be needed here ! ---
            FVector dir = ThrowingDirectionOfItem();
            attachedHandCarriedItem->MarkForApplyImpulse(dir);

            EjectCarryByHandItem(); //remove thrown item
        }
    }
}

//caution: dirty oslution, no camera look dir implemented here!!!!
//may be head rotation?
FVector AIKCarryInterfaceAnimatedActor::ThrowingDirectionOfItem(){
    //erstmal so
    FVector forward(1, 0, 0);
    FRotator rotator = GetActorRotation();
    MMatrix rotationMat(rotator);
    forward = rotationMat * forward;
    return forward;
}

void AIKCarryInterfaceAnimatedActor::PickupQeuedItemIfThrowFinished(){
    if(ActiveAnimationIsThrowingItemAnimation()){
        if(qeuedForPickupAttachedHandCarriedItem != nullptr){
            InjectCarryByHandItem(qeuedForPickupAttachedHandCarriedItem);
            qeuedForPickupAttachedHandCarriedItem = nullptr;
        }
        /*else{
            EjectCarryByHandItem(); //remove thrown item
        }*/
        DebugHelper::logMessage("AIKCarryInterfaceAnimatedActor::PickupQeuedItemIfThrowFinished");
        FireAnimation(EArmAnimationEnum::running); //switch to running.
        return;
    }
    animationActiveFlag = false;
}

bool AIKCarryInterfaceAnimatedActor::ActiveAnimationIsThrowingItemAnimation(){
    if(animationActiveFlag){
        return activeAnimation.IsType(EArmAnimationEnum::throwItem);
    }
    return false;
}

bool AIKCarryInterfaceAnimatedActor::CurrentAnimationCanBeTicked(){
    //tick by default excpect in cases:
    bool canBeTicked = true;

    //if the actor has moved and must run -> tick
    if(!hasMovedFlag && activeAnimation.IsType(EArmAnimationEnum::running)){
        canBeTicked = false;
        
        //DEBUG
        //canBeTicked = true;
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
    UpdateHandComponentLocation(type, outPosLocal);

    
    //DebugDrawHandLocation(type, deltatime);

    return finished;
}



















void AIKCarryInterfaceAnimatedActor::DebugDrawHandLocation(EArmType type, float deltatime){
    if(logEnabled){
        if(USceneComponent *hand = FindHand(type)){
            DebugHelper::showLineBetween(
                GetWorld(),
                hand->GetComponentLocation(),
                hand->GetComponentLocation() + FVector(0, 0, 10000),
                FColor::Red,
                deltatime * 2.0f
            );
        }
    }
}








void AIKCarryInterfaceAnimatedActor::TickUpdateAttachedItem(){
    TickUpdateAttachedItemGlobalTransform();
    TickUpdateAttachedItemLocalTransform(EArmType::ELeft);
    TickUpdateAttachedItemLocalTransform(EArmType::ERight);
}

void AIKCarryInterfaceAnimatedActor::TickUpdateAttachedItemGlobalTransform(){
    if(attachedHandCarriedItem){
        //update attached item location to own
        FVector location = GetActorLocation();
        FRotator rotation = GetActorRotation();
        attachedHandCarriedItem->UpdateActorTransform(location, rotation);
        
        //draw
        /*DebugHelper::showLineBetween(
            GetWorld(),
            location, 
            location + FVector(100,0,0),
            FColor::Green,
            0.1f
        );*/
    }
}

void AIKCarryInterfaceAnimatedActor::TickUpdateAttachedItemLocalTransform(EArmType type){
    if(attachedHandCarriedItem){
        if(USceneComponent *hand = FindHand(type)){

            //update components 
            FVector localLocation = hand->GetRelativeLocation();
            FRotator localRotation = hand->GetRelativeRotation();
            attachedHandCarriedItem->UpdateLocalSceneTransformCarriedByHand(
                type, 
                localLocation, 
                localRotation
            );
        }
    }
}









// ---- HAND ATTACHED ITEMS ----
void AIKCarryInterfaceAnimatedActor::InjectCarryByHandItem(IIkCarryInterface *newItem){
    if(IsHandAttachedItem(newItem)){
        //throw running with an active item: queue:
        if(ActiveAnimationIsThrowingItemAnimation() && attachedHandCarriedItem != nullptr){
            qeuedForPickupAttachedHandCarriedItem = newItem;
            return;
        }

        attachedHandCarriedItem = newItem;
    }
}

IIkCarryInterface *AIKCarryInterfaceAnimatedActor::CurrentAttachedItem(){
    return attachedHandCarriedItem;
}

void AIKCarryInterfaceAnimatedActor::EjectCarryByHandItem(){
    attachedHandCarriedItem = nullptr;
}

bool AIKCarryInterfaceAnimatedActor::IsHandAttachedItem(IIkCarryInterface *newItem){
    if(newItem){
        return newItem->GetCarryType() == EIKCarryType::ECarryByHand;
    }
    return false;
}
