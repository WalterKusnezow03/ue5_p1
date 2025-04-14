#include "StaticMeshSimplifier.h"


StaticMeshSimplifier::StaticMeshSimplifier(){

} 

StaticMeshSimplifier::~StaticMeshSimplifier(){

}

void StaticMeshSimplifier::append(
    TArray<FVector3f> &vertexBuffer,
    TArray<int32> &trianglesIn
){
    for (int i = 2; i < trianglesIn.Num(); i += 3){
        int32 v0 = trianglesIn[i - 2];
        int32 v1 = trianglesIn[i - 1];
        int32 v2 = trianglesIn[i];
        if(isValidVertexIndex(v0,v1,v2, vertexBuffer)){
            appendEfficent(
                vertexBuffer[v0],
                vertexBuffer[v1],
                vertexBuffer[v2]
            );
        }
    }
    cleanupTriangleBuffer();
}

void StaticMeshSimplifier::appendEfficent(
    FVector3f &a,
    FVector3f &b,
    FVector3f &c
){
    int offset = vertecies.Num();

    int indexA = alreadyHasVertex(a);
    int indexB = alreadyHasVertex(b);
    int indexC = alreadyHasVertex(c);

    int debugEfficentAdded = 3;

    //add if not found correctly
    if(!isValidVertexIndex(indexA)){
        vertecies.Add(a);
        indexA = vertecies.Num() - 1; //0
        debugEfficentAdded--;
    }
    if(!!isValidVertexIndex(indexB)){
        vertecies.Add(b);
        indexB = vertecies.Num() - 1; //1
        debugEfficentAdded--;
    }
    if(!!isValidVertexIndex(indexC)){
        vertecies.Add(c);
        indexC = vertecies.Num() - 1; //2
        debugEfficentAdded--;
    }
    //add to triangle buffer
    triangles.Add(indexA);
    triangles.Add(indexB);
    triangles.Add(indexC);




}


/// @brief returns -1 or index if found
/// @param other 
/// @return 
int StaticMeshSimplifier::alreadyHasVertex(FVector3f &other){
    int index = -1;
    float dist = epsilon;
    for (int i = 0; i < vertecies.Num(); i++){
        float distnew = FVector3f::Dist(vertecies[i], other);
        if (distnew < dist){
            dist = distnew;
            index = i;
        }
    }

    return index;
}

bool StaticMeshSimplifier::isValidVertexIndex(int index){
    return isValidVertexIndex(index, vertecies);
}

bool StaticMeshSimplifier::isValidVertexIndex(int index, TArray<FVector3f> &verteciesIn){
    return index >= 0 && index < verteciesIn.Num();
}

bool StaticMeshSimplifier::isValidVertexIndex(
    int v0, 
    int v1, 
    int v2, 
    TArray<FVector3f> &verteciesIn
){
    return isValidVertexIndex(v0, verteciesIn) &&
           isValidVertexIndex(v1, verteciesIn) &&
           isValidVertexIndex(v2, verteciesIn);
}



void StaticMeshSimplifier::cleanupTriangleBuffer(){

    for (int i = 2; i < triangles.Num(); i+=3){
        int32 v0 = triangles[i - 2];
        int32 v1 = triangles[i - 1];
        int32 v2 = triangles[i];

        int j = i + 3;
        int size = triangles.Num();
        while(j < size){
            if(j >= 2 && j < triangles.Num()){
                int32 &t0 = triangles[j - 2];
                int32 &t1 = triangles[j - 1];
                int32 &t2 = triangles[j];

                if(t0 == v0 && t1 == v2 && t2 == v2){
                    int sizeBuffer = triangles.Num();
                    t0 = triangles[sizeBuffer - 3];
                    t1 = triangles[sizeBuffer - 2];
                    t2 = triangles[sizeBuffer - 1];

                    triangles.Pop();
                    triangles.Pop();
                    triangles.Pop();
                    size = triangles.Num();
                    j -= 3;
                }
            }

            j += 3;
        }
    }
}