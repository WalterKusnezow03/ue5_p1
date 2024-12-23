// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "p2/entities/customIk/animation/KeyFrameAnimation.h"
#include "p2/entities/customIk/animation/DoubleKeyFrameAnimation.h"
#include "p2/entities/customIk/bonePackage/BoneController.h"
#include "IkActor.generated.h"

UCLASS()
class P2_API AIkActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIkActor();

	bool debugRaycastDraw = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//own method
	void SetLocation(FVector &location);

private:
	
	float legScaleCM = 200.0f;

	//new bone controllers for arms and legs
	class BoneController hipController;
	class BoneController shoulderController;
	MMatrix hipToShoulderMatrix;

	bool hipIsPivot = true;



	//GLOBAL TRANSFORM
	MMatrix ownLocation;
	MMatrix ownOrientation;

	

	MMatrix currentTransform();


	void LookAt(FVector TargetLocation);	

	bool performRaycast(FVector &Start, FVector &dir, FVector &outputHit);

	void transformFromWorldToLocalCoordinates(FVector &position);


	//could be deprecated
	void projectToGround(FVector &position);
	void projectToGround(FVector &frameToProject, FVector &offsetMade);


	AActor *createLimbPivotAtTop(int x, int y, int height, int offsetY);



};
