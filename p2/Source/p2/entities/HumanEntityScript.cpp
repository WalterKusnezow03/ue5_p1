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

#include "p2/weapon/setupHelper/weaponSetupHelper.h"


// Sets default values
AHumanEntityScript::AHumanEntityScript()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}


void AHumanEntityScript::BeginPlay(){
    Super::BeginPlay(); //super methods first, will also call init there.
    this->init();

    
}

void AHumanEntityScript::init(){
    Super::init();

    //DebugHelper::showScreenMessage("human init");
    
    //weapon
    EntityManager *e = worldLevel::entityManager();
    if (e != nullptr)
    {

        //testing new helper (works as expected)
        weaponSetupHelper *setuphelper = new weaponSetupHelper();
        setuphelper->setWeaponTypeToCreate(weaponEnum::assaultRifle);
        setuphelper->setSightAttachment(weaponSightEnum::enum_reddot);

        Aweapon *w = e->spawnAweapon(GetWorld(), setuphelper);
		//showScreenMessage("begin weapon");
		if (w != nullptr){
			//showScreenMessage("human pickup weapon");
			w->pickupBot(this);

            //save pointer
            weaponPointer = w;
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





    //create skelleton and attach to this.
    if(skelletonControllerPointer == nullptr){
        if(assetManager *a = assetManager::instance()){
            UClass *humanController = a->findBp(skelletonControllerEnum::human_skelleton);

            if(humanController != nullptr){
                if(e != nullptr){ //entity manager
                    FVector location = GetActorLocation();

                    //ask entity manager for an instance (is spawned or recycled)
                    AskelletonController *controller = e->spawnAskelletonController(
                        GetWorld(),
                        location,
                        skelletonControllerEnum::human_skelleton
                    );
                    if (controller != nullptr)
                    {
                        controller->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
                        Super::skelletonControllerPointer = controller;
                        Super::skelletonControllerPointer->setOwner(this);
                        
                        DebugHelper::showScreenMessage("created human controller!");
                    }
                }
            }
        }
    }

    //attach weapon to skelleton
    if(weaponPointer != nullptr && skelletonControllerPointer != nullptr){
        //detach from bot, testing!
        skelletonControllerPointer->detach(weaponPointer); //detach completly from everything
        //skelletonControllerPointer->attachToRightArm(weaponPointer);
        skelletonControllerPointer->attachToBreastRight(weaponPointer);

        //new attach left hand
        skelletonControllerPointer->attachLeftArmTo(weaponPointer, "none");
    }
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
        if(canSeePlayer && spottedPlayer){
            attackPlayer();
        }

        //if needed one is found
        findOutPostNearby();

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

    //drop weapon from skelleton and release the skelleton 
    if(skelletonControllerPointer != nullptr){
        if(weaponPointer != nullptr){
            skelletonControllerPointer->detach(weaponPointer);
        }
        skelletonControllerPointer->die(); // will deatch from actor and also released
        skelletonControllerPointer = nullptr;
    }

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

