#include "AnyMeshWidgetExtracted.h"
#include "MeshDataPlugin/Public/Extraction/ExtractionByBounds/MeshExtractorByBounds.h"
#include "DebugPlugin/DebugHelper.h"

void UAnyMeshWidgetExtracted::BeginPlay(){
    Super::BeginPlay(); //tries to load meshdata!
    LoadMeshDataFromDisk();
    FindComponentsOnBeginPlayAndLoadMeshData();
}

void UAnyMeshWidgetExtracted::FindComponentsOnBeginPlayAndLoadMeshData(){

    UStaticMeshComponent *plane = FindStaticMeshComponent(planeChildName);
    UStaticMeshComponent *extractFrom = FindStaticMeshComponent(extractFromMeshName);

    
    if(!meshDataWasLoaded()){
        //extract!
        if(plane && extractFrom){

            MeshData &meshData = GetMeshDataRef();

            MeshExtractorByBounds extractionPipe;
            extractionPipe.ExtractSingleMeshDataByBounds(
                extractFrom, //UStaticMeshComponent *componentToExtractFrom, 
                meshData, //MeshData &meshData,
                plane, //UStaticMeshComponent *boundsComponent //must be attached inside component to extract from(!)
                FVector(0,0,5) //offset from original meshdata
            );
            //meshData.flipAllTriangles();
            SaveMeshDataToDisk();
            if(AActor *owner = GetOwner()){
                MMatrix transform(owner->GetActorLocation());
                meshData.debugDrawMesh(transform, GetWorld(), FColor::Green);
            }


            // ---- mesh data immidiatly bricked after Load! ----
            LoadMeshDataFromDisk();

            if(AActor *owner = GetOwner()){
                MeshData &meshDataReload = GetMeshDataRef();
                FVector offset = owner->GetActorLocation() + FVector(10, 0, 0);
                MMatrix transform(offset);
                meshDataReload.debugDrawMesh(transform, GetWorld(), FColor::Cyan);
            }
        }
    }else{
        //mesh data was already loaded!
        MeshData &meshData = GetMeshDataRef();
        DebugHelper::logMessage("UAnyMeshWidgetExtracted::loaded Mesh Data! ", meshData.getVerteciesRef().Num());

        
    }
    allowRender = true;

    if(plane){
        SetResolution(plane);
        plane->SetHiddenInGame(true);
    }
}


void UAnyMeshWidgetExtracted::SetResolution(UStaticMeshComponent *plane){
	if(plane){
        FTransform Transform = plane->GetComponentTransform();
        FVector Scale = Transform.GetScale3D();
        //FVector2D Scale2D(Scale.X, Scale.Y);

        FVector2D Scale2D(Scale.Y, Scale.X);
        FVector2D scaleMeters = Scale2D * 100.0f;
        UAnyMeshWidgetComponentBase::SetResolution(scaleMeters);
    }
}




void UAnyMeshWidgetExtracted::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}



UStaticMeshComponent *UAnyMeshWidgetExtracted::FindStaticMeshComponent(
    FString subString
){
    if(AActor *a = GetOwner()){
        return FindStaticMeshComponent(subString, a);
    }
    return FindStaticMeshComponent(subString, this);
}

UStaticMeshComponent *UAnyMeshWidgetExtracted::FindStaticMeshComponent(
    FString subString,
    AActor *parent
){  
    if(parent){
        TArray<USceneComponent *> childs;
        parent->GetComponents<USceneComponent>(childs);
        if(USceneComponent *actorRoot = parent->GetRootComponent()){
            if(childs.Contains(actorRoot) == false){
                childs.Add(actorRoot);
            }
        }
        return FindStaticMeshComponent(subString, childs);
    }
    return nullptr;
}


UStaticMeshComponent *UAnyMeshWidgetExtracted::FindStaticMeshComponent(
    FString subString,
    TArray<USceneComponent *> &childs
){
    for (int i = 0; i < childs.Num(); i++){
        if(USceneComponent *current = childs[i]){    
            if(UStaticMeshComponent *found = FindStaticMeshComponent(subString, current)){
                return found;
            }
        }
    }
    return nullptr;
}


UStaticMeshComponent *UAnyMeshWidgetExtracted::FindStaticMeshComponent(
    FString subString,
    USceneComponent *child
){      
    if(child){
        if(UStaticMeshComponent *casted = Cast<UStaticMeshComponent>(child)){
            if(casted->GetName().Contains(subString)){
                return casted;
            }
        }
        TArray<USceneComponent *> childsLower = child->GetAttachChildren();
        return FindStaticMeshComponent(subString, childsLower);
    }
    return nullptr;
}
