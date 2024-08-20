// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/throwableItems/throwableEnum.h"
#include "p2/throwableItems/throwableItem.h"
#include "p2/weapon/throwerWeapon.h"


AthrowerWeapon::AthrowerWeapon(){
    //constructor

    throwableTypeToThrow = throwableEnum::greneade_enum;
    requestNewThrowable();
}

void AthrowerWeapon::BeginPlay(){
    Super::BeginPlay();

    //load the item to show
    cooldownTime = 1.5f;
    reloadTime = 1.5f;

    //test
    bulletsInMag = 10;

    singleFireModeOn = true;
}



//update the item show and throw
void AthrowerWeapon::Tick(float deltaTime){

    Super::Tick(deltaTime);


    //update the position of the item to throw if has bullets left
    if(throwableActorPointer != nullptr){
        throwableActorPointer->SetActorLocation(GetActorLocation());
        throwableActorPointer->SetActorRotation(GetActorRotation());
    }
}

void AthrowerWeapon::shootProtected(FVector from, FVector to, int ownTeam){
    //throw an item here instead

    if(Super::canShoot() && throwableActorPointer != nullptr){
        //DONT FORGET THESE
        abzugHinten = true;
		resetCoolTime(cooldownTime);
        bulletsInMag--;

        FVector connect = to - from; //AB = B - A
        connect = connect.GetSafeNormal(); //dir normalized

        throwableActorPointer->throwIntoDirection(from, connect);

        throwableActorPointer = nullptr; //cant control it from then anymore once thrown

        
        //if enough bullets in mag, reload from entity manager
        requestNewThrowable();
    }

}


/// @brief requests a new throwable of the own type of thrower weapon
void AthrowerWeapon::requestNewThrowable(){

    //request and set reference to throwableActorPointer, must be set to nullptr to request a new one!!!
    if(Super::enoughBulletsInMag() && throwableActorPointer == nullptr){
        //request
        throwableActorPointer = nullptr;

        if(EntityManager *e = EntityManager::instance()){
            throwableActorPointer = e->spawnAthrowable(GetWorld(), GetActorLocation(), throwableTypeToThrow);
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