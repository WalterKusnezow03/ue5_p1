#include "ComputerVisionActor.h"


#include "Engine/World.h"

#include "SceneViewExtension.h"
#include "LegacyScreenPercentageDriver.h"
#include "ComputerVisionPlugin/Public/ComputerVision/SiftFeatureExtraction/BlurredImage.h"
#include "ComputerVisionPlugin/Public/ComputerVision/SiftFeatureExtraction/ImageFeatureFinder.h"

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
    BuildProjectionMatrix();
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
    r.Yaw += DeltaTime * 3.0f;
    //r.Pitch += DeltaTime * 10.0f;
    SetActorRotation(r);

    FVector move = GetActorLocation() + FVector(100, 100, 0) * DeltaTime;
    SetActorLocation(move);

    //ReadPixels();




    //VERY SLOW.
    /*
    //match debug wise in tick.
    FScopeLock lock(&Mutex);
    if (features.Num() >= 2)
    {

        int prev = features.Num() - 2;
        int last = features.Num() - 1;
        Async(EAsyncExecution::ThreadPool, [
            this, 
            p1 = prev, 
            p2 = last, 
            f1 = features[prev],
            f2 = features[last]
        ]() mutable {
            RansacMatcher matcher;
            float lossmax = 100.0f; //10 pixels
            bool result = matcher.Match(features[p1], features[p2], lossmax);
            FString m = result ? TEXT("YES") : TEXT("NO");
            FString message = FString::Printf(TEXT("AComputerVisionActor::Matched %s"), *m);
            DebugHelper::logMessage(message);
        });

        


        

        //empty after
        //features.Empty();
    }*/
}











FRHITexture2D *AComputerVisionActor::GpuRhiTexture(){
    if(RenderTarget){
        FRHITexture2D* RHITexture = RenderTarget->GameThread_GetRenderTargetResource()->GetRenderTargetTexture();
        return RHITexture;
    }
    return nullptr;
}




#include "ComputerVisionPlugin/Public/ComputerVision/Actor/Storage/IDImageWriter.h"
bool AComputerVisionActor::TickCheckBufferCompleted(){
    //debug
    if(bufferPackage.refFlagCompleted()){
        //DebugHelper::logMessage("AComputerVisionActor::GpuCopy asyncMarkedCompleted");
    }


    if(bufferPackage.ProcessHasFinished()){
        //DebugHelper::logMessage("AComputerVisionActor::GpuCopy finished");
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
                //DebugHelper::logMessage("AComputerVisionActor::GpuCopy Image has NOT Changed");
                return true;
            }
        }
        LastCapturedPixels = colorBuffer;

        Async(EAsyncExecution::ThreadPool, [this, color = colorBuffer]() mutable
        {
            
            //!ACHTUNG! speichern erzeugt hier manchmal unsafe thread state. Achtung.
            

            /*DebugHelper::logMessage(
                FString::Printf(
                    TEXT("AComputerVisionActor::GpuCopy Color buffer size: (%d)"), color.Num()
                )
            );*/

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
                //DebugHelper::logMessage("AComputerVisionActor::Image::WriteImageRaw");
                IDImageWriter::SaveColorBufferAsPng(ptr, ResolutionX, ResolutionY, imageId);

                // debug blur all
                if (false)
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
                        if (saveImages && false)
                        {
                            //DebugHelper::logMessage("AComputerVisionActor::Image::WriteImageBlurred");
                            IDImageWriter::SaveColorBufferAsPng(ptrB, ResolutionX, ResolutionY, imageId);
                        }
                    }
                }

                // extrema detection output
                if (true)
                {
                    ImageFeatureFinder finder;
                    finder.ExtractFeatures(
                        color,
                        ResolutionX,
                        ResolutionY
                    );
                    //TryMatchFeatures(finder);
                    TArray<FColor> coloredEdges = finder.extremaAsColorBuffer();
                    uint8 *ptrB = (uint8 *)coloredEdges.GetData();
                    imageId++;
                    if (saveImages)
                    {
                        //ImageWriter::SaveColorBufferAsPng(ptrB, ResolutionX, ResolutionY, imageId);
                        
                        //single image overriden
                        IDImageWriter::SaveColorBufferAsPngFromName(
                            ptrB,
                            256,
                            256,
                            "ComputerVisionPlugin",
                            FString::Printf(TEXT("DOGImage")) //single image only.
                        );
                        DebugHelper::logMessage("AComputerVisionActor::Image::WriteImageDOG");
                    }
                }
            }
            

            
            
        });
        return true;
    }

    /*DebugHelper::showScreenMessage(
        FString::Printf(TEXT("AComputerVisionActor::%s"), *bufferPackage.flags()),
        FColor::Cyan
    );*/
    

    return false;
}



