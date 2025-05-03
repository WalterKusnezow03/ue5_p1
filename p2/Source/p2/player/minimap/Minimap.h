#pragma once

#include "CoreMinimal.h"
#include "p2/renderTargets/customRenderTarget/CustomRenderedTexture.h"
#include "p2/entities/customIk/MMatrix.h"
#include "Camera/CameraComponent.h"

#include "Minimap.generated.h"

UCLASS()
class P2_API AMinimap : public AActor {

GENERATED_BODY()

public:
    static AMinimap* Construct(
        UWorld *world, 
        UCameraComponent *cameraComponent
    );

    AMinimap();
    void BeginPlay() override;
    void Tick(float deltaTime) override;

    void updatePlayerPositionAndRotation(FVector &pos, FRotator &rot);

private:

    bool debugMessages = false;

    void deprojectCamera(
        UCameraComponent *Camera,
        TArray<FVector> &outCornerDirections
    );
    FVector findDir(TArray<FVector> &directions, float signY, float signZ);

    void attachToBottomLeftCorner(UCameraComponent *camera);
    void attachTo(
        UCameraComponent *camera,
        FVector &cornerDirection,
        float skalar
    );


    UStaticMeshComponent *Mesh = nullptr;
    UCustomRenderedTexture *texture = nullptr;

    void initialRotation();
    void initTexture();

    MMatrix playerMatrixInverted;
    FVector playerPosition;
    float minimapRadius = 20000; //20m

    void addPlayerMarker();

    void updateMiniMapItems();
    void transformToPlayerSpace(TArray<FVector> &positions);
    void transformToPlayerSpace(TArray<MMatrix> &positions);

    void transformToCanvasSpace(TArray<FVector> &position, FVector2D &canvasScale);
    void transformToCanvasSpace(TArray<MMatrix> &positions, FVector2D &canvasScale);

    float scaleToCanvasSpace(float xPos, float xCanvasScale, float mapsize);
};