#include "ExplosiveDamagableBase.h"


AExplosiveDamagableBase::AExplosiveDamagableBase() : Super() {

}


void AExplosiveDamagableBase::BeginPlay(){
    Super::BeginPlay();
    SetupExplosiveHelperOnBeginPlay();
}

void AExplosiveDamagableBase::SetupExplosiveHelperOnBeginPlay(){
    explosiveHelper.Setup(EXPLOSION_RADIUS, DAMAGE, DAMAGE_RADIUS);
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








void AExplosiveDamagableBase::takedamage(int d){
    Detonate();
}

void AExplosiveDamagableBase::takedamage(int d, FVector &hitpoint){
    takedamage(d);
}

void AExplosiveDamagableBase::takedamage(int d, bool surpressed){
    takedamage(d);
}


void AExplosiveDamagableBase::takedamage(int d, FVector &hitpoint, bool surpressed){
    takedamage(d);
}

void AExplosiveDamagableBase::setTeam(teamEnum t){
    team = t;
}

teamEnum AExplosiveDamagableBase::getTeam(){
    return team;
}