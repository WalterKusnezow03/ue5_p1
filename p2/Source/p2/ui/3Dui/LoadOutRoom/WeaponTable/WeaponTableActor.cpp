#include "WeaponTableActor.h"
#include "p2/weapon/setupHelper/LoadoutHelper.h"

AWeaponTableActor::AWeaponTableActor() : Super() {
    
}

void AWeaponTableActor::BeginPlay(){
    SpawnWidgetActorOnBeginPlay();
    FindSocketOnBeginPlay();
}

void AWeaponTableActor::Tick(float deltatime){

}


void AWeaponTableActor::NotifyWeaponSetupChange(){
    DebugHelper::logMessage("AWeaponTableActor:: Notified change");
    
    //replace weapon
    if(weaponSpawned){
        weaponSpawned->dropToObjectPool();
    }
    weaponSpawned = LoadoutHelper::SpawnWeaponWithAttachments(
        &setupHelper,
        GetWorld()
    );
    ApplyWeaponLocation();
}

void AWeaponTableActor::UpdateLoadoutWithInternalSetup(LoadoutHelper &ref, int index){
    ref.replace(index, setupHelper);
}


void AWeaponTableActor::SpawnWidgetActorOnBeginPlay(){
    tableWidgetActor = AWeaponTableWidgetActor::MakeInstance(
        GetWorld(), 
        RootComponent, //must be replaced
        FVector(-100, 0, 50)
    );
    if(tableWidgetActor){
        tableWidgetActor->SetParentActor(this);
        tableWidgetActor->SetWeaponSetupHelperRefernce(&setupHelper);
    }
}


void AWeaponTableActor::FindSocketOnBeginPlay(){
    if(!socket){
        socket = FindExactChildByName("WeaponSocket");
    }
}

void AWeaponTableActor::ApplyWeaponLocation(){
    if(socket && weaponSpawned){
        weaponSpawned->SetActorLocation(socket->GetComponentLocation()); //world
    }
}