#include "MeshExtractedActor.h"

#include "MeshDataPlugin/Public/Extraction/ExtractionByBounds/MeshExtractorByBounds.h"
#include "AssetPlugin/gamestart/assetManager.h"

void AMeshExtractedActor::BeginPlay(){
    Super::BeginPlay();
    ExtractMeshDataOnBeginPlay();
}

void AMeshExtractedActor::ExtractMeshDataOnBeginPlay(){
    UStaticMeshComponent *exterior = nullptr;
    TTryAssignByNameContains<UStaticMeshComponent>(meshExteriorName, exterior);

    UStaticMeshComponent *plane = nullptr;
    TTryAssignByNameContains<UStaticMeshComponent>(meshPlaneName, plane);

    if(exterior && plane && Widget){
        bMeshDataCreated = true;

        ELod lodMade = ELod::lodNear;

        MeshData &meshData = Widget->GetMeshDataRef();

        /*MeshData &meshData = findMeshDataReference(
            matType,
            lodMade,
            true //bool raycastOnLayer
        );*/
        
        MeshExtractorByBounds extractionPipe;
        extractionPipe.ExtractSingleMeshDataByBounds(
            exterior, //UStaticMeshComponent *componentToExtractFrom, 
            meshData, //MeshData &meshData,
            plane, //UStaticMeshComponent *boundsComponent //must be attached inside component to extract from(!)
            FVector(0,0,1)
        );
        //ReloadMeshForMaterialByLod(lodMade, matType);

        meshData.flipAllTriangles();

        MMatrix transform(GetActorLocation());
        meshData.debugDrawMesh(transform, GetWorld());

        plane->SetHiddenInGame(true);
        //exterior->SetHiddenInGame(true);

        //ReloadMeshAndApplyAllMaterials();
    }
}




void AMeshExtractedActor::Tick(float deltatime){
    Super::Tick(deltatime);

    /*if(!loadedMaterials){
        if (assetManager *e = assetManager::instance())
        {
            UMaterial *materialFound = e->Find<materialEnum, UMaterial>(matType);
            if(materialFound){
                ReloadMeshAndApplyAllMaterials();
                loadedMaterials = true;
            }
        }
    }*/

    
}