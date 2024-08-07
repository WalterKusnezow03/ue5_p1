// Fill out your copyright notice in the Description page of Project Settings.


#include "playerScript.h"
#include "weapon.h"
#include "Components/CapsuleComponent.h" // Include for UCapsuleComponent
#include "Camera/CameraComponent.h" // Include for UCameraComponent

// Sets default values
AplayerScript::AplayerScript()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    weaponpointer = nullptr;

    aiming = false;

    // Set capsule size
    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

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


    //FString JumpAnimPath = TEXT("/Game/Animations/JumpAnim.JumpAnim");

}

// Called when the game starts or when spawned
void AplayerScript::BeginPlay()
{
	Super::BeginPlay();
	
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
}

/**
 * Implemented take damage method from interface
 */
void AplayerScript::takedamage(int d)
{
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
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
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
    FVector End = Start + (CameraRotation.Vector() * 100000.0f); //50000 units in front of the camera

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
				weapon->pickup(*CameraComponent);
				weaponpointer = weapon;
			}
		}

		
    }
}

void AplayerScript::drop(){
	if(weaponpointer != nullptr){
        weaponpointer->dropweapon();
    }
}

void AplayerScript::reload(){
	if(weaponpointer != nullptr){
        weaponpointer->reload(30);
    }
}



void AplayerScript::aim(){
    aiming = !aiming;
    if(weaponpointer != nullptr){
        weaponpointer->aim(aiming);
    }
}

/**
 * shoot the weapon if needed or release. Method handles this automatically
 */
void AplayerScript::shoot(){
    if(weaponpointer != nullptr){
        if(holding){
            weaponpointer->shoot();
        }else{
            weaponpointer->releaseShoot(); //abzug loslassen
        }
    }
}


void AplayerScript::setHolding(bool h){
    holding = h;
}


void AplayerScript::leftMouseDown(){
    setHolding(true);
}

void AplayerScript::leftMouseUp(){
    setHolding(false);
}


