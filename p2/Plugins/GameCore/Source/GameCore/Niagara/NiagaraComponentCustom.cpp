#include "NiagaraComponentCustom.h"


UNiagaraComponentCustom::UNiagaraComponentCustom() : Super()
{
   
}


void UNiagaraComponentCustom::TickExternal(float DeltaTime){
    if(!isInfinite){
        timer.Tick(DeltaTime);
        if(timer.timesUp()){
            StopEffect();
        }
    }
}


bool UNiagaraComponentCustom::IsFinished(){
    return timer.timesUp();
}

void UNiagaraComponentCustom::StartEffect()
{
    if(!isActiveFlag){
        Activate(true);
        
        if(!isInfinite){
            timer.Begin(timeOfEffect);
        }
    }
}

void UNiagaraComponentCustom::StopEffect()
{
    Deactivate();
}

void UNiagaraComponentCustom::StopEffectImmediate()
{
    DeactivateImmediate();
}

void UNiagaraComponentCustom::Reset(){
    DeactivateImmediate();
    ResetSystem();
    StopEffect();
}
