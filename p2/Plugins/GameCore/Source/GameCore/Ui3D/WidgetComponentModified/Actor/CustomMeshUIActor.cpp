#include "CustomMeshUIActor.h" 

#include "CoreMath/Matrix/MMatrix.h"
#include "Kismet/KismetMathLibrary.h"
#include "DebugPlugin/DebugHelper.h"
 
ACustomMeshUIActor::ACustomMeshUIActor() 
{ 
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it. 
    PrimaryActorTick.bCanEverTick = true; 
    PrimaryActorTick.bTickEvenWhenPaused = true; 
 
    // Create the widget component as root
    Widget = CreateDefaultSubobject<UAnyMeshWidgetComponent>(TEXT("Widget")); 
    RootComponent = Widget;
    
    // Set scale to 0.5 on all axes 
    //Widget->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f)); 
 
    Widget->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); 
    Widget->SetCollisionResponseToAllChannels(ECR_Ignore); 
    Widget->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); 
    Widget->SetCollisionResponseToChannel(ECC_Camera, ECR_Block); 
    Widget->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block); 
 
    Widget->SetTwoSided(true); 
    Widget->SetBlendMode(EWidgetBlendMode::Transparent); 
     
    Widget->SetDrawAtDesiredSize(true); 
    Widget->SetGeometryMode(EWidgetGeometryMode::Plane); //custom meshdata on plane mode.
    //EWidgetGeometryMode::Cylinder
    //Widget->SetCylinderArcAngle(30.0f);

	
} 
 
// Called when the game starts or when spawned 
void ACustomMeshUIActor::BeginPlay() 
{ 
    Super::BeginPlay();
    debugUiActorName = TEXT("ACustomMeshUIActorDefaultName");
    SetWidgetClassOnBeginPlay();
    CreateWidgetMeshData();
}

void ACustomMeshUIActor::SetWidgetClassOnBeginPlay(){
    SetWidgetClass(CurrentWidgetClass);
}

void ACustomMeshUIActor::ScaleMeshDataToMaxCm(float scaleTargetIn){
    if(Widget){
        MeshData &data = Widget->GetMeshDataRef();
        data.RebuildBounds();

        float cmCurrent = data.extent().Size();
        DebugHelper::logMessage("ACustomMeshUIActor::ScaleTo (cmCurrent: ", cmCurrent);
        DebugHelper::logMessage("ACustomMeshUIActor::ScaleTo (target: ", scaleTargetIn);

        if(cmCurrent < 0.1f){
            cmCurrent = 1;
        }

        // scalar = distTarget / distAll
        float scalar = scaleTargetIn / cmCurrent;

        MMatrix scale;
        scale.scaleUniform(scalar);
        DebugHelper::logMessage(FString::Printf(TEXT("ACustomMeshUIActor::ScaleTo (scalar: %.2f"), scalar));

        data.transformAllVertecies(scale);


        //print after scale
        data.RebuildBounds();
        cmCurrent = data.extent().Size();
        DebugHelper::logMessage("ACustomMeshUIActor::ScaleTo (cm after scale: ", cmCurrent);
    }
}



void ACustomMeshUIActor::CreateWidgetMeshData(){
    if(!bMeshDataCreated && Widget){
        FVector v0(0, 0, 0);
        FVector v1(0, 0, 100);
        FVector v2(0, 200, 100);
        FVector v3(0, 200, 50);

        MeshData &data = Widget->GetMeshDataRef();
        data.append(v0,v1,v2);
        data.append(v0,v2,v3);

        FVector2D uv0(0, 0);
        FVector2D uv1(0, 1);
        FVector2D uv2(1, 1);
        FVector2D uv3(1, 0);
        data.appendUvs(uv0, uv1, uv2);
        data.appendUvs(uv0, uv2, uv3);
    

        data.calculateNormals();
        bMeshDataCreated = true;
    }
}

void ACustomMeshUIActor::SetDrawSize(FVector2D size){
    if(Widget){
        Widget->SetDrawSize(size);
    }
    
}

// Called every frame 
void ACustomMeshUIActor::Tick(float DeltaTime) 
{ 
    Super::Tick(DeltaTime);
}

void ACustomMeshUIActor::SetWidgetClass(TSubclassOf<UUserWidget> NewWidgetClass) 
{ 
    if (NewWidgetClass) 
    {
       CurrentWidgetClass = NewWidgetClass; 
       Widget->SetWidgetClass(NewWidgetClass);
    } 
} 
 
TSubclassOf<UUserWidget> ACustomMeshUIActor::GetWidgetClass() const 
{ 
    return CurrentWidgetClass;
}





bool ACustomMeshUIActor::RayIntersect(
    const FVector &origin,
    const FVector &direction
){
    if(Widget && AllowRayIntersectInteraction()){
        return Widget->RayIntersect(origin, direction);
    }
    return false;
}


bool ACustomMeshUIActor::RayIntersectHover(
    const FVector &origin,
    const FVector &direction
){
    if(Widget && AllowRayIntersectInteraction()){
        return Widget->RayIntersectHover(origin, direction);
    }
    return false;
}

void ACustomMeshUIActor::UpdateVisibilityAndCollision(bool flag){
    SetActorHiddenInGame(!flag);
    EnableCollision(flag);
}

void ACustomMeshUIActor::EnableCollision(bool flag){
    if(Widget){
        if(!flag){
            Widget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }else{
            Widget->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); 
            Widget->SetCollisionResponseToAllChannels(ECR_Ignore); 
            Widget->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); 
            Widget->SetCollisionResponseToChannel(ECC_Camera, ECR_Block); 
            Widget->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block); 
        }
    }
    
}



