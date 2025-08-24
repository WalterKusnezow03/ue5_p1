// Fill out your copyright notice in the Description page of Project Settings.


#include "playerScript.h"
#include "p2/weapon/weapon.h"
#include "playerInventory.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/_world/worldLevel.h"
#include "p2/entityManager/referenceManager.h"
#include "GameCore/DebugHelper.h"
#include "Animation/AnimSequence.h"
#include "GameCore/team/teamEnum.h"
#include "p2/weapon/setupHelper/LoadoutHelper.h"
#include "p2/weapon/setupHelper/weaponSetupHelper.h"
#include <cmath>
#include "GameCore/interfaces/Interactinterface.h"
#include "GameFramework/Character.h" // Falls noch nicht inkludiert
#include "GameFramework/CharacterMovementComponent.h"
#include "AssetPlugin/gamestart/assetEnums/weaponAttachmentEnum.h"
#include "Components/CapsuleComponent.h" // Include for UCapsuleComponent
#include "Camera/CameraComponent.h" // Include for UCameraComponent
#include "p2/player/minimap/Minimap.h"
#include "PathFinder/pathFinding/PathFinder.h"
#include "p2/vehicles/vehicle/base/vehicleBase.h"
#include "GameCore/PlayerInfo/PlayerInfo.h"

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

    //setTeam(referenceManager::TEAM_PLAYER);
    setTeam(teamEnum::playerTeam);

    setupBoneController(); 

    EntityManager *entityMananger = worldLevel::entityManager();

    //TESTING THROWABLE WEAPONS FROM CODE ---> works as expected!
    
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

    createUserInterface();
    createMiniMap();
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


    //world level chunkTick
    worldLevel::Tick(DeltaTime);


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

    //override rotation and location for now
    FVector pos = GetActorLocation();
    FRotator rot = GetActorRotation();
    rot.Pitch = 0.0f;
    //boneController.debugUpdateTransform(pos, rot);

    FVector controllerLocation = boneController.GetLocation();
    boneController.debugUpdateTransform(pos, rot);
    //boneController.overrideRotationYaw(rot.Yaw);


    //update some of the states based on the camera
    if(CameraComponent != nullptr){
        boneController.updateStatesBasedOnCamera(*CameraComponent);
    }

    boneController.Tick(DeltaTime, GetWorld());


    
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
    
    if(isPaused){
        return;
    }
 
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        if (sprinting) {
            boneController.weaponRaisedReadyPosition(); //testing needed
        }else{
            boneController.weaponContactPosition();
        }

    }
}

void AplayerScript::MoveRight(float Value)
{
    Super::MoveRight(Value);
}





void AplayerScript::Jump(){
    Super::Jump();
    if (isPaused)
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
	Super::interact(); //performs Raycast!

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
    Super::drop();
    boneController.dropWeapon();
    playerInventory.dropWeapon();

    Aweapon *otherWeaponNow = playerInventory.getItemPointer();
    if(otherWeaponNow != nullptr){
        boneController.attachCarriedItem(otherWeaponNow);
    }
}

void AplayerScript::reload(){
    Super::reload();
    playerInventory.reloadWeapon();
}

void AplayerScript::aim(){
    Super::aim();
    
    playerInventory.aim(aiming);
    boneController.weaponAimDownSight(aiming);
}

/**
 * shoot the weapon if needed or release. Method handles this automatically
 */
void AplayerScript::shoot(){
    if (isPaused)
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
    if(isPaused){
        return;
    }
    Aweapon *current = playerInventory.getItemPointer();
    if(current != nullptr){
        current->updateSwayEnabledStatus(isWalking);
    }
}








