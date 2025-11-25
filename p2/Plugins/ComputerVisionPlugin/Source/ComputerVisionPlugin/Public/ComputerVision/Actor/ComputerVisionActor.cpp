#include "ComputerVisionActor.h"


#include "Engine/World.h"

#include "SceneViewExtension.h"
#include "LegacyScreenPercentageDriver.h"
#include "ComputerVisionPlugin/Public/ComputerVision/FeatureExtraction/BlurredImage.h"
#include "ComputerVisionPlugin/Public/ComputerVision/FeatureExtraction/ImageFeatureFinder.h"

#include "DebugPlugin/DebugHelper.h"

AComputerVisionActor::AComputerVisionActor()
{
    PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

}

void AComputerVisionActor::BeginPlay()
{
    Super::BeginPlay();

    // Erstelle ein RenderTarget für das Bot-Bild
    RenderTarget = NewObject<UTextureRenderTarget2D>(this);
    //RenderTarget->InitAutoFormat(ResolutionX, ResolutionY);

    RenderTarget->bGPUSharedFlag = true; // zusätzlicher Hinweis (not tested)
    //RenderTarget->RenderTargetFormat = RTF_RGBA8; //??

    RenderTarget->InitCustomFormat(ResolutionX, ResolutionY, PF_B8G8R8A8, true);
    RenderTarget->bGPUSharedFlag = true;//helps?
    RenderTarget->ClearColor = FLinearColor::Black;
    RenderTarget->UpdateResourceImmediate(true);

    CreateCaptureComponent();
}

void AComputerVisionActor::EndPlay(const EEndPlayReason::Type EndPlayReason){
    
    Super::EndPlay(EndPlayReason);
}

//void AComputerVisionActor::EndPlay()


void AComputerVisionActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if(TickCheckBufferCompleted()){
        //CaptureViewNew();
        CaptureWithCaptureComponent();
        ConvertRenderTargetToTensor();
    }

    FRotator r = GetActorRotation();
    r.Yaw += DeltaTime * 10.0f;
    //r.Pitch += DeltaTime * 10.0f;
    SetActorRotation(r);

    FVector move = GetActorLocation() + FVector(100, 100, 0) * DeltaTime;
    SetActorLocation(move);

    //ReadPixels();
}











FRHITexture2D *AComputerVisionActor::GpuRhiTexture(){
    if(RenderTarget){
        FRHITexture2D* RHITexture = RenderTarget->GameThread_GetRenderTargetResource()->GetRenderTargetTexture();
        return RHITexture;
    }
    return nullptr;
}




