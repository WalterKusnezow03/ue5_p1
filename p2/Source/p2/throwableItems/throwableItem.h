// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "throwableEnum.h"
#include "throwableItem.generated.h"

UCLASS()
class P2_API AthrowableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AthrowableItem();

	void setLocation(FVector location);
	void setRotation(FRotator rotation);
	void throwIntoDirection(FVector start, FVector direction);

	void enablePhysics(bool enable);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "throwabletype")
	throwableEnum throwableType;

	throwableEnum getType();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool isThrown;
	
};
