#include "CameraRenderActor.h"


ACameraRenderActor::ACameraRenderActor() : Super(){
    PrimaryActorTick.bCanEverTick = true;


    CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
    RootComponent = CaptureComponent;

    MeshToRenderOn = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RenderMesh"));
    MeshToRenderOn->SetupAttachment(RootComponent);

    // Capture Settings (To be explored, dont know any of them.)
    //CaptureComponent->bCaptureEveryFrame = true;
    CaptureComponent->bCaptureOnMovement = false;
    CaptureComponent->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
}


void ACameraRenderActor::BeginPlay(){
    Super::BeginPlay();
    TrySetup();
}

bool ACameraRenderActor::CanSetup(){
    return MeshToRenderOn != nullptr;
}

//to be called in tick if mesh set up later.
void ACameraRenderActor::TrySetup(){
    if(CanSetup()){
        SetupRenderTarget();
        ApplyMaterialToMesh();
    }
}


void ACameraRenderActor::SetupRenderTarget(){
    int resX = 128;
    int resY = 128;
    RenderedTexture = UCustomRenderedTexture::Construct(this, resX, resY);
    if(RenderedTexture){
        if(UCanvasRenderTarget2D *target = RenderedTexture->GetRenderTarget()){
            CaptureComponent->TextureTarget = target;
        }
    }
}

void ACameraRenderActor::ApplyMaterialToMesh(){
    if(MeshToRenderOn){
        if(RenderedTexture){
            UMaterialInterface *material = RenderedTexture->getMaterial();
            if(material){
                int layer = 0;
                MeshToRenderOn->SetMaterial(0, material);
            }
        }
    }
}



void ACameraRenderActor::Tick(float deltatime){
    Super::Tick(deltatime);
    
    //tick this or capture frame manually?
    //unclear
    if(RenderedTexture){
        RenderedTexture->Tick(deltatime);
    }
}
