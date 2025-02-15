// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanEntityScript.h"

#include "EntityScript.h"
#include "p2/player/playerScript.h"
#include "p2/pathFinding/PathFinder.h"
#include "p2/entityManager/referenceManager.h"
#include "p2/entityManager/EntityManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "p2/entityManager/OutpostManager.h"
#include "p2/entityManager/Outpost.h"
#include "p2/weapon/weaponEnum.h"
#include "p2/_world/worldLevel.h"
#include "p2/entities/skelletons/skelletonController.h"
#include "p2/gameStart/assetManager.h"
#include "p2/entities/skelletons/socketNames.h"

#include "p2/weapon/setupHelper/weaponSetupHelper.h"


// Sets default values
AHumanEntityScript::AHumanEntityScript()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}


void AHumanEntityScript::BeginPlay(){
    Super::BeginPlay(); //super methods first, will also call init there.
    weaponPointer = nullptr;
    this->init();
    DebugHelper::showScreenMessage("human start!");
}

void AHumanEntityScript::init(){
    Super::init();

    //DebugHelper::showScreenMessage("human init");
    
    //weapon currently hidden
    
    EntityManager *e = worldLevel::entityManager();
    if (e != nullptr && weaponPointer == nullptr)
    {

        //testing new helper (works as expected)
        weaponSetupHelper *setuphelper = new weaponSetupHelper();
        
        setuphelper->setWeaponTypeToCreate(weaponEnum::pistol);
        //setuphelper->setWeaponTypeToCreate(weaponEnum::assaultRifle);
        //setuphelper->setSightAttachment(weaponSightEnum::enum_reddot);

        Aweapon *w = e->spawnAweapon(GetWorld(), setuphelper);
		//showScreenMessage("begin weapon");
		if (w != nullptr){
			//showScreenMessage("human pickup weapon");
			w->pickupBot(this); //saves the pointer inside the weapon. Weapon is further managed by custom skelleton

            boneController.attachCarriedItem(w); //NEW!

            //save pointer
            weaponPointer = w;

            e->addActorToIgnoreRaycastParams(weaponPointer, teamEnum::neutralTeam);
        }

        

        delete setuphelper; //immer löschen nicht vergessen!
        setuphelper = nullptr;
    }

    //spotting
    spottedPlayer = false;
    canSeePlayer = false;

    defaultSpottingTime = 5;
	setSpottingTime(defaultSpottingTime);

    //outpost
    outpost = nullptr;

    //team
    //setTeam(referenceManager::TEAM_ENEMY);
    setTeam(teamEnum::enemyTeam);




}

void AHumanEntityScript::Tick(float DeltaTime){
    Super::Tick(DeltaTime); //entity tick (spotting, path)

    //only tick if wanted
    if(Super::isActivatedForUpdate()){
        //reload weapon
        if(weaponPointer != nullptr){
            if(!weaponPointer->enoughBulletsInMag() && weaponPointer->canReload()){
                int defaultSize = 30;
                weaponPointer->reload(defaultSize);
            }
        }

        //addition to the base entity: attack the player if in vision
        adaptWeaponToCurrentPlayerVisibilty();

        //if needed one is found
        findOutPostNearby();

        //if player spotted, enable alarm if needed
        if(outpost != nullptr && spottedPlayer){
            outpost->alarmAll();
        }
    }
}


void AHumanEntityScript::adaptWeaponToCurrentPlayerVisibilty(){
    if(canSeePlayer){
        //DebugHelper::showScreenMessage("can see", FColor::Red);
    }

    if(canSeePlayer && spottedPlayer){
        boneController.weaponAimDownSight();
        attackPlayer();
    }
    if(!canSeePlayer){
        if(!spottedPlayer){
            boneController.weaponHolsterPosition();
        }else{
            boneController.weaponContactPosition();
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
    
    if(!boneController.canChangeStateNow()){
        return;
    }

    DebugHelper::showScreenMessage("shoot bot!");

    // boneController.stopLocomotion(); //blocked debug wise
    Super::LookAt(target); // look at the target


    if(weaponPointer != nullptr){
        //will try to shoot the weapon
        weaponPointer->shootBot(target);
    }
}



/// @brief checks if the player is within a max given range or the outpost. Is a overriden method from 
/// AEntityScript
/// @param vec to check if in range
/// @return bool in range or not
bool AHumanEntityScript::isWithinMaxRange(FVector vec){

    //float dist = FVector::Dist(GetActorLocation(), vec) / 100;
    //DebugHelper::showScreenMessage(FString::Printf(TEXT("dist human %f"), dist));

    //within range of outpost (instead to check to find a path or not)
    if(outpost != nullptr){
        return outpost->isInRange(vec);
    }
    
    //default is in range
    return Super::isWithinMaxRange(vec);
    
}

/// @brief release own instance to entity manager
void AHumanEntityScript::die(){
    Super::resetpath();

    enableActiveStatus(false); //disable?

    //entity manager
    EntityManager *entityManager = worldLevel::entityManager();

    boneController.dropWeapon();

    //drop weapon and release
    if (weaponPointer != nullptr)
    {
        weaponPointer->drop();

        if(entityManager != nullptr){
            entityManager->add(weaponPointer);
        }


        weaponPointer = nullptr;
    }

    

    //release over outpost, so the outpost ca remove the entity from its own list
    if(outpost != nullptr){
        outpost->releaseEntity(this);
        outpost = nullptr;

    }else{
        //default entity manager death
        //Super::die();
        if (entityManager != nullptr){
            entityManager->add(this); //cant call entity super method because super method would add entity instead of human entity
        }
    }


}

/// @brief despawns the entity 
void AHumanEntityScript::despawn(){
    //despawn weapon manually to the entity manager too.
    if(weaponPointer != nullptr){
        weaponPointer->drop();
        if (EntityManager *e = worldLevel::entityManager()){
            e->add(weaponPointer);
        }

        weaponPointer = nullptr;
    }

    die();
}

/// @brief sets the outpost reference if is not a nullptr,
/// newly created entites usually will be created by an outpost which subscribes them
/// automatically, if an entity doesnt have an outpost,
/// the findOutPostNearby method will manange the search. The outpostmanager will
/// will find or create an outpost and also subscribe the human entity with this 
/// method.
/// @param outpostIn outpost to subscribe to
void AHumanEntityScript::setOutpost(AOutpost *outpostIn){
    if(outpostIn != nullptr){
        this->outpost = outpostIn;
    }
}

/// @brief finds an outpost nearby if needed and subscribes to it
void AHumanEntityScript::findOutPostNearby(){
    if(outpost == nullptr){
        //try find outpost nearby
        OutpostManager *instance = worldLevel::outpostManager(); // OutpostManager::instance();
        if(instance != nullptr){
            instance->tryRequestOutpost(GetWorld(), this); //manager will subscribe the entity on success
        }
    }
}

