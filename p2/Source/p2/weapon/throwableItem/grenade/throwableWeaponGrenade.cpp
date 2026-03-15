#include "throwableWeaponGrenade.h"

#include "p2/entityManager/EntityManager.h"
#include "p2/_world/worldLevel.h"



void AthrowableWeaponGrenade::BeginPlay(){
    Super::BeginPlay();
    SetupExplosiveHelperOnBeginPlay();
    timerStarted = false;
    isDetonated = false;
    wasMarkedForThrow = false;
}

void AthrowableWeaponGrenade::SetupExplosiveHelperOnBeginPlay(){
    explosiveHelper.Setup(EXPLOSION_RADIUS, DAMAGE, DAMAGE_RADIUS);
}


void AthrowableWeaponGrenade::releaseShoot(){
    Super::releaseShoot();
    if(wasMarkedForThrow){
        delayedExplosionTimer.Begin(timeDelay);
        isDetonated = false;
        timerStarted = true;
    }
}

void AthrowableWeaponGrenade::Tick(float deltatime){
    Super::Tick(deltatime);
    TickExplosionTimer(deltatime);
}

void AthrowableWeaponGrenade::TickExplosionTimer(float deltatime){
    if(timerStarted){
        delayedExplosionTimer.Tick(deltatime);
        if (delayedExplosionTimer.timesUp())
        {
            Detonate();
            timerStarted = false;
        }
    }
}




//to be overriden if needed!
void AthrowableWeaponGrenade::Detonate(){
    if(isDetonated){
        return;
    }
    isDetonated = true;

    //use physics mesh location for damage: is different from aactor!
    FVector location = PhysicsMeshLocation(); 
    explosiveHelper.detonate(location, GetWorld());

    release(); //release to entity manager
}

void AthrowableWeaponGrenade::release(){
    ResetPhysics();
    ResetDenonatedStatus();
    showWeapon(false);
    SetPhysicsEnabled(false);
    if (EntityManager *e = AworldLevel::entityManager())
    {
        e->add(this);
    }
}


void AthrowableWeaponGrenade::ResetDenonatedStatus(){
    isDetonated = false;
}


void AthrowableWeaponGrenade::takedamage(FCustomHitResult &result){
    Detonate();
}


bool AthrowableWeaponGrenade::IsInRange(const FVector &position, float maxDistance){
	float distSquared = maxDistance * maxDistance;
	return FVector::DistSquared(position, GetActorLocation()) <= distSquared;
}



void AthrowableWeaponGrenade::setTeam(teamEnum t){
    team = t;
}

teamEnum AthrowableWeaponGrenade::getTeam(){
    return team;
}




USceneComponent *AthrowableWeaponGrenade::FindHandCarriedScene(EArmType type){
    if(type == EArmType::ERight){
        //grenade to update location
        //or maybe not who knows.
    }
    USceneComponent *found = handAndFingerPositionManager.findPermanentTargetComponent(type);
    if(found){
        DebugHelper::showScreenMessage("AthrowableWeaponGrenade::Found hand Scene", FColor::Blue);
    }
    return found;

    //return handAndFingerPositionManager.findPermanentTargetComponent(type);
}