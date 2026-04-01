#include "WeaponTableActor.h"
#include "p2/weapon/setupHelper/LoadoutHelper.h"

AWeaponTableActor::AWeaponTableActor() : Super() {
    
}

void AWeaponTableActor::BeginPlay(){
    FindSocketOnBeginPlay();
    SpawnWidgetActorOnBeginPlay();
    UpdateHasWeaponFlag();
}

void AWeaponTableActor::Tick(float deltatime){

}


void AWeaponTableActor::NotifyWeaponSetupChange(){
    DebugHelper::logMessage("AWeaponTableActor:: Notified change");

    DropAndRespawnWeapon();
    ApplyWeaponLocation();
}

void AWeaponTableActor::DropAndRespawnWeapon(){
    //replace weapon
    DropCurrentWeaponFromTable();
    SpawnWeaponFromSetupHelper();
    UpdateHasWeaponFlag();
}

void AWeaponTableActor::DropCurrentWeaponFromTable(){
    if(weaponSpawned){
        weaponSpawned->SetActorEnableCollision(true);
        weaponSpawned->dropToObjectPool();
    }
    weaponSpawned = nullptr;
}

void AWeaponTableActor::SpawnWeaponFromSetupHelper(){
    weaponSpawned = LoadoutHelper::SpawnWeaponWithAttachments(
        &setupHelper,
        GetWorld());
    if(weaponSpawned){
        weaponSpawned->SetActorEnableCollision(false);
    }
}

void AWeaponTableActor::UpdateHasWeaponFlag(){
    bHasAWeaponForSetup = false;
    if(weaponSpawned){
        bHasAWeaponForSetup = true;
    }
}




void AWeaponTableActor::UpdateWidgetDebugIndex(int index){
    if(tableWidgetActor){
        tableWidgetActor->UpdateDebugNameFromIndex(index);
    }
}

void AWeaponTableActor::UpdateLoadoutWithInternalSetup(LoadoutHelper &ref, int index){
    if(bHasAWeaponForSetup){
        ref.replace(index, setupHelper);
    }
}

void AWeaponTableActor::SpawnWidgetActorOnBeginPlay(){
    tableWidgetActor = AWeaponTableWidgetActor::MakeInstance(
        GetWorld(), 
        widgetSocket, //must be replaced
        localLocationWidget
    );
    if(tableWidgetActor){
        tableWidgetActor->SetParentActor(this);
        tableWidgetActor->SetWeaponSetupHelperRefernce(&setupHelper);
    }
}


void AWeaponTableActor::FindSocketOnBeginPlay(){
    if(!weaponsocket){
        weaponsocket = FindExactChildByName("WeaponSocket");
    }
    if(!widgetSocket){
        widgetSocket = FindExactChildByName("WidgetSocket");
    }
}

void AWeaponTableActor::ApplyWeaponLocation(){
    if(weaponsocket && weaponSpawned){
        weaponSpawned->SetActorLocation(weaponsocket->GetComponentLocation()); //world
    }
}