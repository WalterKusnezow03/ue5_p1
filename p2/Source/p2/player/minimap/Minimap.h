#pragma once

#include "CoreMinimal.h"
#include "p2/renderTargets/customRenderTarget/CustomRenderedTexture.h"
#include "p2/entities/customIk/MMatrix.h"

#include "Minimap.generated.h"

UCLASS()
class P2_API AMinimap : public AActor {

GENERATED_BODY()

public:
    static AMinimap* Construct(UWorld *world, AActor *attachto);

    AMinimap();
    void BeginPlay() override;
    void Tick(float deltaTime) override;

    void updatePlayerPositionAndRotation(FVector &pos, FRotator &rot);

private:

    UStaticMeshComponent *Mesh = nullptr;
    UCustomRenderedTexture *texture = nullptr;

    void initialRotationAndScale();
    void initTexture();

    MMatrix playerMatrixInverted;
    FVector playerPosition;
    float minimapRadius = 20000; //20m


    void updateMiniMapItems();
    void transformToPlayerSpace(TArray<FVector> &positions);
    void transformToCanvasSpace(TArray<FVector> &position, FVector2D &canvasScale);
    float scaleToCanvasSpace(float xPos, float xCanvasScale, float mapsize);
};