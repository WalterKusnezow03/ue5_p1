#include "WeaponTableActor.h"
#include "p2/weapon/setupHelper/LoadoutHelper.h"

AWeaponTableActor::AWeaponTableActor() : Super() {
    
}

void AWeaponTableActor::BeginPlay(){
    FindSocketOnBeginPlay();
    SpawnWidgetActorOnBeginPlay();
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
    if(weaponSpawned){
        weaponSpawned->SetActorEnableCollision(true);
        weaponSpawned->dropToObjectPool();
    }
    weaponSpawned = LoadoutHelper::SpawnWeaponWithAttachments(
        &setupHelper,
        GetWorld()
    );
    if(weaponSpawned){
        weaponSpawned->SetActorEnableCollision(false);
    }
}



void AWeaponTableActor::UpdateLoadoutWithInternalSetup(LoadoutHelper &ref, int index){
    ref.replace(index, setupHelper);
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