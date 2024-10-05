// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "p2/gamestart/assetEnums/skelletonControllerEnum.h"
#include "skelletonController.generated.h"

UCLASS()
class P2_API AskelletonController : public AActor, public IDamageinterface
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

	//damage interface methods
	virtual void takedamage(int d) override; //= 0 schreiben damit sie pure virtual sind
	virtual void takedamage(int d, FVector &from) override;
	virtual void setTeam(teamEnum t) override;
	virtual teamEnum getTeam() override;

	void setOwner(IDamageinterface *owner);
	void enableActiveStatus(bool enable);

private:
	class IDamageinterface *owningEntity = nullptr; //must be reset on death

public:
	//release
	void die();


	//skelleton
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skelletal Mesh To Control"

	//skelleton uproperty enum val
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Skelletal Mesh To Control")
	skelletonControllerEnum type;

	skelletonControllerEnum getType();

	//attach somehow
	void attachToLeftArm(AActor *other);
	void attachToRightArm(AActor *other);
	void detach(AActor *other);
	void attachToBreastRight(AActor *other);

	//new should allow to attach the hand itself too
	void attachLeftArmTo(AActor *other, FName otherSocketName);
	void attachRightArmTo(AActor *other, FName otherSocketName);


private:
	class USkeletalMeshComponent *skelletonComponentPointer = nullptr;

	static USkeletalMeshComponent* findSkeleton(AActor *actor);
	void attachToSocket(AActor *actorToAttach, FName socketName);


	//names
	FName leftHandSocket = "handL_socket";
	FName rightHandSocket = "handR_socket";
	FName breastRightSocket = "breastShaftSocket";

	class UskelletonAnimInstance *animInstance = nullptr;

	void setupAnimInstance();
	void updateIkPositions();
	FVector boneWorldLocation(FName boneName);

	class TargetMesh{
		public:
			void init(AActor *actor, FName socketName);
			void reset();
			FVector socketLocation();

			bool isInited();

			TargetMesh();
			~TargetMesh();
		
		private:
			USkeletalMeshComponent *skeletonPointer = nullptr;
			FName targetSocket;
			AActor *actorPointer = nullptr;
		
	};

	TargetMesh leftHandTargetMesh;
	TargetMesh rightHandTargetMesh;
};
