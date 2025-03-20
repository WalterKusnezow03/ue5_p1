// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "p2/gameStart/assetEnums/weaponAttachmentEnum.h"
#include "sightScript.generated.h"

UCLASS()
class P2_API AsightScript : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AsightScript();

	void setType(weaponAttachmentEnum type);
	weaponAttachmentEnum GetSightType();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector getSightCenter();

	weaponAttachmentEnum sightType = weaponAttachmentEnum::iron_sight;
};
