#include "HudUiActor.h"

AHudUiActor *AHudUiActor::MakeInstance(
    UWorld *world, 
    USceneComponent *attachTo,
    FVector relativeLocation
){
    if(!world || !attachTo){
        return nullptr;
    }

    /*
    UClass* SpawnClass = AUINumberWidgetActor::StaticClass();
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    FVector Location(0, 0, 0);
    AUINumberWidgetActor *spawned = world->SpawnActor<AUINumberWidgetActor>(
        SpawnClass,
        Location,
        FRotator::ZeroRotator,
        SpawnParams
    );

    if(ASimulationActor *actor = ASimulationActor::GetInstance()){
        // Set pause menu to hidden by default
        spawned->SetWidgetClass(actor->NumberWidgetClass);
    }else{
        return nullptr;
    }

    if (!spawned)
    {
        UE_LOG(LogTemp, Error, TEXT("AUINumberWidgetActor::MakeINstance - Spawned actor is not a AUINumberWidgetActor"));
        return nullptr;
    }

    spawned->SetVisible(true);

    //attach
    spawned->AttachToComponent(attachTo->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    spawned->SetActorRelativeLocation(FVector(0, 0, 50));

    //debug
    spawned->SetNumber(999);

    return spawned;


    */
    return nullptr;
}

void AHudUiActor::AttachToScene(USceneComponent *camera){

}


// update

void AHudUiActor::updateAmmunitionText(FString message){

}

void AHudUiActor::updateHealthText(FString health){

}


void AHudUiActor::updateTopWaringElement(FString message){

}

void AHudUiActor::updateTopWarningElementTimed(FString message, float timetoLive){

}

void AHudUiActor::Update(FPlayerStatus &playerStatusStruct){

}


void AHudUiActor::BeginPlay(){
    Super::BeginPlay();

}

void AHudUiActor::Tick(float deltatime){
    Super::Tick(deltatime);

}