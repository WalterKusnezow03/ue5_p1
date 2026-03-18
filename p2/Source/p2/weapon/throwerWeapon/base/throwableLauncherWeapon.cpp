// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/weapon/throwerWeapon/enum/throwableEnum.h"
#include "p2/entityManager/EntityManager.h"
#include "DebugPlugin/DebugHelper.h"
#include "p2/_world/worldLevel.h"
#include "throwableLauncherWeapon.h"


AthrowableLauncherWeapon::AthrowableLauncherWeapon(){
    //constructor
    PrimaryActorTick.bCanEverTick = true;
    Type = weaponEnum::default_thrower; //a thrower by default
    
}

void AthrowableLauncherWeapon::BeginPlay(){
    Super::BeginPlay();
    FindThrowableSocketOnBeginPlay();
    FindDefaultThrowableOnBeginPlay();
    //reload(1);
    
}

void AthrowableLauncherWeapon::FindThrowableSocketOnBeginPlay(){
    if(!socketForThrowable){
        TTryAssignByNameContains<USceneComponent>(
            "throwableSocket", //must be named like that
            socketForThrowable
        );
    }
}

void AthrowableLauncherWeapon::FindDefaultThrowableOnBeginPlay(){
    if(!launchableActorPointer){
        UChildActorComponent *childActorComponent = nullptr;
        TTryAssignByNameContains<UChildActorComponent>(
            "throwableInstance", //must be named like that
            childActorComponent
        );
        if(childActorComponent){
            if(AActor* ChildActor = childActorComponent->GetChildActor()){
                if(ALaunchableItemBase *casted = Cast<ALaunchableItemBase>(ChildActor)){
                    AttachLaunchableActor(casted);

                    if(casted){
                        bulletsInMag = 1;
                    }
                }
            }
        }
    }
}





void AthrowableLauncherWeapon::AttachLaunchableActor(ALaunchableItemBase *actor){
    if(actor && socketForThrowable){
        /*actor->AttachToComponent(
            socketForThrowable,
            FAttachmentTransformRules::SnapToTargetNotIncludingScale
        );*/

        actor->AttachToComponent(
            socketForThrowable,
            FAttachmentTransformRules(
                EAttachmentRule::SnapToTarget,   // Location
                EAttachmentRule::SnapToTarget,   // Rotation
                EAttachmentRule::KeepWorld,      // Scale
                false
            )
        );
        launchableActorPointer = actor;
    }
}

void AthrowableLauncherWeapon::detachLaunchableActorPointer(){
    if(launchableActorPointer){
        launchableActorPointer->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    }
}







//update the item show and throw
void AthrowableLauncherWeapon::Tick(float deltaTime){
    Super::Tick(deltaTime);

}

bool AthrowableLauncherWeapon::shootProtected(
    FVector from, 
    FVector dir, 
    float sizeRay, 
    teamEnum ownTeam
){ 
    //team enum is ignored here
    //throw an item here instead

    /*DebugHelper::showScreenMessage("AthrowableLauncherWeapon Shoot protected try!");
    if(launchableActorPointer == nullptr){
        DebugHelper::showScreenMessage("AthrowableLauncherWeapon Shoot protected Throwable Item Invalid!", FColor::Orange);
        RequestNewThrowable();
    }
    if(launchableActorPointer == nullptr){
        DebugHelper::showScreenMessage("AthrowableLauncherWeapon Shoot protected Throwable Item STILL Invalid!", FColor::Red);
        return false;
    }*/

    if(Super::canShoot() && launchableActorPointer != nullptr){

        DebugHelper::showScreenMessage("AthrowableLauncherWeapon Shoot protected OK!");

        //DONT FORGET THESE
        abzugHinten = true;
		resetCoolTime(cooldownTime());
        bulletsInMag--;

        detachLaunchableActorPointer();
        launchableActorPointer->throwIntoDirection(from, dir);
        launchableActorPointer = nullptr; //cant control it from then anymore once thrown

        
        //if enough bullets in mag, reload from entity manager
        //RequestNewThrowable();
        shootAnimation();
        return true; // very random. Doesnt have a logic here.
    }
    return false;
}



/// @brief reloads the weapon and creates a new throwable if needed
void AthrowableLauncherWeapon::reload(int amount){
    if(amount > 0){
        DebugHelper::showScreenMessage("try reload thrower weapon");
        bulletsInMag += amount;
        RequestNewThrowableIfNeeded();
    }
}


/// @brief requests a new throwable of the own type of thrower weapon
void AthrowableLauncherWeapon::RequestNewThrowableIfNeeded(){

    //request and set reference to launchableActorPointer, must be set to nullptr to request a new one!!!
    if(Super::enoughBulletsInMag() && launchableActorPointer == nullptr){
        if(EntityManager *e = AworldLevel::entityManager()){
            FVector a = GetActorLocation();
            AttachLaunchableActor(
                e->spawnAthrowable(GetWorld(), a, throwableTypeToThrow)
            );
        }
    }
    
}




/// @brief sets the throwable type to request and throw (should be called upon instantiation)
/// @param type type to throw
void AthrowableLauncherWeapon::setThrowableType(EThrowableEnum type){
    throwableTypeToThrow = type;
}


/// @brief overriden for thrower weapon to also hide the item
/// @param show bool show or hide
void AthrowableLauncherWeapon::showWeapon(bool show){

    Super::showWeapon(show);

    //not nesecarry anymore, since attached to component.
    //unclear.

    if(launchableActorPointer != nullptr){
        launchableActorPointer->show(show);
    }
}




ammunitionEnum AthrowableLauncherWeapon::getAmmunitionType(){
    if(throwableTypeToThrow == EThrowableEnum::ERpg){
        return ammunitionEnum::rpg_ammunition;
    }
    return ammunitionEnum::rpg_ammunition;
}