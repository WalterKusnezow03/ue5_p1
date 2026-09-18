#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "p2/renderTargets/customRenderTarget/CustomRenderedTexture.h"
#include "GameCore/util/ActorBase/ActorBase.h"
#include "RenderTargetActorBase.generated.h"

UCLASS()
class ARenderTargetActorBase : public AActorBase
{
    GENERATED_BODY()

public:
    ARenderTargetActorBase();

    

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;

private:
    bool CanSetup();
    void TrySetup();

    void SetupRenderTarget();
    void ApplyMaterialToMesh();

    

protected:
    UPROPERTY()
    UStaticMeshComponent* MeshToRenderOn;
    
    UPROPERTY()
    UCustomRenderedTexture *RenderedTexture;
};