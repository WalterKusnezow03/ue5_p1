// Fill out your copyright notice in the Description page of Project Settings.


#include "playerScript.h"
#include "p2/weapon/weapon.h"
#include "playerInventory.h"
#include "p2/entityManager/EntityManager.h"

#include "p2/entityManager/referenceManager.h"
#include "DebugPlugin/DebugHelper.h"
#include "Animation/AnimSequence.h"
#include "GameCore/team/teamEnum.h"
#include "p2/weapon/setupHelper/LoadoutHelper.h"
#include "p2/weapon/setupHelper/weaponSetupHelper.h"
#include <cmath>
#include "GameCore/interfaces/Interactinterface.h"
#include "GameFramework/Character.h" // Falls noch nicht inkludiert
#include "GameFramework/CharacterMovementComponent.h"
#include "AssetEnumCollection/assetEnums/weaponAttachmentEnum.h"
#include "Components/CapsuleComponent.h" // Include for UCapsuleComponent
#include "Camera/CameraComponent.h" // Include for UCameraComponent
#include "p2/player/minimap/Minimap.h"
#include "PathFinder/pathFinding/PathFinder.h"
#include "p2/vehicles/vehicle/base/vehicleBase.h"
#include "GameCore/PlayerInfo/PlayerInfo.h"

#include "p2/_world/worldLevel.h"

#include "p2/ui/PlayerUi.h"


// Sets default values
AplayerScript::AplayerScript() : APlayerControllerBase()
{
    aiming = false;
}

// Called when the game starts or when spawned
void AplayerScript::BeginPlay()
{
	Super::BeginPlay();
    referenceManager *i = referenceManager::instance();
	if(i){
		i->setPlayerReference(this);
	}

    
    setTeam(teamEnum::playerTeam);
    setupBoneController(); 

    EntityManager *entityMananger = AworldLevel::entityManager();
    Aweapon *weapon = nullptr;
    if(entityMananger != nullptr){
        //w = e->spawnAweapon(GetWorld(), throwableEnum::greneade_enum);

        
        weapon = entityMananger->spawnAweapon(GetWorld(), weaponEnum::assaultRifle);
        if(weapon != nullptr){
            weapon->applySight(weaponAttachmentEnum::reddot);
            pickUpWeaponIntoInventoryIfNeededAndAttachToBoneController(weapon);
        }

        entityMananger->addActorToIgnoredAllParams(this); //skelleton may not walk on player.
    }

    CreateUiHudActorOnBeginPlay();
    createMiniMap();
}

void AplayerScript::CreateUiHudActorOnBeginPlay(){
    FVector relativeLocation(50, 0, 0);
    AHudUiActor::MakeInstance(GetWorld(), CameraComponent, relativeLocation);
}

// Called to bind functionality to input
void AplayerScript::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent); //very important!

    //other player controls in Super Class!

    PlayerInputComponent->BindAction("key1", IE_Pressed, this, &AplayerScript::keydown1);
    PlayerInputComponent->BindAction("key2", IE_Pressed, this, &AplayerScript::keydown2);
    PlayerInputComponent->BindAction("key3", IE_Pressed, this, &AplayerScript::keydown3);
    PlayerInputComponent->BindAction("key4", IE_Pressed, this, &AplayerScript::keydown4);

}



void AplayerScript::switchCamera(){
    //return;

    if(isCamInPlayer){
        cameraDebugFpv();
        isCamInPlayer = false;
    }
    else
    {
        cameraDefaultFpv();
        isCamInPlayer = true;
    }
}

void AplayerScript::cameraDefaultFpv(){
    if(CameraComponent){
        CameraComponent->SetRelativeLocation(cameraReltiveLocationOriginal); // Position the camera
	    CameraComponent->bUsePawnControlRotation = true;
        CameraComponent->SetRelativeRotation(FRotator(0, 0, 0)); // Look downward
    }
}

void AplayerScript::cameraDebugFpv(){
    if(CameraComponent){
        CameraComponent->SetRelativeLocation(FVector(-200.0f, 0, 400.0f)); // Position the camera
	    CameraComponent->bUsePawnControlRotation = true;
        CameraComponent->SetRelativeRotation(FRotator(-70, 0, 0)); // Look downward
    }
}



