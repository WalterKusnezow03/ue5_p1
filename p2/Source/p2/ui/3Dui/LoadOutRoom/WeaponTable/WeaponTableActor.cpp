#include "WeaponTableActor.h"
#include "p2/weapon/setupHelper/LoadoutHelper.h"

AWeaponTableActor::AWeaponTableActor() : Super() {
    
}


///// MAKE INSTANCE METHOD NEEDED



void AWeaponTableActor::BeginPlay(){
    SpawnWidgetOnBeginPlay();
    if(tableWidgetActor){
        tableWidgetActor->SetWeaponSetupHelperRefernce(&setupHelper);
    }
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


void AWeaponTableActor::SpawnWidgetOnBeginPlay(){
    tableWidgetActor = AWeaponTableWidgetActor::MakeInstance(
        GetWorld(), 
        RootComponent, //must be replaced
        FVector(-100, 0, 50)
    );
    if(tableWidgetActor){
        tableWidgetActor->SetParentActor(this);
    }
}