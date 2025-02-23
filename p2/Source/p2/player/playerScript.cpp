// Fill out your copyright notice in the Description page of Project Settings.


#include "playerScript.h"
#include "p2/weapon/weapon.h"
#include "playerInventory.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/_world/worldLevel.h"
#include "p2/entityManager/referenceManager.h"
#include "p2/DebugHelper.h"
#include "Animation/AnimSequence.h"
#include "teamEnum.h"
#include "Components/CapsuleComponent.h" // Include for UCapsuleComponent
#include "Camera/CameraComponent.h" // Include for UCameraComponent


// Sets default values
AplayerScript::AplayerScript()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    

    aiming = false;

    // Set capsule size
    float radius = 55.0f;
    float halfHeight = 85.0f; //100cm leg, 70 arms / torso = 170 -> 170 /2 = 85.0f
    GetCapsuleComponent()->InitCapsuleSize(radius, halfHeight);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    CameraComponent->SetupAttachment(GetCapsuleComponent());
    //CameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 84.f)); // Position the camera
    //CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, halfHeight * 2.0f)); // Position the camera
    CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, halfHeight)); // Position the camera
    CameraComponent->bUsePawnControlRotation = true;

    DebugHelper::logMessage("debugsetup player capsule");

    // Set default values
    TurnRateGamepad = 45.f;
    LookUpRateGamepad = 45.f;

	TurnRateGamepad = 90.f;
    LookUpRateGamepad = 90.f;

	health = 100;


    // Initialize the Skeletal Mesh Component pointer
    SkeletalMeshComponent = GetMesh();

    


    if (GEngine && SkeletalMeshComponent) {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "found skeletal mesh");
    }

    timeleft = 0;

    //FString JumpAnimPath = TEXT("/Game/Animations/JumpAnim.JumpAnim");

    sprinting = false;
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
            weapon->applySight(weaponSightEnum::enum_reddot);
            weapon->pickup(CameraComponent);
            playerInventory.addWeapon(weapon);

            boneController.attachCarriedItem(weapon);
        }

        entityMananger->addActorToIgnoredAllParams(this); //skelleton may not walk on player.
    }
}

// Called to bind functionality to input
void AplayerScript::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	// Bind input actions
    PlayerInputComponent->BindAxis("MoveForward", this, &AplayerScript::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AplayerScript::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &AplayerScript::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUpRate", this, &AplayerScript::LookUpAtRate);
	
	
	

    PlayerInputComponent->BindAction("InteractKey", IE_Pressed, this, &AplayerScript::interact);
	PlayerInputComponent->BindAction("ReloadKey", IE_Pressed, this, &AplayerScript::reload);
	PlayerInputComponent->BindAction("DropKey", IE_Pressed, this, &AplayerScript::drop);
    PlayerInputComponent->BindAction("JumpKey", IE_Pressed, this, &AplayerScript::Jump);

    PlayerInputComponent->BindAction("sprintKey", IE_Pressed, this, &AplayerScript::sprint);
    PlayerInputComponent->BindAction("sprintKey", IE_Released, this, &AplayerScript::sprint);

    // Bind the key down action, here, not needed
	// PlayerInputComponent->BindAction("InteractKey", IE_Pressed, this, &AplayerScript::OnInteractKeyDown);
	// Bind the key up action
    PlayerInputComponent->BindAction("leftMouse", IE_Pressed, this, &AplayerScript::leftMouseDown);
	PlayerInputComponent->BindAction("leftMouse", IE_Released, this, &AplayerScript::leftMouseUp);

    PlayerInputComponent->BindAction("rightMouse", IE_Pressed, this, &AplayerScript::aim);
    PlayerInputComponent->BindAction("rightMouse", IE_Released, this, &AplayerScript::aim);
}



void AplayerScript::switchCamera(){
    return;

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
        CameraComponent->SetRelativeLocation(FVector(0, 0, 2000.0f)); // Position the camera
	    CameraComponent->bUsePawnControlRotation = true;
        CameraComponent->SetRelativeRotation(FRotator(-80, 0, 0)); // Look downward
    }
}



void AplayerScript::debugPathFinder(){
    FVector posA(0, 0, 200);
    FVector own = GetActorLocation();

    DebugHelper::showLineBetween(GetWorld(), posA, own, FColor::Yellow);

    PathFinder *in = PathFinder::instance();
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

    shoot(); //shoot the weapon if needed or release. Method handles automatically

    updateAnimTime(DeltaTime);

    
    TickBoneController(DeltaTime);
    resetFlagsOnTick();
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

    //new testing needed
    SetActorLocation(boneController.stabilizedHipLocation());

    //update some of the states based on the camera
    if(CameraComponent != nullptr){
        boneController.updateStatesBasedOnCamera(*CameraComponent);
    }


    //debug
    isWalking = false;

    //wenn walking, auch locomotion an
    if(isWalking){
        boneController.setStateWalking();
    }else{
        boneController.stopLocomotion();
    }

    boneController.Tick(DeltaTime, GetWorld());


    
    //boneController.debugDrawHeadForward(GetWorld(), DeltaTime);
}

