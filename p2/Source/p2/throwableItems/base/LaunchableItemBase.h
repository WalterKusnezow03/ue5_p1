// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "p2/weapon/throwerWeapon/enum/throwableEnum.h"
#include "GameCore/util/ActorBase/ActorBase.h"
#include "LaunchableItemBase.generated.h"

UCLASS()
class P2_API ALaunchableItemBase : public AActorBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaunchableItemBase();

	
	virtual void throwIntoDirection(FVector start, FVector direction);

	//show or hide this actor
	void show(bool show);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "throwableProperty")
	EThrowableEnum throwableType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "throwableProperty")
	float velocityThrow = 200.0f;

	EThrowableEnum getType();
	virtual void reset();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // Called every frame
	virtual void Tick(float DeltaTime) override;
    virtual void release();

	bool isThrown;
	FVector latestThrowingDirection;

	void FindMeshOnBeginPlay();
	UPROPERTY()
	UStaticMeshComponent *mesh = nullptr;

	

	


};
