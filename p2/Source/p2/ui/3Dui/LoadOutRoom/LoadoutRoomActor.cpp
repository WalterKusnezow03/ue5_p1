#include "LoadoutRoomActor.h"
#include "AssetEnumCollection/assetEnums/EGameActorEnum.h"
#include "p2/player/playerScript.h"


ALoadoutRoomActor *ALoadoutRoomActor::MakeInstance(UWorld *world){
    

    UClass *SpawnClass = nullptr;
    if(assetManager *a = assetManager::instance()){
        SpawnClass = a->Find<EGameActorEnum, UClass>(EGameActorEnum::ELoadoutRoom);
    }
    if(!SpawnClass){
        return nullptr;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    FVector Location(0, 0, 0);
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


    return spawned;
}





ALoadoutRoomActor::ALoadoutRoomActor() : Super(){

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
            
            //update all loadout slots
            for (int i = 0; i < weaponTables.Num(); i++){
                if(AWeaponTableActor *current = weaponTables[i]){
                    //get setup
                    current->UpdateLoadoutWithInternalSetup(loadout, i);
                }
            }

            casted->reloadLoadout(loadout);
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
