// Fill out your copyright notice in the Description page of Project Settings.


#include "playerScript.h"

#include "p2/entityManager/EntityManager.h"

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
#include "PathFinder/pathFinding/PathFinder.h"
#include "p2/vehicles/vehicle/base/vehicleBase.h"
#include "GameCore/PlayerInfo/PlayerInfo.h"

#include "p2/_world/worldLevel.h"

#include "p2/entities/widgetPayloads/WidgetEntityLootPayload.h"

#include "p2/ui/3Dui/HUD/HudUiActor.h"


// Sets default values
AplayerScript::AplayerScript() : Super()
{
    aiming = false;
}

// Called when the game starts or when spawned
void AplayerScript::BeginPlay()
{
	Super::BeginPlay();
    playerInventory.Setup(CameraComponent);

    setTeam(teamEnum::playerTeam);
    setupBoneController();
    PickupDefaultWeaponOnBeginPlay();
    CreateUiHudActorOnBeginPlay();
    //createMiniMap();
}

void AplayerScript::PickupDefaultWeaponOnBeginPlay(){
    EntityManager *entityMananger = AworldLevel::entityManager();
    Aweapon *weapon = nullptr;
    if(entityMananger != nullptr){
        entityMananger->addActorToIgnoredAllParams(this); //skelleton may not walk on player.

        //w = e->spawnAweapon(GetWorld(), throwableEnum::greneade_enum);
        weapon = entityMananger->spawnAweapon(GetWorld(), weaponEnum::assaultRifle);
        if(weapon != nullptr){
            weapon->applySight(weaponAttachmentEnum::reddot);
            pickUpWeaponIntoInventoryIfNeededAndAttachToBoneController(weapon);
        }

        

        weapon = entityMananger->spawnAweapon(GetWorld(), weaponEnum::pistol);
        if(weapon){
            pickUpWeaponIntoInventoryIfNeededAndAttachToBoneController(weapon);
        }
    }
}










void AplayerScript::CreateUiHudActorOnBeginPlay(){
    FVector relativeLocation(100, 0, 0);
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
    //TickMiniMap();


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
    if(InteractionBlockedPauseAndAnimation()){
        return;
    }
	Super::interact(); //performs Raycast and sets AActor* interactedActorPointer!!

    //show if held pressed
    DebugHelper::showScreenMessage("AplayerScript::Interact E", FColor::Green);

    if(interactedActorPointer){
        Aweapon *weapon = Cast<Aweapon>(interactedActorPointer);
        if(weapon){
            pickUpWeaponIntoInventoryIfNeededAndAttachToBoneController(weapon);
            return;
        }

        AvehicleBase *vehicle = Cast<AvehicleBase>(interactedActorPointer);
        if(vehicle != nullptr){
            vehicle->seatAsDriver(this); //interface passed here
            return;
        }

        


    }
    interactedActorPointer = nullptr; //reset
}



/// @brief drops the weapon from the inventory and bone controller and attaches the new weapon if
/// possible
void AplayerScript::drop(){
    if(InteractionBlockedPauseAndAnimation()){
        return;
    }
    Super::drop();
    playerInventory.dropWeapon();
    boneController.dropCarriedItem();



    Aweapon *otherWeaponNow = playerInventory.getItemPointer();
    if(otherWeaponNow != nullptr){
        //boneController.attachCarriedItem(otherWeaponNow);
        boneController.attachOrReplaceCarriedItem(otherWeaponNow);
    }
}

void AplayerScript::reload(){
    if(InteractionBlockedPauseAndAnimation()){
        return;
    }
    Super::reload();
    playerInventory.reloadWeapon();
}

