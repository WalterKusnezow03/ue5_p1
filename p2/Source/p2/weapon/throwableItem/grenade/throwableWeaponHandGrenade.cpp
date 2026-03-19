#include "throwableWeaponHandGrenade.h"

#include "DebugPlugin/DebugHelper.h"


void AthrowableWeaponHandGrenade::BeginPlay(){
    Super::BeginPlay();
    SetupExplosiveHelperOnBeginPlay();
    
}

void AthrowableWeaponHandGrenade::SetupExplosiveHelperOnBeginPlay(){
    explosiveHelper.Setup(EXPLOSION_RADIUS, DAMAGE, DAMAGE_RADIUS);
}

void AthrowableWeaponHandGrenade::Detonate(){
    if(isDetonated){
        return;
    }
    Super::Detonate();

    for (int i = 0; i < 10; i++){
        DebugHelper::showScreenMessage("AthrowableWeaponHandGrenade:: DETONATE!!!!", FColor::Red);
        DebugHelper::logMessage("AthrowableWeaponHandGrenade:: DETONATE!!!!");
    }

    // use physics mesh location for damage: is different from aactor!
    FVector location = PhysicsMeshLocation(); 
    explosiveHelper.detonate(location, GetWorld());

    release(); //release to entity manager
}



void AthrowableWeaponHandGrenade::takedamage(FCustomHitResult &result){
    Super::takedamage(result);
    Detonate();
}