void AplayerScript::resetFlagsOnTick(){
    isWalking = false;
}

/**
 * Implemented take damage method from interface, need to override it, this method is not valid yet
 */
void AplayerScript::takedamage(int d)
{
    //DebugHelper::showScreenMessage("Player Damage", FColor::Yellow);
    // Implementierung der Methode
    health -= d;
	if(health <= 0){
		health = 0;
	}
}

void AplayerScript::takedamage(int d, FVector &from){
    takedamage(d);
}

//Movement
/**
 * moves the player forward
 */
void AplayerScript::MoveForward(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        //testing
        if(sprinting){
            //DebugHelper::showScreenMessage("sprint");
            Value *= SPRINT_MULTIPLY;
        }

        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        if(true){
            AddMovementInput(Direction, Value);
        }
        

        isWalking = true;
    }
}

void AplayerScript::MoveRight(float Value)
{
    if ( (Controller != nullptr) && (Value != 0.0f) )
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void AplayerScript::TurnAtRate(float Rate)
{
    float yawRate = Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds();
    AddControllerYawInput(yawRate);
    //AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AplayerScript::LookUpAtRate(float Rate)
{
	
	AddControllerPitchInput(Rate * LookUpRateGamepad * GetWorld()->GetDeltaSeconds());
	//UE_LOG(LogTemp, Warning, TEXT("Turning at rate: %f"), Rate);
}

void AplayerScript::Jump(){
    if (CanJump())
    {
        ACharacter::Jump(); // Calls the base class jump function



        if (EntityManager *e = worldLevel::entityManager()){
            FVector location = GetActorLocation();
            //e->createALayout(GetWorld(), location, 20, 20);

            //int meters = 50;
            //e->createTerrain(GetWorld(), meters);
        }

        //new testing
        


    }

    //debug
    switchCamera();

    //TESTING of layout creator, works as expected
    
    
}

void AplayerScript::sprint(){
    sprinting = !sprinting;
}

/**
 * allows the player to interact
 */
void AplayerScript::interact(){
	performRaycast();

}

void AplayerScript::performRaycast()
{
	// Get the camera location and rotation
    FVector CameraLocation = CameraComponent->GetComponentLocation();
    FRotator CameraRotation = CameraComponent->GetComponentRotation();

    // Define the start and end vectors for the raycast
    FVector Start = CameraLocation;

    //100 = 1m
    //500m = 
    FVector End = Start + (CameraRotation.Vector() * 100.0f); //1 meter in front of the camera

    // Perform the raycast
    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this); // Ignore the character itself

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);



    // If the raycast hit something, log the hit actor's name
    if (bHit)
    {
		AActor* actor = HitResult.GetActor();
		if(actor != nullptr){
			

			Aweapon *weapon = Cast<Aweapon>(actor);
			if(weapon){
				weapon->pickup(CameraComponent);
                playerInventory.addWeapon(weapon);

                boneController.attachCarriedItem(weapon);
            }
        }

		
    }
}


/// @brief drops the weapon from the inventory and bone controller and attaches the new weapon if
/// possible
void AplayerScript::drop(){
    boneController.dropWeapon(); 
    playerInventory.dropWeapon();

    Aweapon *otherWeaponNow = playerInventory.getItemPointer();
    if(otherWeaponNow != nullptr){
        boneController.attachCarriedItem(otherWeaponNow);
    }
}

void AplayerScript::reload(){
    playerInventory.reloadWeapon();
}



void AplayerScript::aim(){
    aiming = !aiming;
    playerInventory.aim(aiming);
    boneController.weaponAimDownSight(aiming);
}

/**
 * shoot the weapon if needed or release. Method handles this automatically
 */
void AplayerScript::shoot(){
    
    if(holding){ //checks if holding mouse down
        

        //recoil apply (immidiate, will be based on canshoot of weapon)
        float recoil = playerInventory.recoilValue();
        LookUpAtRate(recoil);

        playerInventory.shoot();

    }else{
        playerInventory.releaseShoot(); //abzug loslassen
    }
}

/// @brief sets the left mouse holding status for the weapon
/// @param h 
void AplayerScript::setHolding(bool h){
    holding = h;
}


void AplayerScript::leftMouseDown(){
    setHolding(true);

    //debug
    //debugPathFinder();
}

void AplayerScript::leftMouseUp(){
    setHolding(false);
}







//testing needed

void AplayerScript::PlayAnimation(UAnimSequence* AnimSequence)
{
    if (AnimSequence && SkeletalMeshComponent)
    {   
        float animationlength = AnimSequence->GetPlayLength();
        resetAnimtime(animationlength);

        SkeletalMeshComponent->PlayAnimation(AnimSequence, false); // false means don't loop
    }else{

        DebugHelper::showScreenMessage("player animation not found");
    }
}



