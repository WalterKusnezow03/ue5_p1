// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetEnumCollection/assetEnums/throwableEnum.h"
#include "p2/throwableItems/throwableItem.h"
#include "p2/entityManager/EntityManager.h"
#include "DebugPlugin/DebugHelper.h"
#include "p2/_world/worldLevel.h"
#include "p2/weapon/throwerWeapon.h"


AthrowerWeapon::AthrowerWeapon(){
    //constructor
    PrimaryActorTick.bCanEverTick = true;
    Type = weaponEnum::thrower; //a thrower by default
    throwableTypeToThrow = throwableEnum::greneade_enum;
    
}

void AthrowerWeapon::BeginPlay(){
    Super::BeginPlay();
    reload(1);
}



//update the item show and throw
void AthrowerWeapon::Tick(float deltaTime){

    Super::Tick(deltaTime);

    //DebugHelper::showScreenMessage("AthrowerWeapon Tick");

    //update the position of the item to throw if has bullets left
    if(throwableActorPointer != nullptr){
        throwableActorPointer->SetActorLocation(GetActorLocation());
        throwableActorPointer->SetActorRotation(GetActorRotation());
    }
}

bool AthrowerWeapon::shootProtected(FVector from, FVector dir, float sizeRay, teamEnum ownTeam){ //team enum is ignored here
    //throw an item here instead

    DebugHelper::showScreenMessage("AthrowerWeapon Shoot protected try!");
    if(throwableActorPointer == nullptr){
        DebugHelper::showScreenMessage("AthrowerWeapon Shoot protected Throwable Item Invalid!", FColor::Orange);
        requestNewThrowable();
    }
    if(throwableActorPointer == nullptr){
        DebugHelper::showScreenMessage("AthrowerWeapon Shoot protected Throwable Item STILL Invalid!", FColor::Red);
    }


    

    if(Super::canShoot() && throwableActorPointer != nullptr){

        DebugHelper::showScreenMessage("AthrowerWeapon Shoot protected OK!");

        //DONT FORGET THESE
        abzugHinten = true;
		resetCoolTime(cooldownTime());
        bulletsInMag--;

        //FVector connect = to - from; //AB = B - A
        //connect = connect.GetSafeNormal(); //dir normalized

        FVector connect = dir.GetSafeNormal();

        throwableActorPointer->throwIntoDirection(from, connect);

        throwableActorPointer = nullptr; //cant control it from then anymore once thrown

        
        //if enough bullets in mag, reload from entity manager
        requestNewThrowable();
        return true; //very random. Doesnt have a logic here.
    }
    return false;
}

/// @brief requests a new throwable of the own type of thrower weapon
void AthrowerWeapon::requestNewThrowable(){

    //request and set reference to throwableActorPointer, must be set to nullptr to request a new one!!!
    if(Super::enoughBulletsInMag() && throwableActorPointer == nullptr){
        //request
        throwableActorPointer = nullptr;

        if(EntityManager *e = AworldLevel::entityManager()){
            FVector a = GetActorLocation();
            throwableActorPointer = e->spawnAthrowable(GetWorld(), a, throwableTypeToThrow);
        }
        
    }
    
}

/// @brief reloads the weapon and creates a new throwable if needed
void AthrowerWeapon::reload(int amount){
    if(amount > 0){
        DebugHelper::showScreenMessage("try reload thrower weapon");

        int copy = bulletsInMag; //need to create a copy here because the request new throwable method
        //is using the enough bullets in mag method!
        bulletsInMag += amount;

        // was empty: create a new one
        if(copy <= 0){
            requestNewThrowable(); //request a new throwable if had none
        }

        

        //Super::reload(amount);
    }
}






/// @brief sets the throwable type to request and throw (should be called upon instantiation)
/// @param type type to throw
void AthrowerWeapon::setThrowableType(throwableEnum type){
    throwableTypeToThrow = type;
}


/// @brief overriden for thrower weapon to also hide the item
/// @param show bool show or hide
void AthrowerWeapon::showWeapon(bool show){

    Super::showWeapon(show);

    if(throwableActorPointer != nullptr){
        throwableActorPointer->show(show);
    }
}




ammunitionEnum AthrowerWeapon::getAmmunitionType(){
    switch(throwableTypeToThrow){
    case throwableEnum::greneade_enum:
        return ammunitionEnum::grenade_ammunition;
    case throwableEnum::molotov_enum:
        return ammunitionEnum::molotov_ammunition;
    }
    return ammunitionEnum::grenade_ammunition;
}