#include "ComputerVisionPlugin/Public/ComputerVision/Actor/Storage/ImageWriter.h"
bool AComputerVisionActor::TickCheckBufferCompleted(){
    //debug
    if(bufferPackage.refFlagCompleted()){
        DebugHelper::logMessage("AComputerVisionActor::GpuCopy asyncMarkedCompleted");
    }


    if(bufferPackage.ProcessHasFinished()){
        DebugHelper::logMessage("AComputerVisionActor::GpuCopy finished");
        LogGpuCopyTime();

        TArray<FColor> colorBuffer;
        ToColorBuffer(
            bufferPackage.refDataPtr(),
            ResolutionX,
            ResolutionY,
            colorBuffer
        );
        if (colorBuffer.Num() > 0){
            // compare buffer
            if (colorBuffer == LastCapturedPixels)
            {
                DebugHelper::logMessage("AComputerVisionActor::GpuCopy Image has NOT Changed");
                return true;
            }
        }
        LastCapturedPixels = colorBuffer;

        Async(EAsyncExecution::ThreadPool, [this, color = colorBuffer]() mutable
        {
            
            
            

            DebugHelper::logMessage(
                FString::Printf(
                    TEXT("AComputerVisionActor::GpuCopy Color buffer size: (%d)"), color.Num()
                )
            );

            // debug use unreal own method
            // color = ReadPixels(); ---> bricked!

            // DEBUG
            bool saveImages = true;

            if (color.Num() > 0)
            {
                
                // DebugHelper::logMessage("AComputerVisionActor::GpuCopy Image has Changed");
                // PrintBuffer(color);

                uint8 *ptr = (uint8 *)color.GetData(); // bufferPackage.DataPtrAsUint8()
                imageId++;
                DebugHelper::logMessage("AComputerVisionActor::Image::WriteImageRaw");
                ImageWriter::SaveColorBufferAsPng(ptr, ResolutionX, ResolutionY, imageId);

                // debug blur all
                if (true)
                {
                    BlurredImage image;
                    image.BlurImage(
                        color,
                        ResolutionX,
                        ResolutionY,
                        5, // int k,
                        1.5f
                    );
                    image.ApplyGrayScale();

                    if (image.IsValid(ResolutionX, ResolutionY))
                    {
                        uint8 *ptrB = image.RawColorArrayPtr();
                        imageId++;
                        if (saveImages)
                        {
                            DebugHelper::logMessage("AComputerVisionActor::Image::WriteImageBlurred");
                            ImageWriter::SaveColorBufferAsPng(ptrB, ResolutionX, ResolutionY, imageId);
                        }
                    }
                }

                // extrema detection
                if (true)
                {
                    ImageFeatureFinder finder;
                    finder.ExtractFeatures(
                        color,
                        ResolutionX,
                        ResolutionY
                    );
                    TArray<FColor> coloredEdges = finder.extremaAsColorBuffer();
                    uint8 *ptrB = (uint8 *)coloredEdges.GetData();
                    imageId++;
                    if (saveImages)
                    {
                        ImageWriter::SaveColorBufferAsPng(ptrB, ResolutionX, ResolutionY, imageId);
                        DebugHelper::logMessage("AComputerVisionActor::Image::WriteImageDOG");
                    }
                }
            }
            

                
            
        });
        return true;
    }

    DebugHelper::showScreenMessage(
        FString::Printf(TEXT("AComputerVisionActor::%s"), *bufferPackage.flags()),
        FColor::Cyan
    );
    

    return false;
}

void AComputerVisionActor::PrintBuffer(TArray<FColor> &colors){
    int log = 10;
    log = std::min(log, colors.Num());
    for (int i = 0; i < log; i++)
    {
        FColor &colorA = colors[i];
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("AComputerVisionActor::GpuCopy::Color compare extracted %s"),
                *colorA.ToString()
            )
        );
    }
}





#include "ComputerVision/TensorBridge/TensorTextureBridge.h"
void AComputerVisionActor::ConvertRenderTargetToTensor()
{
    if (!RenderTarget){
        return;
    }
        

    
    FRHITexture2D *RHITexture = GpuRhiTexture();
    id<MTLTexture> MetalTex = (id<MTLTexture>)RHITexture->GetNativeResource();
    if (!MetalTex)
    {
        UE_LOG(LogTemp, Error, TEXT("AComputerVisionActor::Metal Texture is null!"));
        return;
    }

    





    //copy data from gpu
    StartGpuCopyTime();
    DebugHelper::logMessage("AComputerVisionActor::ConvertRenderTargetToTensor::GpuCopy Launched");

    bufferPackage.MarkLaunchCopy();
    bool &refFlag = bufferPackage.refFlagCompleted();

    void *&gpuTexture = bufferPackage.refDataPtr(); 
    // nullptr; // must be asyn cchecked / saved statically for
    // async copy operation bitwise image copy!


    
    BridgeTextureRead(
    //BridgeTextureReadSharedBuffer(
        MetalTex,
        RenderTarget->SizeX,
        RenderTarget->SizeY,
        gpuTexture, //gpuTexture,
        refFlag
    );
    /*
    if (gpuTexture)
    {
        LogGpuCopyTime();
        DebugHelper::logMessage("AComputerVisionActor::metalCopySucess");

        //use.
    }*/
}