/// @brief sets the time left to a new time
/// @param newTime 
void AplayerScript::resetAnimtime(float newTime){
    timeleft = newTime;
}

/// @brief update the animation time delta
/// @param delta 
void AplayerScript::updateAnimTime(float delta){
    if(timeleft > 0){
        timeleft -= delta;    
    }
}
/// @brief returns if an animation is still playing
/// @return 
bool AplayerScript::animationisPlaying(){
    return timeleft > 0;
}





void AplayerScript::setTeam(teamEnum teamIn){
    this->team = teamIn;
    // referenceManager::verifyTeam(teamIn);
}

teamEnum AplayerScript::getTeam(){
    return team;
}











void AplayerScript::setupBoneController(){
	

	//init offset for now

	FVector offset = GetActorLocation();
	boneController.SetLocation(offset);

	// debug testing meshes
	float legScaleCM = 100.0f;
	float armScaleCM = 70.0f;
	float legHalfScale = legScaleCM / 2.0f;
	float armHalfScale = armScaleCM / 2.0f;

    //set camera to head pos:
    if(CameraComponent){
        float allScale = legScaleCM + armScaleCM;
        float allHalf = allScale / 2.0f;
        allHalf += 5.0f; //add up
        cameraReltiveLocationOriginal = FVector(0, 0, allHalf);
        CameraComponent->SetRelativeLocation(cameraReltiveLocationOriginal);
    }
    

    int sizeX = 10;
	int sizeY = 10;
	int offY = sizeY / 2;
	offY = 0;

	
	AActor *oberarm = createLimbPivotAtTop(sizeX, sizeY, armHalfScale, 0);
	AActor *unterarm = createLimbPivotAtTop(sizeX, sizeY, armHalfScale, 0);
	boneController.attachLimbMeshes(oberarm, unterarm, 3); //hand 1 debug
	
	//holding weapon
	AActor *oberarm_1 = createLimbPivotAtTop(sizeX, sizeY, armHalfScale, 0);
	AActor *unterarm_1 = createLimbPivotAtTop(sizeX, sizeY, armHalfScale, 0);
    unterarm_1 = nullptr; //DEBUG
    boneController.attachLimbMeshes(oberarm_1, unterarm_1, 4); // hand 2 debug


    //fingers right
    int fingerX = 2;
    int fingerY = 4;
    HandBoneIndexEnum array[] = {
        HandBoneIndexEnum::thumb,
        HandBoneIndexEnum::finger1,
        HandBoneIndexEnum::finger2,
        HandBoneIndexEnum::finger3,
        HandBoneIndexEnum::finger4,
    };
    for (int i = 0; i < 5; i++)
    {
        AActor *fingertop = createLimbPivotAtTop(fingerX, fingerX, fingerY, 0);
        AActor *fingerbottom = createLimbPivotAtTop(fingerX, fingerX, fingerY, 0);
        boneController.attachFinger(
            fingertop,
            fingerbottom,
            HandBoneIndexEnum::rightHand,
            array[i]
        );
    }

    return;


	AActor *oberschenkel = createLimbPivotAtTop(sizeX, sizeY, legHalfScale, 0);
	AActor *unterschenkel = createLimbPivotAtTop(sizeX, sizeY, legHalfScale, 0);
	boneController.attachLimbMeshes(oberschenkel, unterschenkel, 1); //foot 1 debug
	
	AActor *oberschenkel_1 = createLimbPivotAtTop(sizeX, sizeY, legHalfScale, 0);
	AActor *unterschenkel_1 = createLimbPivotAtTop(sizeX, sizeY, legHalfScale, 0);
	boneController.attachLimbMeshes(oberschenkel_1, unterschenkel_1, 2); //foot 2 debug


    AActor *torsoMesh = createLimbPivotAtTop(sizeX, sizeY * 4, -armScaleCM, 0);
	boneController.attachTorso(torsoMesh);

    //foot
	AActor *foot1 = createLimbPivotAtTop(20, 10, 10, 10);
	AActor *foot2 = createLimbPivotAtTop(20, 10, 10, 10);
	boneController.attachPedalFoots(foot1, foot2);


	//head
	AActor *headPointer = createLimbPivotAtTop(15, 20, -1 * 25, 0); //-35 flip pivot
	boneController.attachHead(headPointer);


	

	//DEBUG HIDE OWN MESH 
	
}


AActor *AplayerScript::createLimbPivotAtTop(int x, int y, int height, int pushFront){

	height *= -1; //orient downwardss
	/**
	 * DEBUG CREATE FOLLOW LIMBS
	 */
	UMaterial *material = nullptr;
	assetManager *assetManagerPointer = assetManager::instance();
	if(assetManagerPointer != nullptr){
		material = assetManagerPointer->findMaterial(materialEnum::wallMaterial);
	}

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
				material
			);

			entitymanagerPointer->addActorToIgnoreRaycastParams(
				this, getTeam()
			);

			oberschenkel->setDamagedOwner(this);



			return oberschenkel;
		}
	}
	return nullptr;
}