#include "NiagaraComponentCustom.h"


UNiagaraComponentCustom::UNiagaraComponentCustom() : Super()
{
   
}


void UNiagaraComponentCustom::TickExternal(float DeltaTime){
    if(!isInfinite){
        if(timerFinshed){ //not restarted yet.
            Reset();
            return;
        }

        timer.Tick(DeltaTime);
        if(timer.timesUp()){
            // StopEffect();
            timerFinshed = true;
            Reset();
        }
    }
}


bool UNiagaraComponentCustom::IsFinished(){
    return timer.timesUp();
}

void UNiagaraComponentCustom::StartEffect()
{
    Activate(true);
    if(!isInfinite){
        timer.Begin(timeOfEffect);
        timerFinshed = false;
    }
}

/*void UNiagaraComponentCustom::StopEffect()
{
    Deactivate();
}*/

void UNiagaraComponentCustom::StopEffectImmediate()
{
    DeactivateImmediate();
}

void UNiagaraComponentCustom::Reset(){
    StopEffectImmediate();
    ResetSystem();
    StopEffectImmediate();
}


void UNiagaraComponentCustom::ResetAndRestart(){
    Reset();
    StartEffect();
}