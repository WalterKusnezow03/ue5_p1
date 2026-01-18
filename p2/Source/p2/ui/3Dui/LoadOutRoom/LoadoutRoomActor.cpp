#include "LoadoutRoomActor.h"
#include "AssetEnumCollection/assetEnums/EGameActorEnum.h"
#include "p2/player/playerScript.h"

ALoadoutRoomActor *ALoadoutRoomActor::instance = nullptr;



void ALoadoutRoomActor::CreateInstanceIfNeeded(AActor *actor){
    if(actor){
        CreateInstanceIfNeeded(actor->GetWorld());
    }
}

void ALoadoutRoomActor::CreateInstanceIfNeeded(UWorld *world){
    if(world && !instance){
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
    FVector Location(0, 0, -1000);
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

    return spawned;
}


void ALoadoutRoomActor::EndPlay(const EEndPlayReason::Type EndPlayReason){
    instance = nullptr;
    Leave();
    weaponTables.Empty();
    Super::EndPlay(EndPlayReason);
}

ALoadoutRoomActor::ALoadoutRoomActor() : Super(){
    //constructor
}

void ALoadoutRoomActor::BeginPlay(){
    Super::BeginPlay();
    FindActorsOnBeginPlay();
}

void ALoadoutRoomActor::FindActorsOnBeginPlay(){
    weaponTables.Empty();
    TArray<UChildActorComponent *> Childs;
    TFindAllChildsByType<UChildActorComponent>(Childs);
    for (int i = 0; i < Childs.Num(); i++){
        if(UChildActorComponent *current = Childs[i]){
            if(AActor *parent = current->GetChildActor()){
                if(AWeaponTableActor *castedcurrent = Cast<AWeaponTableActor>(parent)){
                    weaponTables.Add(castedcurrent);
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
    }
}

void ALoadoutRoomActor::Leave(){
    if(playerEntered){
        UpdatePlayerInventory();
        ResetPlayerLocation();
        TriggerLeaveAnimation();
    }
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
   if(playerEntered && weaponTables.Num() > 0){
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
        playerEntered->SetActorLocation(enteredLocation);
        playerEntered = nullptr;
    }
}




void ALoadoutRoomActor::TriggerEnteredAnimation(){

}
void ALoadoutRoomActor::TriggerLeaveAnimation(){

}
