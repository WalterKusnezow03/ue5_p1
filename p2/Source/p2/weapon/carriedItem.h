// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include <list>
#include "p2/entities/customIk/bonePackage/handPackage/HandTargetContainer.h"
#include "p2/entities/customIk/bonePackage/handPackage/HandBoneIndexEnum.h"

#include "IkHumanoidModell/carryItems/container/CarriedItemPositionData.h"
#include "IkHumanoidModell/carryItems/Interface/IkCarryInterface.h"

#include "carriedItem.generated.h"

UCLASS()
class P2_API AcarriedItem : public AActor, public IIkCarryInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AcarriedItem();

	// --- IK CARRY INTERFACE ---
	//target for hands, new IkHumanoid Plugin info container!
	virtual CarriedItemPositionData &getItemPositionDataRef() override;
	//must have this api to update from outside
	virtual void UpdateActorTransform(FVector &location, FRotator &rotation) override;


	// Api

	/// @brief pickup by player.
	/// @param cameraIn 
	void pickup(UCameraComponent *cameraIn);
	void pickupBot(AActor *actorIn); //pickup for bot!


	virtual void drop();
	bool isPickedupByPlayer();
	void showItem(bool show);

	virtual void leftMouseDown();
	virtual void leftMouseUp();

	bool isActive();


	/// @brief must be overriden for carried weapon, used also by new ik humanoid!
	virtual FVector leftHandLocation();

	/// @brief must be overriden for carried weapon, used also by new ik humanoid!
	virtual FVector rightHandLocation();

	virtual FVector leftHandFingerLocation(HandBoneIndexEnum type);
	virtual FVector rightHandFingerLocation(HandBoneIndexEnum type);

	virtual void loadFingerTargets(HandTargetContainer &container);

protected:
	CarriedItemPositionData internalCarriedItemPositionContainer;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// @brief will save whether the item is shown (selected or not.)
	bool isVisible;

	/// @brief shows or hides a child actor component if not nullptr
	/// @param component 
	/// @param show 
	void showChildActor(UChildActorComponent *component, bool show);

	//follow
	class UCameraComponent *cameraPointer; // why class, idk, just keep it like that
	class AActor *botPointer;
	
	FVector offset;

	///offset vector, overriden by weapon script
	virtual FVector getOffsetVector();
	

	void enableCollider(bool enable);

	void showScreenMessage(FString s);

	
	template <typename T>
	void findAllOfType(AActor &a, std::list<T *> &items);

	MMatrix handAlignForwardRotationMatrix();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void applyWeaponSway();

	
	virtual FVector actorAnimationOffsetLocal();
	
protected:
	FVector actorAnimationOffsetTicked;
	void updateAnimationOffset(FVector &other);
	void addToAnimationOffset(FVector &other);

	//sight offset for weapons
	FVector verticalSightOffset;

public:
	FVector sightOffsetNoRotation();
};