// --- not tested! ---
void AComputerVisionActor::ToColorBuffer(
    void *voidGputexturePointer, 
    int sizeX, 
    int sizeY,
    TArray<FColor> &outColorBuffer
){
    if(!voidGputexturePointer){
        DebugHelper::logMessage("AComputerVisionActor::TexturePtr is null!");
        return;
    }




    //this is correct, dont touch

    
    uint8* gpuTexture = static_cast<uint8*>(voidGputexturePointer);
    int bytesPerPixel = 4;
    int sizeBuffer = sizeX * sizeY;
    int bytesNum = sizeBuffer * bytesPerPixel;
    outColorBuffer.SetNumUninitialized(sizeBuffer);
    

    for (int i = 0; i < sizeBuffer; i++){

        uint8 r;
        uint8 g;
        uint8 b;
        uint8 a;

        FMemory::Memcpy(&r, gpuTexture, sizeof(uint8));
        FMemory::Memcpy(&g, gpuTexture + sizeof(uint8), sizeof(uint8));
        FMemory::Memcpy(&b, gpuTexture + sizeof(uint8) * 2, sizeof(uint8));
        FMemory::Memcpy(&a, gpuTexture + sizeof(uint8) * 3, sizeof(uint8));
        a = 255;
        outColorBuffer[i] = FColor(b, g, r, a); // FColor(r, g, b, a);

        //go one step (r,g,b,a a 8 bit) = 32bit = 4 byte
        uint8 step = sizeof(uint8) * 4;
        gpuTexture += step;
        
    }
}

TArray<FColor> AComputerVisionActor::ReadPixels(){
    if(RenderTarget){
        StartGpuCopyTime();
        FTextureRenderTargetResource *RTResource = RenderTarget->GameThread_GetRenderTargetResource();
        if(RTResource){
            TArray<FColor> OutPixels;
            RTResource->ReadPixels(OutPixels);

            for (int i = 0; i < OutPixels.Num(); i++){
                FColor &current = OutPixels[i];
                current.A = 1.0f;
            }

            LogGpuCopyTime();
            return OutPixels;
        }
    }
    TArray<FColor> empty;
    return empty;
}

/// log
void AComputerVisionActor::StartGpuCopyTime(){
    gpuCopyTimeStart = FPlatformTime::Seconds();
}

void AComputerVisionActor::LogGpuCopyTime(){
    //hier output ca 70ms, schlecht!
    float deltatime = FPlatformTime::Seconds() - gpuCopyTimeStart; // AB = B - A
    deltatime = std::max(0.0f, deltatime);

    DebugHelper::logMessage(
        FString::Printf(TEXT("AComputerVisionActor::LogGpuCopyTime %.2f"), deltatime)
    );
}









#include "Components/SceneCaptureComponent2D.h"

void AComputerVisionActor::CreateCaptureComponent(){
    if(!SceneCapture){

        /*//something is messed up here!
        SceneCapture = NewObject<USceneCaptureComponent2D>(this);
        SceneCapture->RegisterComponent();
        SceneCapture->SetRelativeLocation(FVector(100,0,0));
        SceneCapture->SetRelativeRotation(FRotator(0,0,0));
        SceneCapture->SetupAttachment(RootComponent);*/

        // 1. Attach bevor registrieren
        SceneCapture = NewObject<USceneCaptureComponent2D>(this);
        SceneCapture->SetupAttachment(RootComponent);

        // 2. Optional: Position + Rotation
        SceneCapture->SetRelativeLocation(FVector(100,0,0));
        SceneCapture->SetRelativeRotation(FRotator(0,0,0));

        // 3. Komponente registrieren
        SceneCapture->RegisterComponent();

    }
}

void AComputerVisionActor::CaptureWithCaptureComponent(){
    if(SceneCapture && RenderTarget)
    {
        SceneCapture->TextureTarget = RenderTarget;
        SceneCapture->CaptureScene();
    }
}



