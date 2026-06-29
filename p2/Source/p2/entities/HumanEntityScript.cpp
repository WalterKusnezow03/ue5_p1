// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanEntityScript.h"

#include "EntityScript.h"
#include "p2/player/playerScript.h"
#include "PathFinder/pathFinding/PathFinder.h"
 #include "p2/entityManager/EntityManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "p2/entityManager/OutPost/OutpostManager.h"
#include "p2/entityManager/OutPost/Outpost.h"
#include "AssetEnumCollection/assetEnums/weaponEnum.h"
#include "p2/_world/worldLevel.h"
#include "p2/entityManager/AlertManager.h"
#include "AssetPlugin/gamestart/assetManager.h"
#include "p2/entities/botActionHelper/EAttackType.h"
#include "DebugPlugin/DebugHelper.h"
#include "GameCore/PlayerInfo/PlayerInfo.h"

#include "p2/weapon/setupHelper/weaponSetupHelper.h"
#include "IkHumanoidModell/carryItems/enum/ECarriedItemPosition.h"

#include "PathfinderNNExtension/Connection/NNPathFinderSocket.h"


// Sets default values
AHumanEntityScript::AHumanEntityScript() : AEntityScript()
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
    FVector location = GetActorLocation();
    init(location);
}

void AHumanEntityScript::init(FVector &location){

    Super::init(location); //will setup default vars and preoject skelleton to ground.

    attackTypeOfBot = EAttackType::EAssault;

    //DebugHelper::showScreenMessage("human init");
    
    //weapon currently hidden
    EntityManager *e = AworldLevel::entityManager();
    if (e != nullptr && weaponPointer == nullptr)
    {

        //testing new helper (works as expected)
        weaponSetupHelper setuphelper;

        setuphelper.setWeaponTypeToCreate(weaponEnum::pistol);
        //setuphelper->setWeaponTypeToCreate(weaponEnum::assaultRifle);


        Aweapon *w = e->spawnAweapon(GetWorld(), &setuphelper);
		//showScreenMessage("begin weapon");
		if (w != nullptr){


            //showScreenMessage("human pickup weapon");
			w->pickupBot(this); //saves the pointer inside the weapon. Weapon is further managed by custom skelleton

            //attach to bone controller
            humanoidPluginController.dropCarriedItem();
            humanoidPluginController.attachOrReplaceCarriedItem(w);

            //save pointer
            weaponPointer = w;

            e->addActorToIgnoreRaycastParams(weaponPointer, teamEnum::neutralTeam);
        }
    }


    //outpost
    outpost = nullptr;

    //team
    setTeam(teamEnum::enemyTeam);




}


/// ----- todo: ------


FVisionCone &AHumanEntityScript::GetVisionCone(){
    //TODO:
    FVector forward = humanoidPluginController.lookDirection();		 
    FVector currentLocation = humanoidPluginController.GetLocation();

    visionCone.Update(currentLocation,forward, VisionAngleDegree());
    
    return visionCone;
}






void AHumanEntityScript::Tick(float DeltaTime){
    Super::Tick(DeltaTime); //entity tick (spotting, path)
    if(AworldLevel::gameStateManager.GameStateIsPaused()){
		return;
	}

    //only tick if wanted
    if(Super::isActivatedForUpdate()){

        reloadOwnWeaponIfNeeded();

        //addition to the base entity: attack the player if in vision
        //adaptWeaponToCurrentPlayerVisibilty();
        PerformActionsBasedOnPlayerVisibility();

        //if needed one is found
        findOutPostNearby();

        //if player spotted, enable alarm if needed
        if(outpost != nullptr && spottedPlayer){
            outpost->alarmAll();
        }
    }
}


void AHumanEntityScript::reloadOwnWeaponIfNeeded(){
    //reload weapon
    if(weaponPointer != nullptr){
        if(!weaponPointer->enoughBulletsInMag() && weaponPointer->canReload()){
            int defaultSize = 30;
            weaponPointer->reload(defaultSize);
        }
    }
}

void AHumanEntityScript::PerformActionsBasedOnPlayerVisibility(){
    adaptWeaponToCurrentPlayerVisibilty(); 

    //any bot can share memory whether player was seen
    FlagPlayerVisibleToNNInterface();
    RequestPlayerPredictionFromNNInterface();
}

void AHumanEntityScript::adaptWeaponToCurrentPlayerVisibilty(){
    if(canSeePlayer && spottedPlayer){

        

        humanoidPluginController.changeCarriedItemSocket(ECarriedItemPosition::EAimDownSightPosition);
        humanoidPluginController.stopLocomotionOnceRotationHasFinished();
        attackPlayer();

        //clear path if can see player to attack
        Super::resetpath(); //clear path, might change!
    }
    if(!canSeePlayer){
        if(!spottedPlayer){
            humanoidPluginController.changeCarriedItemSocket(ECarriedItemPosition::EHipPosition);
        }else{
            humanoidPluginController.changeCarriedItemSocket(ECarriedItemPosition::ETorsoPosition); //contact state
        }
    }
}


void AHumanEntityScript::FlagPlayerVisibleToNNInterface(){
    if(playerPointer && canSeePlayer){
        FlagActorVisibleToNNInterface(playerPointer);
    }
}

