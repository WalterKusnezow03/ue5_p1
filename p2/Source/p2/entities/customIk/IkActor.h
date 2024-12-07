// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "p2/entities/customIk/animation/KeyFrameAnimation.h"
#include "p2/entities/customIk/animation/DoubleKeyFrameAnimation.h"
#include "p2/entities/customIk/BoneIk.h"
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
	float legScaleCM = 200;
	float legScaleMeters = 2.0f;

	//depreacted sort of
	float velocity = 1.0f;
	float halfVelocity = 0.0f; //for offset time later, aber eins nach dem anderen.

	/// @brief degree per second constant for etha moving
	float degreePerSecond = 40;

	//legs for now
	class BoneIk leg1;
	class BoneIk leg2;
	class BoneIk arm1;

	//GLOBAL TRANSFORM
	MMatrix ownLocation;
	MMatrix ownOrientation;

	MMatrix ownLocationFoot;
	MMatrix ownLocationFootRight;
	MMatrix chestRelative;
	

	MMatrix currentTransform();
	MMatrix currentFootTransform();
	MMatrix currentFootTransform(MMatrix &foottranslationToRotate);
	MMatrix currentShoulderTransform();

	void LookAt(FVector TargetLocation);


	//new testing more arm targets
	FVector targetA;
	FVector targetB;
	FVector targetC;
	FVector targetD;
	float timeCopy;
	int direction = 1;

	

	FVector legTarget;
	float legTime = 0.0f;

	



	//debug var for hip leg switch







	//debug:
	float debugFlipTime = 0.0f;
	float debugStandAloneTime = 0.0f;
	float delay = 0.0f;

	FVector hipRelativeToFootRelativeTarget(FVector &other);
	


	//NEU
	class KeyFrameAnimation animationKeys_1;
	class KeyFrameAnimation legAnimationKeys;
	void standAloneKeyFrameAnim(BoneIk &bone, KeyFrameAnimation &frames, float DeltaTime);

	//neu 4
	void standAloneKeyFrameAnim(
		BoneIk &bone,
		KeyFrameAnimation &frames,
		MMatrix &initalTransform,
		float DeltaTime,
		FColor color
	);

	bool performRaycast(FVector &Start, FVector &dir, FVector &outputHit);

	void transformFromWorldToLocalCoordinates(FVector &position);




	//NEU 2
	class DoubleKeyFrameAnimation legDoubleKeys_1;
	class DoubleKeyFrameAnimation legDoubleKeys_2;
	void projectToGround(FVector &position);
	

	//NEU 3
	void projectToGround(FVector &frameToProject, FVector &offsetMade);



	//NEU MIT LEG SWITCH / SEPERATE MATRIX FOR LEGS!
	bool leg1isPlaying = true;
	void KeyFrameAnimAndHipAdjustTime(
		BoneIk &bone,
		DoubleKeyFrameAnimation &frames,
		MMatrix &footMatrix, // MMatrix foot transform
		float DeltaTime,
		FColor color
	);

	void buildRawAndKeepEndInPlace(BoneIk &boneIk, MMatrix &legTransform, float deltaTime, FColor color);
	void standAloneKeyFrameAnim(
		BoneIk &bone,
		DoubleKeyFrameAnimation &frames,
		MMatrix &footMatrix,
		float DeltaTime,
		FColor color
	);



	//create limbs section
	AActor *createLimbPivotAtTop(int x, int y, int height, int offsetY);



};