void AplayerScript::aim(){
    Super::aim();
    if(InteractionBlockedPauseAndAnimation()){
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
    if(InteractionBlockedPauseAndAnimation())
    {
        //DebugHelper::showScreenMessage("GAME IS PAUSED!", FColor::Orange)        
        return;
    }

    if(holding){ //checks if holding mouse down
        
        playerInventory.shoot();
        ApplyRecoil();

    }else{
        //weil das drücken jetzt losgelassen wird
        //können werfbare items weggeworfen werden
        //der humanoid controller muss dazu informiert werden
        //wenn ein throwable item tatsächlich vorliegt.
        if(released){
            released = false;
            if(boneController.IsPerformingThrowItem()){
                return;
            }
            playerInventory.releaseShoot(); // abzug loslassen





            

            //since a throwable is the current item, the new one must be attached and
            //the other thrown away.
            if(playerInventory.CurrentItemIsThrowable()){
                //notify
                Aweapon *spawnedNewItem = playerInventory.GetCurrenThrowablePointer(); //inner reload 
                boneController.NotifyThrowItem(spawnedNewItem);

                //reload newley spawned item
                //boneController.attachOrReplaceCarriedItem(spawnedNewItem);


                DebugHelper::logMessage("AplayerScript::NotifyThrowItem");
                DebugHelper::showScreenMessage("AplayerScript::NotifyThrowItem");
            }
            
        }
        
    }
}

void AplayerScript::ApplyRecoil(){
    float recoil = playerInventory.recoilValue(); 
    DebugHelper::logMessageFloat("WeaponRecoil", recoil); //ok
    addPendingRecoil(recoil * 10.0f);
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
    if(InteractionBlockedPauseAndAnimation()){
        return;
    }
    if(playerInventory.currentIndexNum() != index){
        //find index weapon in inventory, pickup
        playerInventory.selectIndex(index);
        
        //even if null, it must be collected
        //empty arms will be selected in this case.

        Aweapon *targetedWeapon = playerInventory.getItemPointerAtIndex(index);
        boneController.attachOrReplaceCarriedItem(targetedWeapon);
        
        
        
        
        
        /*if(targetedWeapon != nullptr){
            //bone controller pickup
            pickUpWeaponIntoInventoryIfNeededAndAttachToBoneController(
                targetedWeapon
            );
        }*/
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

    boneController.defaultSetup(this);

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

bool AplayerScript::InteractionBlockedPauseAndAnimation(){
    return IsPaused() || boneController.IsPerformingThrowItem();
}

void AplayerScript::openPauseMenu(){
    //Super::openPauseMenu();

    //APauseRoomActor::StaticEnter(this);

    //enters pause room by default
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
    pickUpWeaponIntoInventoryIfNeededAndAttachToBoneController(newWeapons);

    //debug
    /*DebugHelper::logMessage(
        FString::Printf(TEXT("AplayerScript reload loadout weapons(%d)"), newWeapons.size())
    );*/

    // push all to inventory
    if (newWeapons.size() > 0)
    {
        //select first for inventory and bone controller
        playerInventory.selectIndex(0);
        Aweapon *firstWeapon = playerInventory.getItemPointer();
        pickUpWeaponIntoInventoryIfNeededAndAttachToBoneController(firstWeapon);
    }
}

void AplayerScript::pickUpWeaponIntoInventoryIfNeededAndAttachToBoneController(
    std::vector<Aweapon *> &newWeapons
){
    FVector playerLocation = GetActorLocation();
    for (int i = 0; i < newWeapons.size(); i++){
        Aweapon *current = newWeapons[i]; 
        if(current != nullptr){
            current->SetActorLocation(playerLocation);
            //MUST BE PICKED UP WITH PLAYER CAM TO INTERACT!
            pickUpWeaponIntoInventoryIfNeededAndAttachToBoneController(current);
        }
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





void AplayerScript::ReceiveCallback(UWidgetInteractPayload *payload){
    if(payload && IsValid(payload)){
        if(UWidgetEntityLootPayload *entityPayload = Cast<UWidgetEntityLootPayload>(payload)){
            playerInventory.Collect(entityPayload);
        }
    }
}



