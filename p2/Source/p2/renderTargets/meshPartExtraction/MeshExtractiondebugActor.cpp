#include "MeshExtractiondebugActor.h"
#include "StaticMeshBoundsextractor.h"
#include "GameCore/DebugHelper.h"
#include <algorithm>

AMeshExtractiondebugActor::AMeshExtractiondebugActor(){

}

void AMeshExtractiondebugActor::BeginPlay(){
    Super::BeginPlay();
    findAllStaticMeshes();

    FVector2D a;
    FVector2D b;
    FVector normal;
    findPaneBounds(a,b,normal);

    TArray<FVector3f> verteciesTargetMesh;
    TArray<int32> trianglesTargetMesh;
    copyMeshData(
        targetMesh,
        verteciesTargetMesh,
        trianglesTargetMesh
    );

    FVector paneLocalLocation = paneMesh->GetRelativeLocation();
    DebugHelper::logMessage("extracted relative location ", paneLocalLocation);

    StaticMeshBoundsextractor extractor;
    extractor.CopyInBounds(
        a,
        b,
        normal,
        verteciesTargetMesh,
        trianglesTargetMesh,
        paneLocalLocation,
        GetActorLocation(),
        GetWorld()
    );
}

void AMeshExtractiondebugActor::Tick(float deltaTime){
    Super::Tick(deltaTime);


}


void AMeshExtractiondebugActor::findAllStaticMeshes(){
    TArray<UStaticMeshComponent*> components; //create t array
    GetComponents<UStaticMeshComponent>(components); // ask actor for components

    // Add each component to the output array
    for (UStaticMeshComponent* Component : components)
    {
        if (Component)
        {
			FString name = Component->GetName();

			if (name.Contains("targetMesh")){
                targetMesh = Component;
            }
            else if (name.Contains("pane")){
                paneMesh = Component;
            }
        }
	}
}


void AMeshExtractiondebugActor::findPaneBounds(
    FVector2D &a,
    FVector2D &b,
    FVector &normal
){
    if(!paneMesh){
        return;
    }
    TArray<FVector3f> verteciesOut;
    TArray<int32> trianglesOut;
    copyMeshData(paneMesh, verteciesOut, trianglesOut);
    scaleMeshVertecies(
        paneMesh,
        verteciesOut
    );

    //find bounds
    for (int i = 0; i < verteciesOut.Num(); i++){
        FVector3f &vertex = verteciesOut[i];
        a.X = std::min(static_cast<float>(vertex.X), static_cast<float>(a.X));
        b.X = std::max(static_cast<float>(vertex.X), static_cast<float>(b.X));
        a.Y = std::min(static_cast<float>(vertex.Y), static_cast<float>(a.Y));
        b.Y = std::max(static_cast<float>(vertex.Y), static_cast<float>(b.Y));
        //a.Z = std::min(vertex.Z, a.Z);
        //b.Z = std::max(vertex.Z, b.Z);
    }

    DebugHelper::logMessage("extracted bounds x", a.X);
    DebugHelper::logMessage("extracted bounds x", b.X);
    DebugHelper::logMessage("extracted bounds y", a.Y);
    DebugHelper::logMessage("extracted bounds y", b.Y);


    // find normal
    normal = findNormal(paneMesh);
}

void AMeshExtractiondebugActor::copyMeshData(
    UStaticMeshComponent* StaticMeshComponent,
    TArray<FVector3f> &verteciesOut,
    TArray<int32> &trianglesOut
){
    if(!StaticMeshComponent){
        return;
    }

    UStaticMesh *StaticMesh = StaticMeshComponent->GetStaticMesh();
    if(StaticMesh){
        int32 LODIndex = 0;
        const FStaticMeshLODResources& LOD = StaticMesh->GetRenderData()->LODResources[0]; // meist LOD 0
        const FRawStaticIndexBuffer& IndexBuffer = LOD.IndexBuffer;
        int32 IndexCount = IndexBuffer.GetNumIndices();
        
        trianglesOut.Reserve(IndexCount);
        for (int32 i = 0; i < IndexCount; ++i){
            trianglesOut.Add(IndexBuffer.GetIndex(i));
        }

        const FPositionVertexBuffer& PositionVertexBuffer = LOD.VertexBuffers.PositionVertexBuffer;
        int vertexBufferSize = PositionVertexBuffer.GetNumVertices();
        verteciesOut.SetNum(vertexBufferSize);
        for (uint32 VertexIndex = 0; VertexIndex < PositionVertexBuffer.GetNumVertices(); VertexIndex++)
        {
            verteciesOut[VertexIndex] = PositionVertexBuffer.VertexPosition(VertexIndex);
        }

    }


}

void AMeshExtractiondebugActor::scaleMeshVertecies(
    UStaticMeshComponent *comp,
    TArray<FVector3f> &vertecies
){
    FVector Scale = comp->GetComponentTransform().GetScale3D();
    for (int i = 0; i < vertecies.Num(); i++){
        vertecies[i] *= Scale.X;
        vertecies[i] *= Scale.Y;
        vertecies[i] *= Scale.Z;
    }
}



FVector AMeshExtractiondebugActor::findNormal(
    UStaticMeshComponent *comp
){
    //OK
    FVector outNormal(0, 0, 1); //looks up by default
    if(comp){
        FRotator Rotation = comp->GetComponentRotation();
        //Rotation.Pitch += 90.0f;
        FMatrix RotationMatrix = FRotationMatrix(Rotation);
        outNormal = RotationMatrix.TransformPosition(outNormal);
    }

    //draw normal
    FVector location = comp->GetComponentLocation();
    DebugHelper::showLineBetween(
        GetWorld(),
        location,
        location + outNormal * 100.0f,
        FColor::Red,
        100.0f
    );

    return outNormal;
}