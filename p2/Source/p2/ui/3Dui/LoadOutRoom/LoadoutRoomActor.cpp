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
        //instance = MakeInstance(world); //not needed, is set in function
        MakeInstance(world);
    } 
}


ALoadoutRoomActor *ALoadoutRoomActor::MakeInstance(UWorld *world){
    
    if(instance){
        return instance;
    }

    UClass *SpawnClass = nullptr;
    if(assetManager *a = assetManager::instance()){
        SpawnClass = a->Find<EGameActorEnum, UClass>(EGameActorEnum::ELoadoutRoom);
    }
    if(!SpawnClass){
        return nullptr;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    FVector Location(-10000, -10000, -1000);
    ALoadoutRoomActor *spawned = world->SpawnActor<ALoadoutRoomActor>(
        SpawnClass,
        Location,
        FRotator::ZeroRotator,
        SpawnParams
    );

    if (!spawned)
    {
        DebugHelper::logMessage("AHudUiActor::MakeInstance - Spawn actor failed");
        return nullptr;
    }

    //save instance ptr, only one needed.
    instance = spawned;
    DebugHelper::logMessage("ALoadoutRoomActor made instance");
    return spawned;
}


void ALoadoutRoomActor::EndPlay(const EEndPlayReason::Type EndPlayReason){
    
    Leave();
    ClearReferences();
    instance = nullptr;
    Super::EndPlay(EndPlayReason);
}

void ALoadoutRoomActor::ClearReferences(){
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


void ALoadoutRoomActor::Enter(AActor *player){
    if(playerEntered){
        return;
    }
    if(player){
        enteredLocation = player->GetActorLocation();
        playerEntered = player;
        playerEntered->SetActorLocation(GetActorLocation());
        TriggerEnteredAnimation();


        logMessage("ALoadoutRoomActor::Entered Locaiton", enteredLocation);

        FVector debug = GetActorLocation();
        logMessage("ALoadoutRoomActor::Room Locaiton", debug);
    }
}

void ALoadoutRoomActor::Leave(){
    if(playerEntered){
        UpdatePlayerInventory();
        ResetPlayerLocation();
        TriggerLeaveAnimation();
    }
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
    /*
    
    for (int i = 0; i < weaponTables.Num(); i++){
        if(AWeaponTableActor *current = weaponTables[i]){
            current-> //get setup

            //paste to loadout
        }
    }
    */
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






void ALoadoutRoomActor::ResetPlayerLocation(){
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



void ALoadoutRoomActor::TriggerEnteredAnimation(){

}

void ALoadoutRoomActor::TriggerLeaveAnimation(){

}
