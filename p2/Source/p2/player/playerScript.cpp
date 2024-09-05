// Fill out your copyright notice in the Description page of Project Settings.


#include "playerScript.h"
#include "p2/weapon/weapon.h"
#include "playerInventory.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/entityManager/referenceManager.h"
#include "p2/DebugHelper.h"
#include "Animation/AnimSequence.h"
#include "Components/CapsuleComponent.h" // Include for UCapsuleComponent
#include "Camera/CameraComponent.h" // Include for UCameraComponent


// Sets default values
AplayerScript::AplayerScript()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    

    aiming = false;

    // Set capsule size
    GetCapsuleComponent()->InitCapsuleSize(55.f, 90.0f);

    // Create a CameraComponent	
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    CameraComponent->SetupAttachment(GetCapsuleComponent());
    CameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;
	

	// Set default values
    TurnRateGamepad = 45.f;
    LookUpRateGamepad = 45.f;

	TurnRateGamepad = 90.f;
    LookUpRateGamepad = 90.f;

	health = 100;


    // Initialize the Skeletal Mesh Component pointer
    SkeletalMeshComponent = GetMesh();

    //animations
    idle = TEXT("/Game/Imported/Humanoid/rifle_aiming_idle.rifle_aiming_idle");
    walking = TEXT("/Game/Imported/Humanoid/walking.walking");


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
    
    //testing
    EntityManager *e = EntityManager::instance();
    if(e != nullptr){
        FVector spawnLocation = GetActorLocation();
        spawnLocation.Y += 300;
        spawnLocation.X += 300;
        e->spawnHumanEntity(GetWorld(), spawnLocation);
    }


    setTeam(referenceManager::TEAM_PLAYER);




    //TESTING THROWABLE WEAPONS FROM CODE ---> works as expected!
    
    Aweapon *w = nullptr;
    if(e != nullptr){
        //w = e->spawnAweapon(GetWorld(), throwableEnum::greneade_enum);
        w = e->spawnAweapon(GetWorld(), weaponEnum::pistol);
        if(w != nullptr){
            w->pickup(CameraComponent);
            playerInventory.addWeapon(w);
        }
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




// Called every frame
void AplayerScript::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    shoot(); //shoot the weapon if needed or release. Method handles automatically

    updateAnimTime(DeltaTime);
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
        AddMovementInput(Direction, Value);

        //if(!animationisPlaying())
        //    PlayAnimation(LoadObject<UAnimSequence>(nullptr, *walking));
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
    AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
	//UE_LOG(LogTemp, Warning, TEXT("Turning at rate: %f"), Rate);
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



        if(EntityManager *e = EntityManager::instance()){
            FVector location = GetActorLocation();
            //e->createALayout(GetWorld(), location, 20, 20);

            //int meters = 50;
            //e->createTerrain(GetWorld(), meters);
        }

        //new testing
        


    }

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
            }
		}

		
    }
}

void AplayerScript::drop(){
    playerInventory.dropWeapon();
}

void AplayerScript::reload(){
    playerInventory.reloadWeapon();
}



void AplayerScript::aim(){
    aiming = !aiming;
    playerInventory.aim(aiming);
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





void AplayerScript::setTeam(int teamIn){
    this->team = referenceManager::verifyTeam(teamIn);
}

int AplayerScript::getTeam(){
    return team;
}