void AplayerScript::debugPathFinder(){
    FVector posA(0, 0, 200);
    FVector own = GetActorLocation();

    DebugHelper::showLineBetween(GetWorld(), posA, own, FColor::Yellow);

    APathFinder *in = APathFinder::instance();
    if(in != nullptr){
        std::vector<FVector> drawPath = in->getPath(posA, own);
        for (int i = 1; i < drawPath.size(); i++){
            FVector a = drawPath.at(i - 1);
            FVector b = drawPath.at(i);

            a.Z = own.Z;
            b.Z = own.Z;
            for (int j = 0; j < 30; j++)
            {
                DebugHelper::showLineBetween(GetWorld(), a + FVector(0,0,j), b + FVector(0,0,j), FColor::Green);
            }
        }
    }
}


// Called every frame
void AplayerScript::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    



    shoot(); //shoot the weapon if needed or release. Method handles both automatically
    TickWeaponSway();

    TickBoneController(DeltaTime);
    resetFlagsOnTick();
    //TickWingsuitTimer(DeltaTime);

    TickUpdateWingsuit(DeltaTime);

    updateUi();
    TickMiniMap();


}

void AplayerScript::TickBoneController(float DeltaTime){

    //return;

    //override rotation and location for now
    FVector pos = GetActorLocation();
    FRotator rot = GetActorRotation();
    rot.Pitch = 0.0f;
    rot.Roll = 0.0f;


    if(CameraComponent){
        boneController.OverrideTransformAndCamera(
            *CameraComponent,
            pos,
            rot
        );
    }
    boneController.Tick(DeltaTime);

    /*
    boneController.debugUpdateTransform(pos, rot);
    //update some of the states based on the camera
    if(CameraComponent != nullptr){
        boneController.updateStatesBasedOnCamera(*CameraComponent);
    }

    boneController.Tick(DeltaTime, GetWorld());
    */

    
    //boneController.debugDrawHeadForward(GetWorld(), DeltaTime);
}




void AplayerScript::resetFlagsOnTick(){
    isWalking = false;
}


//Movement
/**
 * moves the player forward
 */
void AplayerScript::MoveForward(float Value)
{
    Super::MoveForward(Value);
    
    if(IsPaused()){
        //DebugHelper::showScreenMessage("PlayerSctrip: Move Forward PAUSED", FColor::Red);
        return;
    }
 
    //prints as expected
    //DebugHelper::showScreenMessage("PlayerSctrip: Move Forward", FColor::Green);
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        boneController.weaponRaisedReadyPosition(sprinting); //testing needed
    }
}

void AplayerScript::MoveRight(float Value)
{
    Super::MoveRight(Value);
}





void AplayerScript::Jump(){
    Super::Jump();
    if(IsPaused())
    {
        return;
    }

    if (CanJump()){
        //ACharacter::Jump(); // Calls the base class jump function

        if(!wingsuitInterface.wingsuitIsOpenFlag()){
            wingsuitInterface.setWingsuitTimerOnJump();
        }
    }

    //debug
    if(false){
        switchCamera();
    }
    
    
    
}


/**
 * allows the player to interact (for example picking up a weapon by pressing "E")
 */
void AplayerScript::interact(){
    if(IsPaused()){
        return;
    }
	Super::interact(); //performs Raycast and sets AActor* interactedActorPointer!!

    if(interactedActorPointer){
        Aweapon *weapon = Cast<Aweapon>(interactedActorPointer);
        if(weapon){
            pickUpWeaponIntoInventoryIfNeededAndAttachToBoneController(weapon);
            return;
        }

        AvehicleBase *vehicle = Cast<AvehicleBase>(interactedActorPointer);
        if(vehicle != nullptr){
            vehicle->seatAsDriver(this); //interface
            return;
        }
    }
    interactedActorPointer = nullptr; //reset
}



/// @brief drops the weapon from the inventory and bone controller and attaches the new weapon if
/// possible
void AplayerScript::drop(){
    if(IsPaused()){
        return;
    }
    Super::drop();
    //boneController.dropWeapon();
    playerInventory.dropWeapon();

    
    boneController.dropCarriedItem();



    Aweapon *otherWeaponNow = playerInventory.getItemPointer();
    if(otherWeaponNow != nullptr){
        //boneController.attachCarriedItem(otherWeaponNow);
        boneController.attachOrReplaceCarriedItem(otherWeaponNow);
    }
}

void AplayerScript::reload(){
    if(IsPaused()){
        return;
    }
    Super::reload();
    playerInventory.reloadWeapon();
}

