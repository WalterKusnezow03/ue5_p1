#include "RenderTargetActorBase.h"


ARenderTargetActorBase::ARenderTargetActorBase() : Super(){
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));;
    MeshToRenderOn = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RenderMesh"));
    MeshToRenderOn->SetupAttachment(RootComponent);

    // Disable collision completely
    MeshToRenderOn->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MeshToRenderOn->SetCollisionResponseToAllChannels(ECR_Ignore);

    // Load Engine's default Plane mesh
    static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshObj(
        TEXT("/Engine/BasicShapes/Plane.Plane")
    );

    if (PlaneMeshObj.Succeeded())
    {
        MeshToRenderOn->SetStaticMesh(PlaneMeshObj.Object);
    }
}


void ARenderTargetActorBase::BeginPlay(){
    Super::BeginPlay();
    TrySetup();
}

bool ARenderTargetActorBase::CanSetup(){
    return MeshToRenderOn != nullptr;
}

//to be called in tick if mesh set up later.
void ARenderTargetActorBase::TrySetup(){
    if(CanSetup()){
        SetupRenderTarget();
        ApplyMaterialToMesh();
    }
}


void ARenderTargetActorBase::SetupRenderTarget(){
    int resX = 128;
    int resY = 128;
    RenderedTexture = UCustomRenderedTexture::Construct(this, resX, resY);
    /*if(RenderedTexture){
        if(UCanvasRenderTarget2D *target = RenderedTexture->GetRenderTarget()){
            CaptureComponent->TextureTarget = target;
        }
    }*/
}

void ARenderTargetActorBase::ApplyMaterialToMesh(){
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



void ARenderTargetActorBase::Tick(float deltatime){
    Super::Tick(deltatime);
    
    //tick this or capture frame manually?
    //unclear
    if(RenderedTexture){
        RenderedTexture->Tick(deltatime);
    }
}
