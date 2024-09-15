// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "p2/util/FVectorTouple.h"
#include "ProceduralMeshComponent.h"
#include "p2/interfaces/Damageinterface.h"
#include "p2/player/teamEnum.h"
#include "Components/BoxComponent.h"
#include "p2/gamestart/assetEnums/materialEnum.h"
#include "customMeshActor.generated.h"

UCLASS()
class P2_API AcustomMeshActor : public AActor, public IDamageinterface
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

	//derived methods
	//methods
	virtual void takedamage(int d) override;
	virtual void takedamage(int d, FVector &from) override;
	virtual void setTeam(teamEnum t) override;
	virtual teamEnum getTeam() override;

	//custom mesh actor methods

	static void splitAndreplace(AActor *actor, FVector &bottom, int cmTile, materialEnum materialType);

	void process2DMap(std::vector<std::vector<FVector>> &map);

	void createCube(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d,
		FVector &dir,
		int cmheight,
		UMaterial *material
	);

	void createCube(
		FVector &a,
		FVector &b,
		FVector &c,
		FVector &d,
		FVector &a1,
		FVector &b1,
		FVector &c1,
		FVector &d1,
		UMaterial *material
	);

	void createTwoSidedQuad(FVector &a, FVector &b, FVector &c, FVector &d, UMaterial *material);

	void init(materialEnum materialtype);

private:
	
	int health = 100;
	bool isDestructable();
	void setHealth(int d);

	teamEnum team;

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




	materialEnum materialtypeSet = materialEnum::grassMaterial;
};