void AplayerScript::aim(){
    Super::aim();
    if(IsPaused()){
        return;
    }
    
    playerInventory.aim(aiming);

    
    if(playerInventory.CurrentWeaponHasAimDisplacement()){
        boneController.weaponAimDownSight(aiming);
    }


    //boneController.weaponAimDownSight(aiming);
}

/**
 * shoot the weapon if needed or release. Method handles this automatically
 */
void AplayerScript::shoot(){
    if(IsPaused())
    {
        //DebugHelper::showScreenMessage("GAME IS PAUSED!", FColor::Orange)        
        return;
    }

    if(holding){ //checks if holding mouse down
        
        playerInventory.shoot();
        float recoil = playerInventory.recoilValue(); 
        DebugHelper::logMessageFloat("WeaponRecoil", recoil); //ok
        addPendingRecoil(recoil * 10.0f);
    }else{
        playerInventory.releaseShoot(); //abzug loslassen
    }
}





void AplayerScript::keydown1(){
    switchToIndex(0);
}
void AplayerScript::keydown2(){
    switchToIndex(1);
}
void AplayerScript::keydown3(){
    switchToIndex(2);
}
void AplayerScript::keydown4(){
    switchToIndex(3);
}

void AplayerScript::switchToIndex(int index){
    if(IsPaused()){
        return;
    }
    if(playerInventory.currentIndexNum() != index){
        //find index weapon in inventory, pickup

        Aweapon *targetedWeapon = playerInventory.getItemPointerAtIndex(index);
        if(targetedWeapon != nullptr){
            //bone controller pickup
            pickUpWeaponIntoInventoryIfNeededAndAttachToBoneController(
                targetedWeapon
            );
        }
    }
}


void AplayerScript::TickWeaponSway(){
    if(IsPaused()){
        return;
    }
    Aweapon *current = playerInventory.getItemPointer();
    if(current != nullptr){
        current->updateSwayEnabledStatus(isWalking);
    }
}








void AplayerScript::setupBoneController(){

    boneController.defaultSetup(GetWorld());

    //keine ahnung ob das was bringt.
    if(CameraComponent){
        float legScaleCM = 100;
        float armScaleCM = 100;
        float allScale = legScaleCM + armScaleCM;
        float allHalf = allScale / 2.0f;
        allHalf += 5.0f; //add up

        int pushForward = 10;
        cameraReltiveLocationOriginal = FVector(pushForward, 0, allHalf);
        CameraComponent->SetRelativeLocation(cameraReltiveLocationOriginal);
    }


}


AActor *AplayerScript::createLimbPivotAtTop(int x, int y, int height, int pushFront){

	height *= -1; //orient downwardss
	

	EntityManager *entitymanagerPointer = AworldLevel::entityManager();
	if(entitymanagerPointer != nullptr){
		FVector location(0, 0, 0);
		AcustomMeshActor *oberschenkel = entitymanagerPointer->spawnAcustomMeshActor(GetWorld(), location);
		if(oberschenkel != nullptr){
			//int width = 10;
			//int height = -(legScaleCM / 2);

			float xHalf = x / 2.0f;
			float yHalf = y / 2.0f;

			FVector a(-xHalf + pushFront, -yHalf,0);
			FVector b(xHalf + pushFront, -yHalf, 0);
			FVector c(xHalf + pushFront, yHalf,0);
			FVector d(pushFront, yHalf,0);


			FVector at(-xHalf + pushFront, -yHalf, height);
			FVector bt(xHalf + pushFront, -yHalf, height);
			FVector ct(xHalf + pushFront, yHalf, height);
			FVector dt(pushFront, yHalf, height);

			oberschenkel->createCube(
				a,b,c,d,at,bt,ct,dt,
				materialEnum::wallMaterial
			);
            oberschenkel->disablePhysicscollision();

            entitymanagerPointer->addActorToIgnoreRaycastParams(
				this, getTeam()
			);

			oberschenkel->setDamagedOwner(this);



			return oberschenkel;
		}
	}
	return nullptr;
}







/**
 * 
 * ---- wingsuit helpers new ----
 * 
 */
