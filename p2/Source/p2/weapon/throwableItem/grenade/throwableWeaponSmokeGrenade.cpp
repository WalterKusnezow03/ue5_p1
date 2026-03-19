#include "throwableWeaponSmokeGrenade.h"
#include "GameCore/Niagara/NiagaraComponentCustom.h"


void AthrowableWeaponSmokeGrenade::BeginPlay(){
    Super::BeginPlay();
    FindNiagaraSmokeOnBeginPlay();
}


void AthrowableWeaponSmokeGrenade::FindNiagaraSmokeOnBeginPlay(){
    if(!niagaraSmoke){
        TTryAssignByNameContains<UNiagaraComponentCustom>(
            smokeNiagaraComponentName,
            niagaraSmoke
        );
    }
}


void AthrowableWeaponSmokeGrenade::Detonate(){
    if(!isDetonated){
        Super::Detonate();
        if (niagaraSmoke){
            niagaraSmoke->ResetAndRestart();
        }

        //start max life time timer
        maxLifeTimeTimer.Begin(maxLifeTime);
    }
}

void AthrowableWeaponSmokeGrenade::Tick(float deltatime){
    Super::Tick(deltatime);
    TickNiagaraComponent(deltatime);
    TickLifeTime(deltatime);
}

void AthrowableWeaponSmokeGrenade::TickNiagaraComponent(float deltatime){
    if(isDetonated && niagaraSmoke){
        niagaraSmoke->TickExternal(deltatime);
    }
}

void AthrowableWeaponSmokeGrenade::TickLifeTime(float deltatime){
    if(isDetonated){
        if(niagaraSmoke){
            if(niagaraSmoke->IsFinished()){
                release();
            }
        }

        if(maxLifeTimeTimer.timesUp()){
            release();
        }
        maxLifeTimeTimer.Tick(deltatime);
    }
}





void AthrowableWeaponSmokeGrenade::release(){
    if(niagaraSmoke){
        niagaraSmoke->Reset();
    }
    Super::release();
}