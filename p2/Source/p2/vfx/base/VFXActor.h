#pragma once

#include "CoreMinimal.h"
#include "p2/vfx/type/EVFXActorType.h"
#include "GameCore/Niagara/NiagaraComponentCustom.h"
#include "GameCore/util/ActorBase/ActorBase.h"

#include "VFXActor.generated.h"

UCLASS()
class P2_API AVFXActor : public AActorBase {
    GENERATED_BODY()

public:
    AVFXActor();

    UPROPERTY(EditAnywhere, Category="ActorProperty")
    EVFXActorType typeVFX = EVFXActorType::ENone;

    void Init();
    void PlayAllEffects();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;
    void TickAllComponents(float deltatime);

    void FindAllComponentsIfEmpty();
    TArray<UNiagaraComponentCustom *> components;

    
    void ResetAllEffects();
    void ReleaseToEntityManager();
};