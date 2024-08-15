// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanEntityScript.h"

#include "EntityScript.h"
#include "p2/playerScript.h"
#include "p2/pathFinding/PathFinder.h"
#include "p2/referenceManager.h"
#include "p2/entityManager/EntityManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "p2/entityManager/OutpostManager.h"
#include "p2/entityManager/Outpost.h"


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
    this->init();
}

void AHumanEntityScript::init(){
    Super::init();

    DebugHelper::showScreenMessage("human init");
    
    //weapon
    EntityManager *e = EntityManager::instance();
    if(e != nullptr){
        Aweapon *w = e->spawnAweapon(GetWorld());
		showScreenMessage("begin weapon");
		if (w != nullptr){
			showScreenMessage("human pickup weapon");
			w->pickupBot(this);

            //save pointer
            weaponPointer = w;
        }
    }

    //outpost
    outpost = nullptr;
}

void AHumanEntityScript::Tick(float DeltaTime){
    Super::Tick(DeltaTime);

    if(Super::isActivatedForUpdate()){
        //reload weapon
        if(weaponPointer != nullptr){
            if(!weaponPointer->enoughBulletsInMag() && weaponPointer->canReload()){
                int defaultSize = 30;
                weaponPointer->reload(defaultSize);
            }
        }

        //addition to the base entity: attack the player if in vision
        if(canSeePlayer && spottedPlayer){
            attackPlayer();
        }

        findOutPostNearby(); // if needed one is found

        //if player spotted, enable alarm if needed
        if(outpost != nullptr && spottedPlayer){
            outpost->alarmAll();
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





/// @brief release own instance to entity manager
void AHumanEntityScript::die(){
    if(weaponPointer != nullptr){
        weaponPointer->drop();
        weaponPointer = nullptr;
    }

    if(outpost != nullptr){
        outpost->releaseEntity(this);
        outpost = nullptr;

    }else{
        //default entity manager death
        Super::die();
    }
}


/// @brief sets the outpost reference if not a nullptr
/// is a set method because newly created entites usually will be created by an outpost which subscribes them
/// automatically
/// @param outpostIn 
void AHumanEntityScript::setOutpost(AOutpost *outpostIn){
    if(outpostIn != nullptr){
        this->outpost = outpostIn;
    }
}

/// @brief finds an outpost nearby if needed and subscribes to it
void AHumanEntityScript::findOutPostNearby(){
    if(outpost == nullptr){
        //find outpost nearby 
        //outpost manager needs to be implemented yet.
        OutpostManager *instance = OutpostManager::instance();
        if(instance != nullptr){
            instance->tryRequestOutpost(GetWorld(), this);
        }
    }
}

