#include "throwableWeaponAbstractGrenade.h"

#include "p2/entityManager/EntityManager.h"
#include "p2/_world/worldLevel.h"

#include "DebugPlugin/DebugHelper.h"


void AthrowableWeaponAbstractGrenade::BeginPlay(){
    Super::BeginPlay();
    FindDetonationLeverOnBeginPlay();
    timerStarted = false;
    isDetonated = false;
    wasMarkedForThrow = false;
}

void AthrowableWeaponAbstractGrenade::FindDetonationLeverOnBeginPlay(){
    if(!leverMesh){
        TTryAssignByNameContains<USceneComponent>(leverMeshName, leverMesh);
    }
}

void AthrowableWeaponAbstractGrenade::releaseShoot(){
    Super::releaseShoot();

    if(!wasMarkedForThrow){//if(!timerStarted){
        delayedExplosionTimer.Begin(timeDelay); //, false);
        isDetonated = false;
        timerStarted = true;
        leverTimer.Begin(leverReleaseTime, false);
    }
}

void AthrowableWeaponAbstractGrenade::Tick(float deltatime){
    Super::Tick(deltatime);
    TickExplosionTimer(deltatime);
    TickLeverMovement(deltatime);
}

void AthrowableWeaponAbstractGrenade::TickLeverMovement(float deltatime){
    TickLeverTimer(deltatime);
    ApplyLeverMovement();
}

void AthrowableWeaponAbstractGrenade::TickLeverTimer(float deltatime){
    if(leverTimer.timesUp()){
        return;
    }
    leverTimer.Tick(deltatime);
}
void AthrowableWeaponAbstractGrenade::ApplyLeverMovement(){
    if(leverMesh){
        if(leverTimer.timesUp()){
            return;
        }
        float scalar = leverTimer.scalar();

        float degreeFreedom = 30.0f;
        FRotator r;
        r.Yaw = 0.0f;
        r.Roll = 0.0f;
        r.Pitch = -1.0f * degreeFreedom * scalar;
        leverMesh->SetRelativeRotation(r);
    }
}








void AthrowableWeaponAbstractGrenade::TickExplosionTimer(float deltatime){
    if(timerStarted){
        //DebugHelper::showScreenMessage("AthrowableWeaponAbstractGrenade::TIME STARTED!", FColor::Cyan);
        delayedExplosionTimer.Tick(deltatime);
        float timeLeft = delayedExplosionTimer.currentTimeLeft();
        //DebugHelper::showScreenMessage("AthrowableWeaponAbstractGrenade:: Time left", timeLeft);

        if (delayedExplosionTimer.timesUp())
        {
            Detonate();
            timerStarted = false;
        }
    }else{
        //DebugHelper::showScreenMessage("AthrowableWeaponAbstractGrenade::TIME NOT STARTED!", FColor::Red);
    }
}




//to be overriden ! 
void AthrowableWeaponAbstractGrenade::Detonate(){
    if(isDetonated){
        return;
    }
    isDetonated = true;


    // --- custom logic in derived classes here ---

}

void AthrowableWeaponAbstractGrenade::release(){
    ResetPhysics();
    ResetDenonatedStatus();
    showWeapon(false);
    SetPhysicsEnabled(false);
    UnRegisterFromMiniMap();
    if (EntityManager *e = AworldLevel::entityManager())
    {
        e->add(this);
    }
}


void AthrowableWeaponAbstractGrenade::ResetDenonatedStatus(){
    isDetonated = false;
}


void AthrowableWeaponAbstractGrenade::takedamage(FCustomHitResult &result){
    
}


bool AthrowableWeaponAbstractGrenade::IsInRange(const FVector &position, float maxDistance){
	float distSquared = maxDistance * maxDistance;
	return FVector::DistSquared(position, GetActorLocation()) <= distSquared;
}



void AthrowableWeaponAbstractGrenade::setTeam(teamEnum t){
    team = t;
}

teamEnum AthrowableWeaponAbstractGrenade::getTeam(){
    return team;
}




USceneComponent *AthrowableWeaponAbstractGrenade::FindHandCarriedScene(EArmType type){
    if(type == EArmType::ERight){
        //grenade to update location
        //or maybe not who knows.
    }
    USceneComponent *found = handAndFingerPositionManager.findPermanentTargetComponent(type);
    if(found){
        //DebugHelper::showScreenMessage("AthrowableWeaponAbstractGrenade::Found hand Scene", FColor::Blue);
    }
    return found;

    //return handAndFingerPositionManager.findPermanentTargetComponent(type);
}