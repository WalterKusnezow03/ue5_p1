// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanEntityScript.h"

#include "EntityScript.h"
#include "p2/playerScript.h"
#include "p2/pathFinding/PathFinder.h"
#include "p2/referenceManager.h"
#include "p2/entityManager/EntityManager.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AHumanEntityScript::AHumanEntityScript()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	health = 100;
	spottedPlayer = false;
	playerPointer = nullptr;
	defaultSpottingTime = 5;
	setSpottingTime(defaultSpottingTime);
}


void AHumanEntityScript::BeginPlay(){
    Super::BeginPlay(); //super methods first, will also call init there.


    EntityManager *e = EntityManager::instance();
    if(e != nullptr){
        Aweapon *w = e->spawnAweapon(GetWorld());
		showScreenMessage("begin weapon");
		if (w != nullptr){
			showScreenMessage("pickup weapon");
			w->pickupBot(this);

            //save pointer
            weaponPointer = w;
        }
    }
}


void AHumanEntityScript::Tick(float DeltaTime){
    Super::Tick(DeltaTime);

    if(Super::isActivatedForUpdate()){
        //reload weapon
        if(weaponPointer != nullptr){
            if(!weaponPointer->enoughBulletsInMag()){
                int defaultSize = 30;
                weaponPointer->reload(defaultSize);
            }
        }

        //addition to the base entity: attack the player if in vision
        if(canSeePlayer && spottedPlayer){
            attackPlayer();
        }
    }
    
}

/// @brief attack the player if playerpointer not nullptr
void AHumanEntityScript::attackPlayer(){
    if(playerPointer != nullptr){
        shootAt(playerPointer->GetActorLocation());
    }
}

/// @brief shoot at a specified target
/// @param target 
void AHumanEntityScript::shootAt(FVector target){
    Super::LookAt(target); //look at the target

    if(weaponPointer != nullptr){
        //will try to shoot the weapon
        weaponPointer->shootBot(target);
    }
}





//activate de activate die method
void AHumanEntityScript::die(){
    if(weaponPointer != nullptr){
        weaponPointer->dropweapon();
    }
    Super::die();
}