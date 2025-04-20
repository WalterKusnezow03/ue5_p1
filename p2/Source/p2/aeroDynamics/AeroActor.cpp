#include "AeroActor.h"
#include "p2/meshgen/ELod.h"
#include "p2/gamestart/assetEnums/materialEnum.h"
#include "p2/gamestart/assetManager.h"
#include "p2/meshgen/MeshData/aeroDynamic/AeroMeshData.h"
#include "p2/meshgen/MeshData/MeshData.h"

AAeroActor::AAeroActor() : AcustomMeshActorBase(){

}

void AAeroActor::BeginPlay(){
    Super::BeginPlay();
    initMesh();
}


void AAeroActor::initMesh(){
    //materialEnum::wingMaterial
    
    
    //meshDataMain setup

    //
    if(Mesh){
        int layer = 0;
        bool enableCollision = true;

        updateMesh(
            *Mesh,
            meshDataMain, //MeshData & otherMesh,
            layer,
            enableCollision
        );

        ApplyMaterial(
            Mesh,
            wingMaterialPointer(),
            layer
        );
    }

    //ReloadMeshAndApplyAllMaterials(); // super
}

UMaterialInterface *AAeroActor::wingMaterialPointer(){
    UMaterialInterface *ptr = nullptr;
    if (assetManager *a = assetManager::instance())
    {
        a->findMaterial(materialEnum::wingMaterial);
    }
    return ptr;
}



void AAeroActor::Tick(float deltaTime){
    //Super::Tick(deltaTime);


    //todo: überlegen wie man wing mesh aufbaut und testet!
    //und an objekt bewegung anhängt!


}


FVector AAeroActor::transformVektorToLocalSpace(FVector &dir){

    FVector dirRelativeToVelocity = dir - velocity; //AB = B - A

    FRotator r = GetActorRotation();
    FMatrix rotationMatrix = FRotationMatrix(r);
    FMatrix InverseMatrix = rotationMatrix.GetTransposed(); //R^-1 = R^T

    FVector output = InverseMatrix.TransformVector(dirRelativeToVelocity);
    return output;
}
