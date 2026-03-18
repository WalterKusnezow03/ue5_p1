#include "RpgLauncherWeapon.h"

#include "p2/throwableItems/rpg/RpgThrowable.h"

ARpgLauncherWeapon::ARpgLauncherWeapon() : Super(){
    throwableTypeToThrow = EThrowableEnum::ERpg;

};


void ARpgLauncherWeapon::BeginPlay(){
    Super::BeginPlay();
    
}

void ARpgLauncherWeapon::RequestNewThrowableIfNeeded(){
    if(!launchableActorPointer){
        Super::RequestNewThrowableIfNeeded();
        if(ARpgThrowable *rpg = ThrowableCasted()){
            rpg->AddIgnoredActor(this);
        }
    }
}


ARpgThrowable *ARpgLauncherWeapon::ThrowableCasted(){
    if(launchableActorPointer){
        if(ARpgThrowable *casted = Cast<ARpgThrowable>(launchableActorPointer)){
            return casted;
        }
    }
    return nullptr;
}

