
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UiActor.generated.h"

class UPlayerUiBase;

UCLASS()
class CUSTOMUIPLUGIN_API AUiActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUiActor();

    static AUiActor *createInstance(UWorld *world, UPlayerUiBase *instanceToTick);

	static UWorld *GetWorldFromInstance();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Override end play to reset the instance ptr
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    UPlayerUiBase *uiToUpdate = nullptr;

	static AUiActor *currentInstance;
};
