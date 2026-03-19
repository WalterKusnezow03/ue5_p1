#pragma once

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "CoreMinimal.h"
#include "CoreMath/animation/timer/Timer.h"

#include "NiagaraComponentCustom.generated.h"

/// @brief provides simple functionality for a niagara system component
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMECORE_API UNiagaraComponentCustom : public UNiagaraComponent{
    GENERATED_BODY()

public:
    UNiagaraComponentCustom();

    UPROPERTY(EditAnywhere, Category="ComponentParams")
    float timeOfEffect = 2.0f;

    UPROPERTY(EditAnywhere, Category="ComponentParams")
    bool isInfinite = false;

    virtual void TickExternal(float DeltaTime);

    void StartEffect();
    
    void StopEffectImmediate();
    void Reset();

    void ResetAndRestart();

    bool IsFinished();

    void StopEffect();

protected:
    Timer timer;
    bool timerFinshed = true; //true on begin.

   

private:





};