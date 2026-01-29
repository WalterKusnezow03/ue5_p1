#include "PauseRoomActor.h"
#include "p2/ui/3Dui/LoadOutRoom/LoadoutRoomActor.h"
#include "p2/ui/3Dui/GameStartRoom/GameStartRoom.h"
#include "p2/ui/3Dui/PauseActor/Widget/PauseWidgetActor.h"
#include "p2/ui/3Dui/PauseActor/Widget/PauseWidget.h"


APauseRoomActor *APauseRoomActor::instance = nullptr;

APauseRoomActor::APauseRoomActor() : Super() {

}

void APauseRoomActor::CreateInstanceIfNeeded(AActor *player){
    if(player){
        if(!instance){
            instance = TMakeInstance<APauseRoomActor>(
                player->GetWorld(),
                EGameActorEnum::EPauseRoom,
                player->GetActorLocation()
            );
            if(instance){
                DebugHelper::logMessage("APauseRoomActor:: success to make instance");
            }else{
                DebugHelper::logMessage("APauseRoomActor:: failed to make instance");
            }
        }
    }
}



void APauseRoomActor::BeginPlay(){
    Super::BeginPlay();
    FindPauseMenuOnBeginPlay();
    SetParentReferenceForPauseWidget();
}

void APauseRoomActor::Tick(float deltatime){
    Super::Tick(deltatime);
}

void APauseRoomActor::Notify(EPauseWidgetEvent event){
    if(event == EPauseWidgetEvent::EOpenLoadoutRoom){
        NotifyOpenLoadoutRoom();
        return;
    }
    if(event == EPauseWidgetEvent::EExitPauseRoom){
        NotifyExit();
        return;
    }
    if(event == EPauseWidgetEvent::EOpenGameStartRoom){
        NotifyOpenGameStartRoom();
        return;
    }
}



void APauseRoomActor::NotifyExit(){
    Leave();
}

void APauseRoomActor::NotifyOpenLoadoutRoom(){
    AActor *copy = playerEntered;
    Leave();
    ALoadoutRoomActor::StaticEnter(copy);
}

void APauseRoomActor::NotifyOpenGameStartRoom(){
    AActor *copy = playerEntered;
    Leave();
    AGameStartRoom::StaticEnter(copy);
}


void APauseRoomActor::StaticEnter(AActor *player){
    CreateInstanceIfNeeded(player);
    if (instance)
    {
        instance->Enter(player);
    }
}

bool APauseRoomActor::Enter(AActor *player){
    if(Super::Enter(player)){
        SetActorHiddenInGame(false);
        LockPlayerMovement(true);
        SetToPlayerLocation();
        return true;
    }
    return false;
}


void APauseRoomActor::StaticLeave(){
    if (instance){
        instance->Leave();
    }
}


void APauseRoomActor::Leave(){
    SetActorHiddenInGame(true);
    LockPlayerMovement(false);
    playerEntered = nullptr;
    UpdateGameStateOnLeave(EGameState::EGamePlay);
    // Super::Leave();
}





void APauseRoomActor::SetToPlayerLocation(){
    if(APlayerControllerBase *base = GetPlayerEnteredControllerBase()){
        FVector look = base->playerLookDir();
        FRotator targetRotation = look.Rotation();

        FVector playerLocation = base->GetActorLocation();
        FVector targetLocation = playerLocation + look * distanceFromPlayerToBeKept;

        SetActorLocation(targetLocation);
        SetActorRotation(targetRotation);
    }
}


void APauseRoomActor::FindPauseMenuOnBeginPlay(){
    if(!pauseWidgetActor){
        AActor *actor = FindChildActorByName("PauseWidgetActorBp");
        if(actor){
            if(APauseWidgetActor *casted = Cast<APauseWidgetActor>(actor)){
                pauseWidgetActor = casted;
            }
        }
    }
}

void APauseRoomActor::SetParentReferenceForPauseWidget(){
    if(UPauseWidget *widget = GetPauseWidget()){
        widget->SetParentActor(this);
    }
}

void APauseRoomActor::ClearReferencesOnEndPlay(){
    if(UPauseWidget *widget = GetPauseWidget()){
        widget->ResetParentActor();
    }
    Super::ClearReferencesOnEndPlay();
}

UPauseWidget *APauseRoomActor::GetPauseWidget(){
    if(pauseWidgetActor){
        if(UPauseWidget *internalPtr = pauseWidgetActor->GetWidget<UPauseWidget>()){
            return internalPtr;
        }
    }
    return nullptr;
}