void AComputerVisionActor::TryMatchFeatures(ImageFeatureFinder &found){
    //MUTEX HERE
    FScopeLock Lock(&Mutex);
    features.Add(found);
}

void AComputerVisionActor::PrintBuffer(TArray<FColor> &colors){
    int log = 10;
    log = std::min(log, colors.Num());
    for (int i = 0; i < log; i++)
    {
        FColor &colorA = colors[i];
        /*DebugHelper::logMessage(
            FString::Printf(
                TEXT("AComputerVisionActor::GpuCopy::Color compare extracted %s"),
                *colorA.ToString()
            )
        );*/
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
    //DebugHelper::logMessage("AComputerVisionActor::ConvertRenderTargetToTensor::GpuCopy Launched");

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
        //DebugHelper::logMessage("AComputerVisionActor::TexturePtr is null!");
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

    /*DebugHelper::logMessage(
        FString::Printf(TEXT("AComputerVisionActor::LogGpuCopyTime %.2f"), deltatime)
    );*/
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








bool AComputerVisionActor::ProjectedBounds(AActor *actor, TArray<FVector2D> &pixelPositions){
    TArray<FVector2D> result;
    if(actor){
        FBox Box = actor->GetComponentsBoundingBox();
        FVector Center = Box.GetCenter(); //already in world space?
        FVector Extents = Box.GetExtent();

        //gehe von world space aus.
        //erstmal so lassen mit extends, könnte komische ergebnisse erzeugen.
        TArray<FVector> worldPositions;
        worldPositions.Add(Center - Extents);
        worldPositions.Add(Center + Extents);

        
        if(ProjectWorldPositionToRenderTarget(
            worldPositions,
            pixelPositions
        )){
            return true;
        }
    }

    return false;
}

bool AComputerVisionActor::ProjectWorldPositionToRenderTarget(
    TArray<FVector> &worldPositions,
    TArray<FVector2D>& OutPixels
){
    OutPixels.SetNum(worldPositions.Num());
    for (int i = 0; i < worldPositions.Num(); i++){
        if(!ProjectWorldPositionToRenderTarget(
            worldPositions[i],
            OutPixels[i] //so per ref trotzdem ok
        )){
            return false;
        }
    }
    return true;
}

//manual projection of camera coordinates
bool AComputerVisionActor::ProjectWorldPositionToRenderTarget(
    const FVector& WorldPosition,
    FVector2D& OutPixel
){
    if (!SceneCapture || !RenderTarget)
        return false;

    FMatrix View = SceneCapture->GetComponentTransform().ToInverseMatrixWithScale();
    FVector4 Clip = projectionMatrix.TransformFVector4(
        View.TransformFVector4(FVector4(WorldPosition, 1.0f))
    );

    if(Clip.W == 0.0f){
        return false;
    }
    //Check if point is behind the camera
    if(Clip.Z < 0.0f){
        return false;
    }

    OutPixel = ConvertNDCtoPixel(Clip);
    return true;
}

FVector2D AComputerVisionActor::ConvertNDCtoPixel(FVector4 &Clip){
    if(RenderTarget){
        //Perspective divide
        Clip.X /= Clip.W;
        Clip.Y /= Clip.W;
        //to screen
        float u = (Clip.X * 0.5f + 0.5f) * RenderTarget->SizeX;
        float v = (1.0f - (Clip.Y * 0.5f + 0.5f)) * RenderTarget->SizeY;
    }
    return FVector2D(0.0f, 0.0f);
}

void AComputerVisionActor::BuildProjectionMatrix(){
    const float FOV = FMath::DegreesToRadians(SceneCapture->FOVAngle);
    const float Near = 10.0f;  // Unreal-Default für SceneCapture
    const float Far  = 100000.0f; // Oder dein Capture->MaxViewDistanceOverride

    const float W = 1.0f / FMath::Tan(FOV * 0.5f);

    float Aspect = 1.0f;
    if (SceneCapture->TextureTarget)
    {
        Aspect = (float)SceneCapture->TextureTarget->SizeX / (float)SceneCapture->TextureTarget->SizeY;
    }

    projectionMatrix = FReversedZPerspectiveMatrix(
        W, 
        W * Aspect,
        Near,
        Far
    );
}


