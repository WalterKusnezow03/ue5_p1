// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanEntityScript.h"

#include "playerScript.h"
#include "PathFinder.h"
#include "referenceManager.h"
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
    Super::BeginPlay(); //super methods first


    //testing weapons
	referenceManager *instance = referenceManager::instance();
	if(instance != nullptr){
		Aweapon *w = instance->spawnAweapon(GetWorld());
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