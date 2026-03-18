#include "WidgetRoomActorBase.h"
#include "p2/_world/worldLevel.h"



AWidgetRoomActorBase::AWidgetRoomActorBase() : Super() {
    PrimaryActorTick.bCanEverTick = true; 
    PrimaryActorTick.bTickEvenWhenPaused = true; 
}

void AWidgetRoomActorBase::BeginPlay(){
    Super::BeginPlay();
}

void AWidgetRoomActorBase::EndPlay(const EEndPlayReason::Type EndPlayReason){
    Leave();
    ClearReferencesOnEndPlay();
    Super::EndPlay(EndPlayReason);
}

void AWidgetRoomActorBase::Tick(float deltatime){
    Super::Tick(deltatime);
}

bool AWidgetRoomActorBase::Enter(AActor *actor){
    if(playerEntered){
        return false;
    }
    if(actor){
        
        playerEntered = actor;
        enteredLocation = playerEntered->GetActorLocation();
        return true;
        
    }
    return false;
}

void AWidgetRoomActorBase::Leave(){
    if(playerEntered){
        ResetPlayerLocation();
        playerEntered = nullptr;
        UpdateGameStateOnLeave(EGameState::EGamePlay);
    }
}

void AWidgetRoomActorBase::UpdateGameStateOnLeave(EGameState state){
    AworldLevel::gameStateManager.OverrideGameState(state);
}

void AWidgetRoomActorBase::ClearReferencesOnEndPlay(){
    playerEntered = nullptr;
}



void AWidgetRoomActorBase::ResetPlayerLocation(){
    if(playerEntered){
        enteredLocation += FVector(0, 0, 200);
        //playerEntered->SetActorLocation(enteredLocation);
        playerEntered->SetActorLocation(enteredLocation + FVector(0,0,20),
                                false, 
                                nullptr, 
                                ETeleportType::TeleportPhysics);
        playerEntered = nullptr;
    }
}



void AWidgetRoomActorBase::LockPlayerMovement(bool flag){
    if(APlayerControllerBase *casted = GetPlayerEnteredControllerBase()){
        casted->SetMovementLocked(flag);

        
        FString message = TEXT("AGameStartRoom::LockPlayerMovement ");
        message += flag ? TEXT(" locked ") : TEXT(" unlocked ");
        DebugHelper::logMessage(message);
    }
}

APlayerControllerBase *AWidgetRoomActorBase::GetPlayerEnteredControllerBase(){
    if(APlayerControllerBase *casted = Cast<APlayerControllerBase>(playerEntered)){
        return casted;
    }
    return nullptr;
}