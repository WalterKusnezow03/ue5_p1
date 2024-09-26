// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "skelletonController.generated.h"

UCLASS()
class P2_API AskelletonController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AskelletonController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//skelleton
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skelletal Mesh To Control")
	USkeletalMeshComponent *skelletonComponentPointer;
	

	//attach somehow
	void attachLeftArm(AActor *other);
	void attachRightArm(AActor *other);
	void detach(AActor *other);

private:
	void findSkeletonOnStart();
	void attachToSocket(AActor *actorToAttach, FName socketName);
};
