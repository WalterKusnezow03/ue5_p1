// Copyright Walter Kusnezow All Rights Reserved.
#include "MeshDataProjectable.h"
#include "CoreMath/util/Raycaster.h"

void MeshDataProjectable::ProjectIntoDirection(
    const FVector &actorOffset,
    FVector &direction,
    float sizeRaycast,
    UWorld *world
){
    if(!world){
        return;
    }
    FVector dirScaled = direction * sizeRaycast;
    Raycaster raycaster;
    bool traceComplex = true;

    for (int i = 0; i < vertecies.Num(); i++){
        FVector &vertex = vertecies[i];
        
        FVector start = actorOffset + vertex + dirScaled * 0.1f;
        if(raycaster.performRaycast(
            world,
            start,
            dirScaled,
            vertex, //FVector &outputHit, -> auto override
            traceComplex
        )){
            //vertex = outHit;
        }
    }

    //recalculate UVS if needed.


}


void MeshDataProjectable::CreateGrid(float sizeX, float sizeY, float detail){
    sizeX = std::abs(sizeX);
    sizeY = std::abs(sizeY);

    float halfSizeX = sizeX * 0.5f;
    float halfSizeY = sizeY * 0.5f;

    FVector bottomLeft(-halfSizeX, -halfSizeY, 0.0f);
    FVector topRight(halfSizeX, halfSizeY, 0.0f);

    /*
    y
    |
    v1-->v2
    |     |
    v0<--v3 -->x
    */
    float uvX = 0.0f;
    float uvY = 0.0f;

    //detail * a = sizeX
    //a = sizeX / detail

    float uvXStep = detail / sizeX; // scalar = distTarget / distAll

    for (float i = 0; i < sizeX; i += detail){
        for (float j = 0; j < sizeY; j += detail){
            FVector v0 = bottomLeft + FVector(i, j, 0);
            FVector v3 = bottomLeft + FVector(i + detail, j, 0);

            FVector v1 = v0 + FVector(0.0f, j, 0);
            FVector v2 = v3 + FVector(0.0f, j + detail, 0);

            append(v0, v1, v2, v3);

            float _u0 = i / sizeX;
            float _u3 = (i + detail) / sizeX;
            float _v0 = j / sizeY;
            float _v1 = (j + detail) / sizeY;
            FVector2D uv0(_u0, _v0); // entspricht v0
            FVector2D uv1(_u0, _v1); // entspricht v1
            FVector2D uv2(_u3, _v1); // entspricht v2
            FVector2D uv3(_u3, _v0); // entspricht v3
            appendUvs(uv0, uv1, uv2, uv3);
        }
    }

    calculateNormals();
}
