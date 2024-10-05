// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "p2/interfaces/Damageinterface.h"
#include "jumpableActor.h"
#include "p2/gamestart/assetEnums/materialEnum.h"
#include "destructable.generated.h"

UCLASS()
class P2_API Adestructable : public AjumpableActor, public IDamageinterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Adestructable();

	///saves the material type for debree and mesh creation
	UPROPERTY(EditAnywhere)
	materialEnum materialType;

	UPROPERTY(EditAnywhere)
	bool destructable;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void takedamage(int d) override;
	virtual void takedamage(int d, FVector &from) override;
	virtual void setTeam(teamEnum t) override;
	virtual teamEnum getTeam() override;

private:
	
};
