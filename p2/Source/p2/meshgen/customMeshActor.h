// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "p2/util/FVectorTouple.h"
#include "ProceduralMeshComponent.h"
#include "customMeshActor.generated.h"

UCLASS()
class P2_API AcustomMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AcustomMeshActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	static void splitAndreplace(AActor *actor, FVector &bottom, int cmTile);

	void process2DMap(std::vector<std::vector<FVector>> &map);

	void createCube(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d,
		FVector &dir,
		int cmheight
	);
	
	void createCube(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d,
		FVector &a1,
		FVector &b1,
		FVector &c1,
		FVector &d1
	);

	void createTwoSidedQuad(FVector &a, FVector &b, FVector &c, FVector &d, UMaterial *material);

private:
	//saving data too, will be overriden when updateMesh is called
	TArray<FVector> vertecies;
	TArray<int32> triangles;
	TArray<FVector> normals;

	UPROPERTY(VisibleAnywhere)
	class UProceduralMeshComponent *Mesh;


	void updateMesh(
		TArray<FVector> newvertecies, 
		TArray<int32> newtriangles
	);


	void buildQuad(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d,
		TArray<FVector> &output,
		TArray<int32> &trianglesOutput
	);

	void buildTriangle(
		FVector &a,
		FVector &b,
		FVector &c,
		TArray<FVector> &output,
		TArray<int32> &trianglesOutput
	);

	void ApplyMaterial(UProceduralMeshComponent *ProceduralMeshComponent, UMaterial *Material);

	void createFoliage(TArray<FVectorTouple> &touples);
};
