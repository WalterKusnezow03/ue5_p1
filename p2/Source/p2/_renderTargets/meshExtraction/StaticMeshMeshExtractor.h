#pragma once


#include "CoreMinimal.h"

class P2_API StaticMeshMeshExtractor{

public:
    StaticMeshMeshExtractor();
    ~StaticMeshMeshExtractor();

    //method for extract, get vertecies, normals and triangle buffer
    void extract(
        UStaticMeshComponent *comp,
        TArray<FVector> &vertecies,
        TArray<FVector> &normals,
        TArray<int32> &trianlges
    );

private:
    void getEdgesFromSingleMesh(
        UStaticMesh *StaticMesh,
        FTransform LocalToWorldTransform,
        TArray<FVector> &OutVertices,
        TArray<FVector> &OutNormals,
        TArray<int32> &OutIndices
    );
};