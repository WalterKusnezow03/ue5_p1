#include "StaticMeshMeshExtractor.h"


StaticMeshMeshExtractor::StaticMeshMeshExtractor(){

}

StaticMeshMeshExtractor::~StaticMeshMeshExtractor(){
    
}

void StaticMeshMeshExtractor::extract(
    UStaticMeshComponent *MeshComponent,
    TArray<FVector> &vertecies,
    TArray<FVector> &normals,
    TArray<int32> &trianlges
){


    if (MeshComponent && MeshComponent->GetStaticMesh())
    {
        //needs the transform to revert the world space
        const FTransform LocalToWorldTransform = MeshComponent->GetComponentTransform();

        getEdgesFromSingleMesh(
            MeshComponent->GetStaticMesh(), 
            LocalToWorldTransform,
            vertecies,
            normals,
            trianlges
        );
    }

}






// Function to get all vertical edges of a mesh as copy
void StaticMeshMeshExtractor::getEdgesFromSingleMesh(
    UStaticMesh* StaticMesh,
    FTransform LocalToWorldTransform,
    TArray<FVector> &OutVertices,
    TArray<FVector> &OutNormals,
    TArray<int32> &OutIndices
){
    if (!StaticMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("StaticMesh is null."));
        return;
    }

    // Ensure we have the render data for the mesh
    const FStaticMeshRenderData* RenderData = StaticMesh->GetRenderData();
    if (!RenderData) 
    {
        UE_LOG(LogTemp, Warning, TEXT("RenderData is null."));
        return;
    }

    // Iterate through all LODs (Levels of Detail)
    for (int32 LODIndex = 0; LODIndex < RenderData->LODResources.Num(); LODIndex++)
    {
        const FStaticMeshLODResources& LODResources = RenderData->LODResources[LODIndex];

        // Iterate through all the vertex buffers
        const FPositionVertexBuffer& PositionVertexBuffer = LODResources.VertexBuffers.PositionVertexBuffer;
        const FStaticMeshVertexBuffer& StaticMeshVertexBuffer = LODResources.VertexBuffers.StaticMeshVertexBuffer;
        const FIndexArrayView Indices = LODResources.IndexBuffer.GetArrayView();

        // Extract vertices
        for (uint32 VertexIndex = 0; VertexIndex < PositionVertexBuffer.GetNumVertices(); VertexIndex++) {
            FVector WorldPosition = LocalToWorldTransform.TransformPosition(
                FVector(PositionVertexBuffer.VertexPosition(VertexIndex))
            );
            OutVertices.Add(WorldPosition);
        }

        // Extract normals
        for (uint32 VertexIndex = 0; VertexIndex < StaticMeshVertexBuffer.GetNumVertices(); VertexIndex++) {
            FVector Normal = FVector(StaticMeshVertexBuffer.VertexTangentZ(VertexIndex)); 
            Normal = LocalToWorldTransform.TransformVector(Normal);
            OutNormals.Add(Normal);
        }

        // Extract indices
        for (int32 Index = 0; Index < Indices.Num(); Index++) {
            OutIndices.Add(Indices[Index]);
        }


    }
}