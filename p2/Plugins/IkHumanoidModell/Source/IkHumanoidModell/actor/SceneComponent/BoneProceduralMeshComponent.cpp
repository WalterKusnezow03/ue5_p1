#include "BoneProceduralMeshComponent.h"
#include "DebugPlugin/DebugHelper.h"



#include "GameCore/MeshGenBase/MeshData/MeshData.h"
#include "IkHumanoidModell/Ik/Controller/ControllerSetup/Properties/FSingleLimbProperty.h"

int UBoneProceduralMeshComponent::staticId = 0;


UBoneProceduralMeshComponent::UBoneProceduralMeshComponent() : Super()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickInterval = 0.0f;  // default → tickt jeden Frame
    //PrimaryComponentTick.TickGroup = TG_PrePhysics; // oder TG_DuringPhysics etc.
}




void UBoneProceduralMeshComponent::MakeInstance(
    FSingleLimbProperty &property, 
    IBoneTransformInterface *bone, 
    int id
){
    //DebugHelper::logMessage("UBoneProceduralMeshComponent::TryMakeInstace");
    if (!bone)
    {
        DebugHelper::logMessage("UBoneProceduralMeshComponent::TryMakeInstace No Bone - failed!");
        return;
    }

    AActor *owner = property.GetActor();
    if (!owner)
    {
        DebugHelper::logMessage("UBoneProceduralMeshComponent::TryMakeInstace No Actor - failed!");
        return;
    }


    if(false){
        FString ownerName = owner->GetActorNameOrLabel();
        DebugHelper::logMessage("UBoneProceduralMeshComponent::AttachToActor ", ownerName);
    }

    if(UBoneProceduralMeshComponent *created = TryMakeInstance(owner)){
        created->Init(bone, id, property);
        //DebugHelper::logMessage("UBoneProceduralMeshComponent::Created Instance!");
    }
}

UBoneProceduralMeshComponent* UBoneProceduralMeshComponent::TryMakeInstance(AActor *owner){
    if(owner){

        if(USceneComponent *RootComponent = owner->GetRootComponent()){

            FString nameMesh = GetNextBoneName();
            UBoneProceduralMeshComponent *bone =
                NewObject<UBoneProceduralMeshComponent>(owner, *nameMesh);
            if(bone){
                /*if(true){ //supposed to be always true.
                    bone->SetupAttachment(RootComponent); 
                }*/

                bone->SetupAttachment(RootComponent); 

                
                bone->RegisterComponent(); //it is not visible otherwise
            }
            return bone;
        }
    }
    return nullptr;
}

FString UBoneProceduralMeshComponent::GetNextBoneName(){
    //id must be increased each time to allow a new component to exist.
    //otherwise the component / uobject will NOT be instanced, different name is needed!
    staticId++;
    FString nameMesh = FString::Printf(TEXT("UBoneProceduralMeshComponent_%d"), staticId);
    return nameMesh;
}

void UBoneProceduralMeshComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
    Super::OnComponentDestroyed(bDestroyingHierarchy);
    staticId = 0;
}

void UBoneProceduralMeshComponent::Init(
    IBoneTransformInterface *bone, 
    int id,
    FSingleLimbProperty &property
){
    boneInterface = bone;
    limbId = id;
    InitProceduralMesh();
    CreateLimb(property);
    SetPhysicsEnabled(false);
}

void UBoneProceduralMeshComponent::BeginPlay(){
    Super::BeginPlay();

}

void UBoneProceduralMeshComponent::InitProceduralMesh(){
    FString name = GetName(); //name setup from instance maker.
    int nameId = 0;
    meshComponentPair.init(name, nameId, this);
}

void UBoneProceduralMeshComponent::SetPhysicsEnabled(bool flag){
    meshComponentPair.SetPhysicsEnabled(flag);
}

void UBoneProceduralMeshComponent::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction
){
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    //DebugHelper::showScreenMessage("UBoneProceduralMeshComponent::Tick", FColor::Yellow);

    FVector locationUpdate;
    FRotator rotationUpdate;
    if(GetTransformFromInterface(locationUpdate, rotationUpdate)){
        //is called.

        
        //DebugShowComponentLocation();
        //DebugShowProceduralMeshComponentLocation();

        //is correct and tested now.
        SetWorldLocationAndRotation(
            locationUpdate, 
            rotationUpdate,
            false, 
            nullptr, 
            ETeleportType::TeleportPhysics
        );   
    }
}

bool UBoneProceduralMeshComponent::GetTransformFromInterface(
    FVector &location,
    FRotator &rotation
){
    if(boneInterface){
        return boneInterface->GetTransform(location, rotation, limbId);
    }
    return false;
}




//// ----- MESH DATA GEN -----
void UBoneProceduralMeshComponent::CreateLimb(FSingleLimbProperty &property){
    MeshData &data = FindMeshData(materialEnum::wallMaterial);
    CreateLimb(data, property);
    meshComponentPair.updateMeshAllAndApplyMaterials();
}

MeshData &UBoneProceduralMeshComponent::FindMeshData(materialEnum type){
    bool raycastOnLayer = true;
    return meshComponentPair.meshDataReference(type, raycastOnLayer);
}

void UBoneProceduralMeshComponent::CreateLimb(MeshData &data, FSingleLimbProperty &property){
    //property.DebugLogMessage("UBoneProceduralMeshComponent::OnSetup ");

    float x = property.GetWidth();
    float y = property.GetWidth();
    float height = property.GetSize();
    height = std::abs(height) * -1;

    //smaller distances allowed:
    data.MofidyEpsilon(0.1f); //smaller distances allowed, prevents removing triangles

    float xHalf = x / 2.0f;
    float yHalf = y / 2.0f;

    FVector pivot(-xHalf, -yHalf, 0.0f); //because pivot should be centered and cube is sized from pivot.
    data.appendCubeAt(pivot, x, y, height);
    data.calculateNormals();
}








// ---- DEBUG ----
void UBoneProceduralMeshComponent::LogActorAndComponentLocation(){
    DebugHelper::showScreenMessage("UBoneProceduralMeshComponent::Location", GetComponentLocation());
    if(AActor *owner = GetOwner()){
        DebugHelper::showScreenMessage("UBoneProceduralMeshComponent::ActorLocation", owner->GetActorLocation());
    }
}

void UBoneProceduralMeshComponent::LogProceduralMeshValid(){
    //DEBUG LOG NEEDED!
    bool isValid = meshComponentPair.IsValid(true); //raycast enabled mesh
    FString message = isValid ? 
        TEXT("UBoneProceduralMeshComponent::Mesh->Valid") : 
        TEXT("UBoneProceduralMeshComponent::Mesh->InValid");
    DebugHelper::logMessage(message);
}


void UBoneProceduralMeshComponent::DebugShowComponentLocation(){
    DebugShowLocation(GetComponentLocation(),FColor::Red);
}

void UBoneProceduralMeshComponent::DebugShowProceduralMeshComponentLocation(){
    bool raycastenabled = true;
    if (meshComponentPair.IsValid(raycastenabled)){
        DebugShowLocation(
            meshComponentPair.GetComponentLocation(raycastenabled),
            FColor::Cyan
        );
    }
}


void UBoneProceduralMeshComponent::DebugShowLocation(FVector location, FColor color){
    //debug
    if(true){
        //shows the correct world location.
        DebugHelper::showLineBetween(
            GetWorld(),
            location,
            FVector(0,0,0),
            //GetComponentLocation() - FVector(0,0,10),
            color,
            1.0f
        );
    }
}