void AplayerScript::setupBoneController(){
	

	//init offset for now

	FVector offset = GetActorLocation();
	boneController.SetLocation(offset);

    boneController.setAsPlayerOwnedController(BASE_SPEED); //sehr wichtig!
    boneController.setupWings(GetWorld());

    // debug testing meshes
	float legScaleCM = boneController.legScale();
	float armScaleCM = boneController.armScale();
	float legHalfScale = legScaleCM / 2.0f;
	float armHalfScale = armScaleCM / 2.0f;

    //set camera to head pos:
    if(CameraComponent){
        float allScale = legScaleCM + armScaleCM;
        float allHalf = allScale / 2.0f;
        allHalf += 5.0f; //add up

        int pushForward = 10;
        cameraReltiveLocationOriginal = FVector(pushForward, 0, allHalf);
        CameraComponent->SetRelativeLocation(cameraReltiveLocationOriginal);
    }
    

    int sizeX = 10;
	int sizeY = 10;
	int offY = sizeY / 2;
	offY = 0;

    //links arm
	AActor *oberarm = createLimbPivotAtTop(sizeX, sizeY, armHalfScale, 0);
	AActor *unterarm = createLimbPivotAtTop(sizeX, sizeY, armHalfScale, 0);
	boneController.attachLimbMeshes(oberarm, unterarm, 3); //hand 1 debug
	
	//holding weapon rechts
	AActor *oberarm_1 = createLimbPivotAtTop(sizeX, sizeY, armHalfScale, 0);
	AActor *unterarm_1 = createLimbPivotAtTop(sizeX, sizeY, armHalfScale, 0);
    boneController.attachLimbMeshes(oberarm_1, unterarm_1, 4); // hand 2 debug


    //fingers 
    if(false){
        int fingerX = 2;
        HandBoneIndexEnum array[] = {
            HandBoneIndexEnum::thumb,
            HandBoneIndexEnum::finger1,
            HandBoneIndexEnum::finger2,
            HandBoneIndexEnum::finger3,
            HandBoneIndexEnum::finger4,
        };
        for (int i = 0; i < 5; i++)
        {
            //right hand
            int fingerY = boneController.fingerScale(array[i]);
            AActor *fingertop = createLimbPivotAtTop(fingerX, fingerX, fingerY, 0);
            AActor *fingerbottom = createLimbPivotAtTop(fingerX, fingerX, fingerY, 0);
            boneController.attachFinger(
                fingertop,
                fingerbottom,
                HandBoneIndexEnum::rightHand,
                array[i]
            );

            //left hand
            AActor *fingertop1 = createLimbPivotAtTop(fingerX, fingerX, fingerY, 0);
            AActor *fingerbottom2 = createLimbPivotAtTop(fingerX, fingerX, fingerY, 0);
            boneController.attachFinger(
                fingertop1,
                fingerbottom2,
                HandBoneIndexEnum::leftHand,
                array[i]
            );
        }
    }
    

    //return;


	AActor *oberschenkel = createLimbPivotAtTop(sizeX, sizeY, legHalfScale, 0);
	AActor *unterschenkel = createLimbPivotAtTop(sizeX, sizeY, legHalfScale, 0);
	boneController.attachLimbMeshes(oberschenkel, unterschenkel, 1); //foot 1 debug
	
	AActor *oberschenkel_1 = createLimbPivotAtTop(sizeX, sizeY, legHalfScale, 0);
	AActor *unterschenkel_1 = createLimbPivotAtTop(sizeX, sizeY, legHalfScale, 0);
	boneController.attachLimbMeshes(oberschenkel_1, unterschenkel_1, 2); //foot 2 debug

    if(false){
        AActor *torsoMesh = createLimbPivotAtTop(sizeX, sizeY * 4, -armScaleCM, 0);
	    boneController.attachTorso(torsoMesh);
    }
    

    //foot
	AActor *foot1 = createLimbPivotAtTop(20, 10, 10, 10);
	AActor *foot2 = createLimbPivotAtTop(20, 10, 10, 10);
	boneController.attachPedalFoots(foot1, foot2);

    return;

    //head
	AActor *headPointer = createLimbPivotAtTop(15, 20, -1 * 25, 0); //-35 flip pivot
	boneController.attachHead(headPointer);


	

	//DEBUG HIDE OWN MESH 
	
}


AActor *AplayerScript::createLimbPivotAtTop(int x, int y, int height, int pushFront){

	height *= -1; //orient downwardss
	

	EntityManager *entitymanagerPointer = worldLevel::entityManager();
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
void AplayerScript::createUserInterface(){
    if(uiInstance == nullptr){
        uiInstance = UPlayerUi::createNewInstance(GetWorld());
        if(uiInstance != nullptr){
            uiInstance->AddToViewport();  // Fügt das UI zur Anzeige hinzu
        }
    }
}

void AplayerScript::openPauseMenu(){
    Super::openPauseMenu();
    if (uiInstance != nullptr)
    {
        if(isPaused){
            uiInstance->openGameScreen();
            isPaused = false;
        }
        else
        {
            uiInstance->openPauseScreen();
            isPaused = true;
        }
    }else{
        isPaused = false;
    }
}

void AplayerScript::updateUi(){
    updateAmmunitionUi();
    updateHealthUi();
}

void AplayerScript::updateAmmunitionUi(){
    if(uiInstance != nullptr){
        //ammunition of weapon
        FString ammunitionText = playerInventory.ammuntionUiStringOfWeapon();
        uiInstance->updateAmmunitionText(ammunitionText);

        //mags left

    }
}


void AplayerScript::updateHealthUi(){
    if(uiInstance != nullptr){
        uiInstance->updateHealthText(health);
    }
}

void AplayerScript::updatePlayerEnteredAreaUi(bool entered){
    if(uiInstance != nullptr){
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
    boneController.dropWeapon(); 
    
    //clear inventory
    playerInventory.dropAllWeaponsToObjectPool();

    //get all new
    std::vector<Aweapon *> newWeapons = loadout.spawnAllWeaponsAndApplyAttachments(GetWorld());
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
        boneController.dropWeapon(); //drop old weapon(?)
        boneController.attachCarriedItem(weapon);
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