void AplayerScript::TickUpdateWingsuit(float DeltaTime){
    wingsuitInterface.Tick(GetWorld(), *this, DeltaTime);

    //update gravity in any case from wingsuit class
    if(GetCharacterMovement()){
        //gravity based on inner state
        GetCharacterMovement()->GravityScale = wingsuitInterface.currentGravityMultiplier(); 
    }

    if(wingsuitInterface.wingsuitIsOpenFlag()){
        boneController.openWingsuit();
        addWingsuitVelocity(DeltaTime);
        if(isCamInPlayer){
            switchCamera();
        }
    }else{
        boneController.closeWingsuit();
        if(!isCamInPlayer){
            switchCamera();
        }

        
    }
}

void AplayerScript::addWingsuitVelocity(float DeltaTime){
    if(CameraComponent){
        FVector CameraForward = CameraComponent->GetForwardVector();
        CameraForward.Z = 0.0f;

        FVector location = GetActorLocation();
        location += CameraForward * WINGSUIT_SPEED * DeltaTime;
        SetActorLocation(location);
    }
}




/**
 * 
 * --- ui ---
 * 
 */

/// @brief overrides base, return if game is paused in game state manager
/// @return 
bool AplayerScript::IsPaused(){
    return AworldLevel::gameStateManager.GameStateIsPaused();
}

void AplayerScript::openPauseMenu(){
    Super::openPauseMenu();
    AworldLevel::gameStateManager.SwitchGameStatePausedAndChangeUi();
}


void AplayerScript::updateUi(){
    updateAmmunitionUi();
    updateHealthUi();
}

void AplayerScript::updateAmmunitionUi(){
    
    
    //void updateAmmunition(int number);
    AworldLevel::playerStatusManager.updateAmmunition(
        playerInventory.currentAmmunition(),
        playerInventory.currentLeftAmmnutionInMag()
    );

}


void AplayerScript::updateHealthUi(){
    AworldLevel::playerStatusManager.updateHealth(health);
}

void AplayerScript::updatePlayerEnteredAreaUi(bool entered){
    if(UPlayerUi *uiInstance = UPlayerUi::currentInstance()){
        FString message = entered ? "Outpost Area Entered" : "Outpost Area left";

        if(entered){
            uiInstance->updateMissionText(message);
        }else{
            uiInstance->updateMissionTextTimed(message);
        }
    }
}





/**
 * public Api: player ui reload loadout api
 */
void AplayerScript::reloadLoadout(LoadoutHelper &loadout){

    //drop current weapon from bone controller
    //boneController.dropWeapon();
    boneController.dropCarriedItem();

    //clear inventory
    playerInventory.dropAllWeaponsToObjectPool();

    //get all new
    std::vector<Aweapon *> newWeapons = loadout.spawnAllWeaponsAndApplyAttachments(GetWorld());
    
    //debug
    DebugHelper::logMessage(
        FString::Printf(TEXT("AplayerScript reload loadout weapons(%d)"), newWeapons.size())
    );

    // push all to inventory
    FVector playerLocation = GetActorLocation();
    if (newWeapons.size() > 0)
    {
        for (int i = 0; i < newWeapons.size(); i++){
            Aweapon *current = newWeapons[i]; 
            if(current != nullptr){
                current->SetActorLocation(playerLocation);
                //MUST BE PICKED UP WITH PLAYER CAM TO INTERACT!
                pickUpWeaponIntoInventoryIfNeededAndAttachToBoneController(current);
                //playerInventory.addWeaponIfNotInInventory(current);
            }
        }

        //select first for inventory and bone controller
        playerInventory.selectIndex(0);
        Aweapon *firstWeapon = playerInventory.getItemPointer();
        pickUpWeaponIntoInventoryIfNeededAndAttachToBoneController(firstWeapon);
    }
}


void AplayerScript::pickUpWeaponIntoInventoryIfNeededAndAttachToBoneController(
    Aweapon *weapon
){
    if(weapon != nullptr){
        playerInventory.addWeaponIfNotInInventory(weapon);
        weapon->pickup(CameraComponent);

        boneController.attachOrReplaceCarriedItem(weapon);
        // boneController.dropWeapon(); //drop old weapon(?)
        // boneController.attachCarriedItem(weapon);
    }
}

/**
 * player minimap
 */

void AplayerScript::createMiniMap(){
    if(minimap == nullptr && CameraComponent != nullptr){
        minimap = AMinimap::Construct(GetWorld(), CameraComponent);
    }
}

void AplayerScript::TickMiniMap(){
    if(minimap){
        FVector pos = GetActorLocation();
        FRotator rot = GetActorRotation();
        minimap->updatePlayerPositionAndRotation(pos, rot);
    }
}
