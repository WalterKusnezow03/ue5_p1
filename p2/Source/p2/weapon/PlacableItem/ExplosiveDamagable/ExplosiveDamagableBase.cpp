#include "ExplosiveDamagableBase.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/_world/worldLevel.h"


AExplosiveDamagableBase::AExplosiveDamagableBase() : Super() {

}


void AExplosiveDamagableBase::BeginPlay(){
    Super::BeginPlay();
    SetupExplosiveHelperOnBeginPlay();
}

void AExplosiveDamagableBase::SetupExplosiveHelperOnBeginPlay(){
    explosiveHelper.Setup(EXPLOSION_RADIUS, DAMAGE, DAMAGE_RADIUS);
}


void AExplosiveDamagableBase::ResetFlagsAndProperties(){
    Super::ResetFlagsAndProperties();
    isDetonated = false;
}

//to be overriden if needed!
void AExplosiveDamagableBase::Detonate(){
    if(isDetonated){
        return;
    }
    isDetonated = true;    

    FVector location = GetActorLocation();
    explosiveHelper.detonate(location, GetWorld());
    //hide after detonate
    showWeapon(false); //from super::Aweapon

    
    release(); //release to entity manager
}


void AExplosiveDamagableBase::ResetDenonatedStatus(){
    isDetonated = false;
}


void AExplosiveDamagableBase::Tick(float deltatime){
    Super::Tick(deltatime);

}

void AExplosiveDamagableBase::release(){
	showWeapon(false);
	if(EntityManager *e = AworldLevel::entityManager()){
		e->add(this);
	}
}







void AExplosiveDamagableBase::takedamage(FCustomHitResult &result){
    Detonate();
}


bool AExplosiveDamagableBase::IsInRange(const FVector &position, float maxDistance){
	float distSquared = maxDistance * maxDistance;
	return FVector::DistSquared(position, GetActorLocation()) <= distSquared;
}



void AExplosiveDamagableBase::setTeam(teamEnum t){
    team = t;
}

teamEnum AExplosiveDamagableBase::getTeam(){
    return team;
}