#include "WeaponTableActor.h"
#include "p2/weapon/setupHelper/LoadoutHelper.h"

AWeaponTableActor::AWeaponTableActor() : Super() {
    
}

void AWeaponTableActor::BeginPlay(){
    SpawnWidgetActorOnBeginPlay();
}

void AWeaponTableActor::Tick(float deltatime){

}


void AWeaponTableActor::NotifyWeaponSetupChange(){
    
    //replace weapon
    if(weaponSpawned){
        weaponSpawned->drop();
    }
    weaponSpawned = LoadoutHelper::SpawnWeaponWithAttachments(
        &setupHelper,
        GetWorld()
    );
    
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