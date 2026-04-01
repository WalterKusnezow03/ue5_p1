#include "throwableWeaponHitDamage.h"
#include "CoreMath/util/SphereCaster.h"
#include "GameCore/interfaces/DamageInterface/Damageinterface.h"

void AthrowableWeaponHitDamage::Tick(float deltatime){
    Super::Tick(deltatime);
    TickCheckHit(deltatime);
}

void AthrowableWeaponHitDamage::TickCheckHit(float DeltaTime){
    float SphereRadius = 50.0f;
    TArray<AActor *> excludedActors;
    excludedActors.Add(this);
    FVector location = GetActorLocation();

    TArray<AActor *> array = sphereCaster.getTActorsInRadius<AActor>(
        GetWorld(), 
        location, 
        SphereRadius,
        excludedActors
    );
    ProcessHit(array, DeltaTime);    
}

void AthrowableWeaponHitDamage::ProcessHit(
    TArray<AActor *> &castedActors,
    float DeltaTime
){
    for (int i = 0; i < castedActors.Num(); i++){
        if(AActor *current = castedActors[i]){
            if(IDamageinterface *interfaceFound = Cast<IDamageinterface>(current)){

                FCustomHitResult result = MakeHitResultByFoundActor(
                    current,
                    DeltaTime
                );
                interfaceFound->takedamage(result);
                /*virtual void takedamage(FCustomHitResult &result);
                interfaceFound->damageOnHit

                ownTeam = h->getTeam();*/
                return;
            }
        }
    }
}





FCustomHitResult AthrowableWeaponHitDamage::MakeHitResultByFoundActor(
	AActor *hitActor,
    float DeltaTime
){
    FCustomHitResult hitResultPackage;
    if(hitActor){
        FVector direction = PhysicsMeshVelocityDirection();
        hitResultPackage.SetupHitResult(
            throwableDamage,
            true,//bool inSurpressed,
            DeltaTime,
            hitActor,
            direction
        );

    }
	return hitResultPackage;
}