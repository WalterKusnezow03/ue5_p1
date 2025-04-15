#pragma once

#include "CoreMinimal.h"
#include "MeshExtractiondebugActor.generated.h"

UCLASS()
class P2_API AMeshExtractiondebugActor : public AActor {
    GENERATED_BODY()

public:
    AMeshExtractiondebugActor();

    virtual void BeginPlay() override;
    virtual void Tick(float deltaTime) override;

private:
    void findAllStaticMeshes();

    UStaticMeshComponent *targetMesh = nullptr;
    UStaticMeshComponent *paneMesh = nullptr;

    void findPaneBounds(
        FVector2D &a,
        FVector2D &b,
        FVector &normal);
    void copyMeshData(
        UStaticMeshComponent *StaticMesh,
        TArray<FVector3f> &verteciesOut,
        TArray<int32> &trianglesOut);
    void scaleMeshVertecies(
        UStaticMeshComponent *comp,
        TArray<FVector3f> &vertecies);

    FVector findNormal(UStaticMeshComponent *comp);
};