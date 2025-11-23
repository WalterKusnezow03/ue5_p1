#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TextureRenderTarget2D.h"
#include "RendererInterface.h"
#include "SceneView.h"
#include "CanvasTypes.h"
#include "EngineModule.h"
#include "RHICommandList.h"
#include "Engine/Engine.h"

#include "ComputerVisionPlugin/Public/ComputerVision/Actor/Package/BufferPackage.h"


#include "ComputerVisionActor.generated.h"

UCLASS()
class COMPUTERVISIONPLUGIN_API AComputerVisionActor : public AActor
{
    GENERATED_BODY()

public:
    AComputerVisionActor();

    // Parameter
    UPROPERTY(EditAnywhere, Category = "Bot Vision")
    int32 ResolutionX = 256;

    UPROPERTY(EditAnywhere, Category = "Bot Vision")
    int32 ResolutionY = 256;

    UPROPERTY(EditAnywhere, Category = "Bot Vision")
    float FOV = 90.0f;

    // Wird jeden Tick gerendert
    virtual void Tick(float DeltaTime) override;



protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

    UPROPERTY()
    USceneCaptureComponent2D *SceneCapture = nullptr;

    void CreateCaptureComponent();
    void CaptureWithCaptureComponent();
    void ConvertRenderTargetToTensor();
    FRHITexture2D *GpuRhiTexture();


    UPROPERTY()
    UTextureRenderTarget2D* RenderTarget;
    TArray<FColor> LastCapturedPixels;



    void ToColorBuffer(
        void *gpuTexture, 
        int sizeX, 
        int sizeY,
        TArray<FColor> &outColorBuffer
    );
    void PrintBuffer(TArray<FColor> &colors);

    TArray<FColor> ReadPixels();

    int32 imageId = 0;

    //debug
    void StartGpuCopyTime();
    void LogGpuCopyTime();
    float gpuCopyTimeStart = 0.0f;


    //new
    bool TickCheckBufferCompleted();
    BufferPackage bufferPackage;


};