void AHumanEntityScript::FlagActorVisibleToNNInterface(AActor *actor){
    if(actor){
        if(ANNPathFinderSocket *nnSocket = ANNPathFinderSocket::PathFinderNNinstance()){
            nnSocket->FlagVisible(actor);
        }
    }
}

void AHumanEntityScript::RequestPlayerPredictionFromNNInterface(){
    if(!canSeePlayer && playerPointer){
        if(ANNPathFinderSocket *nnSocket = ANNPathFinderSocket::PathFinderNNinstance()){
            nnSocket->PredictNode(this, playerPointer);
        }
    }
}




//process requested positions
void AHumanEntityScript::ResponseNNPositions(const TArray<FVector> &positions){
    if(positions.Num() > 0){
        FString message = FString::Printf(
            TEXT("AHumanEntityScript::ResponseNNPositions Receive Response (%d)"),
            positions.Num()
        );
        DebugHelper::showScreenMessage(message, FColor::Orange);
        DebugHelper::logMessage(message);

        for (int i = 0; i < positions.Num(); i++){
            FVector current = positions[i];
            FVector actorLocation = humanoidPluginController.GetLocation();
            current.Z = actorLocation.Z;

            DebugHelper::showLineBetween(
                GetWorld(),
                current,
                actorLocation,
                FColor::Green,
                1.0f
            );
        }

        //process check if visible at all

        //notifiy team
        
    }
}




/// @brief attack the player if playerpointer not nullptr
void AHumanEntityScript::attackPlayer(){
    if(playerPointer != nullptr){
        if(playerIsInLookDir()){
            shootAt(playerPointer->GetActorLocation());
        }
    }
}

/// @brief shoot at a specified target
/// @param target 
void AHumanEntityScript::shootAt(FVector target){
    
    // To be implemented for humanoid controller
    //if(!boneController.canChangeStateNow()){
    //    return;
    //}

    
    Super::LookAt(target); // look at the target


    if(weaponPointer != nullptr){
        //will try to shoot the weapon
        //DebugHelper::showScreenMessage("shoot bot!");
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
    if(AAlertManager *instance = AAlertManager::Instance(GetWorld())){
        instance->unSubscribeFromAlert(this);
    }
    enableActiveStatus(false); //disable?
    UnRegisterFromMiniMap();
    RequestInteractWidget(); //also setups payload for death

    DebugHelper::showScreenMessage("AHumanEntityScript::DIE", FColor::Red);
    

    humanoidPluginController.dropCarriedItem();
    humanoidPluginController.SetStateCollapseTrue();

    //entity manager
    EntityManager *entityManager = AworldLevel::entityManager();

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
    if(AworldLevel::DebugSkelletonRecordMode()){
        return;
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

void AHumanEntityScript::clearOutpost(){
    outpost = nullptr;
}

/// @brief finds an outpost nearby if needed and subscribes to it
void AHumanEntityScript::findOutPostNearby(){
    if(bDebugBlockOutpostRequest){
        return;
    }

    if(outpost == nullptr){
        //try find outpost nearby
        OutpostManager *instance = AworldLevel::outpostManager(); // OutpostManager::instance();
        if(instance != nullptr){
            instance->tryRequestOutpost(GetWorld(), this); //manager will subscribe the entity on success
        }
    }
}



/// OVERRIDEN
void AHumanEntityScript::requestNewPathTo(FVector &targetLocation, bool towardsPlayer){
    if(attackTypeIs(EAttackType::ESniper) && towardsPlayer){
        APathFinder *p = APathFinder::instance();
        if(p != nullptr){
            FVector newTarget = p->findFurthestConnectedNodeFrom(targetLocation);
            Super::requestNewPathTo(newTarget, towardsPlayer);
            return;
        }
    }

    if(towardsPlayer){
        if(outpost != nullptr){
            //will modify the target based on team leader or not
            outpost->validatePlayerTargetMovingPosition(
                this,
                targetLocation
            );
        }
    }
    //any case if not allowed:
    DebugHelper::showScreenMessage("NEW PATH REQUEST", FColor::Red);
    Super::requestNewPathTo(targetLocation, towardsPlayer);
}

bool AHumanEntityScript::attackTypeIs(EAttackType type){
    return type == attackTypeOfBot;
}



bool AHumanEntityScript::playerIsInLookDir(){
    
    FVector playerLocation = PlayerInfo::playerLocation();
    FVector connectToPlayer = playerLocation - humanoidPluginController.GetLocation(); // AB = B - A
    connectToPlayer.Z = 0.0f;

    connectToPlayer = connectToPlayer.GetSafeNormal();
    FVector forward = humanoidPluginController.lookDirection();
    forward.Z = 0.0f;
    forward = forward.GetSafeNormal();

    return FVector::DotProduct(connectToPlayer, forward) >= 0.6f; //0 orthogonal, 1 parallell
}





///@brief overriden alert method: if look at is near enough: exit path,
/// look at! 
void AHumanEntityScript::alert(FVector lookat){
    Super::alert(lookat);
    if (!isActivatedForUpdate()){
		return;
	}

    // special attention needed: clear path, look at
    if(isWithinCloseRange(lookat)){
        Super::resetpath();
        LookAt(lookat);
    }
}





void AHumanEntityScript::SetupLootDeathPayload(){
    Super::SetupLootDeathPayload();
    if(deathLootPayload){
        deathLootPayload->Setup(weaponPointer);
    }
    
}