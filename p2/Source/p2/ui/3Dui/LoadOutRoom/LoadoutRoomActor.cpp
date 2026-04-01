#include "LoadoutRoomActor.h"
#include "AssetEnumCollection/assetEnums/EGameActorEnum.h"
#include "p2/player/playerScript.h"
#include "p2/ui/3Dui/LoadOutRoom/ExitActor/LoadoutRoomExitButtonActor.h"

ALoadoutRoomActor *ALoadoutRoomActor::instance = nullptr;



void ALoadoutRoomActor::CreateInstanceIfNeeded(AActor *actor){
    if(instance){
        DebugHelper::logMessage("ALoadoutRoomActor Already has instance");
        return;
    }
    if(actor){
        CreateInstanceIfNeeded(actor->GetWorld());
    }
}

void ALoadoutRoomActor::CreateInstanceIfNeeded(UWorld *world){
    if(instance){
        return;
    }
    
    if(world){
        if(!instance){
            instance = TMakeInstance<ALoadoutRoomActor>(
                world,
                EGameActorEnum::ELoadoutRoom,
                FVector(-10000, -10000, -1000)
            );
        }
    }
}


void ALoadoutRoomActor::EndPlay(const EEndPlayReason::Type EndPlayReason){
    
    //Leave();
    //ClearReferences();
    instance = nullptr;
    Super::EndPlay(EndPlayReason);
}

void ALoadoutRoomActor::ClearReferencesOnEndPlay(){
    if(exitButton){
        exitButton->ClearParentActor();
    }
    weaponTables.Empty();
}



ALoadoutRoomActor::ALoadoutRoomActor() : Super(){
    //constructor
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it. 
    PrimaryActorTick.bCanEverTick = true; 
    PrimaryActorTick.bTickEvenWhenPaused = true; 
 
}

void ALoadoutRoomActor::BeginPlay(){
    Super::BeginPlay();
    FindActorsOnBeginPlay();
}

void ALoadoutRoomActor::FindActorsOnBeginPlay(){
    weaponTables.Empty();
    TArray<AActor *> actors = FindAllChildActorsAsAActor();
    ProcessFoundActors(actors);
}

void ALoadoutRoomActor::ProcessFoundActors(TArray<AActor*> &actors){
    for (int i = 0; i < actors.Num(); i++){
        if(AActor *current = actors[i]){
            if(AWeaponTableActor *castedcurrent = Cast<AWeaponTableActor>(current)){
                if(!weaponTables.Contains(castedcurrent)){
                    weaponTables.Add(castedcurrent);
                    castedcurrent->UpdateWidgetDebugIndex(weaponTables.Num() - 1);
                }
            }

            //setup exit button, reference not needed
            if(!exitButton){
                if(ALoadoutRoomExitButtonActor *exitCasted = Cast<ALoadoutRoomExitButtonActor>(current)){
                    exitCasted->SetParentActor(this);
                    exitButton = exitCasted;
                }
            }
        }
    }
}

void ALoadoutRoomActor::Tick(float deltatime){
    Super::Tick(deltatime);

}

void ALoadoutRoomActor::StaticEnter(AActor *actor){
    if(actor){
        CreateInstanceIfNeeded(actor);
        if(instance){
            instance->Enter(actor);
        }
    }
}

void ALoadoutRoomActor::StaticLeave(){
    if(instance){
        instance->Leave();
    }
}


bool ALoadoutRoomActor::Enter(AActor *player){


    if(playerEntered){
        return false;
    }
    if(Super::Enter(player)){
        playerEntered->SetActorLocation(GetActorLocation());
        TriggerEnteredAnimation();
        return true;
    }
    return false;

    /*
    if(player){
        enteredLocation = player->GetActorLocation();
        playerEntered = player;
        playerEntered->SetActorLocation(GetActorLocation());
        TriggerEnteredAnimation();


        logMessage("ALoadoutRoomActor::Entered Locaiton", enteredLocation);

        FVector debug = GetActorLocation();
        logMessage("ALoadoutRoomActor::Room Locaiton", debug);
    }*/
}





void ALoadoutRoomActor::Leave(){
    if(playerEntered){
        UpdatePlayerInventory();
        ResetPlayerLocation();
        TriggerLeaveAnimation();
    }
    Super::Leave();
}

void ALoadoutRoomActor::showScreenMessage(FString message, FColor color){
    message += "-";
    message += GetActorLabel();
    DebugHelper::showScreenMessage(message, color);
}

void ALoadoutRoomActor::logMessage(FString message){
    message += "-";
    message += GetActorLabel();
    DebugHelper::logMessage(message);
}

void ALoadoutRoomActor::logMessage(FString message, FVector pos){
    message += FString::Printf(TEXT("(%.2f, %.2f, %.2f)"), pos.X, pos.Y, pos.Z);
    message += "-";
    message += GetActorLabel();
    DebugHelper::logMessage(message);
}






void ALoadoutRoomActor::UpdatePlayerInventory(){
   if(playerEntered){
        if(AplayerScript *casted = Cast<AplayerScript>(playerEntered)){
            UpdateLoadoutWithTableActors();
            casted->reloadLoadout(loadout);
        }
   }
}

void ALoadoutRoomActor::UpdateLoadoutWithTableActors(){
    //update all loadout slots
    for (int i = 0; i < weaponTables.Num(); i++){
        if(AWeaponTableActor *current = weaponTables[i]){
            //get setup
            current->UpdateLoadoutWithInternalSetup(loadout, i);
        }
    }
}







void ALoadoutRoomActor::TriggerEnteredAnimation(){

}

void ALoadoutRoomActor::TriggerLeaveAnimation(){

}
