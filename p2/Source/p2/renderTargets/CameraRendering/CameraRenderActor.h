#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "p2/renderTargets/customRenderTarget/CustomRenderedTexture.h"
#include "CameraRenderActor.generated.h"

UCLASS()
class ACameraRenderActor : public AActor
{
    GENERATED_BODY()

public:
    ACameraRenderActor();

    

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;

private:
    bool CanSetup();
    void TrySetup();

    void SetupRenderTarget();
    void ApplyMaterialToMesh();

    UPROPERTY()
    UStaticMeshComponent* MeshToRenderOn;

    UPROPERTY()
    USceneCaptureComponent2D* CaptureComponent;

    UPROPERTY()
    UCustomRenderedTexture *RenderedTexture;
};