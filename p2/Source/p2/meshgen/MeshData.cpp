// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/meshgen/MeshData.h"

MeshData::MeshData()
{
}

MeshData::MeshData(TArray<FVector> &&verteciesIn, TArray<int> &&trianglesIn){
    setVertecies(MoveTemp(verteciesIn));
    setTriangles(MoveTemp(trianglesIn));
}

MeshData::~MeshData()
{
    clearMesh();
}


/// @brief copy constructor gets called if passing by value
/// @param other 
MeshData::MeshData(const MeshData &other){
    *this = other;
}

/// @brief abusing the assign operator to create a deepcopy of the data
/// @param other 
/// @return 
MeshData& MeshData::operator=(const MeshData &other){
    if(&other != this){
        clearMesh(); //clear mesh data before adding any data!

        for (int i = 0; i < other.vertecies.Num(); i++){
            vertecies.Add(other.vertecies[i]);
        }
        for (int i = 0; i < other.triangles.Num(); i++){
            triangles.Add(other.triangles[i]);
        }
        for (int i = 0; i < other.normals.Num(); i++){
            normals.Add(other.normals[i]);
        }
        for (int i = 0; i < other.UV0.Num(); i++){
            UV0.Add(other.UV0[i]);
        }
        for (int i = 0; i < other.Tangents.Num(); i++){
            Tangents.Add(other.Tangents[i]);
        }
        for (int i = 0; i < other.VertexColors.Num(); i++){
            VertexColors.Add(other.VertexColors[i]);
        }
    }
    return *this;
}


/// @brief clean up data completely
void MeshData::clearMesh(){
    vertecies.Empty();
    triangles.Empty();
    normals.Empty();

    UV0.Empty();
    Tangents.Empty();
    VertexColors.Empty();
}

/// @brief clears the normals 
void MeshData::clearNormals(){
    normals.Empty();
}

/**
 * --- apply data ---
 */

/// @brief calculates the normals and applies it to the vertecies
void MeshData::calculateNormals(){
    UKismetProceduralMeshLibrary::CalculateTangentsForMesh(vertecies, triangles, UV0, normals, Tangents);
}

/// @brief sets the data for all vertecies, pass by r value reference
/// @param vertecies veretecies to set, be carefull with overriding
void MeshData::setVertecies(TArray<FVector> &&verteciesIn){
    vertecies = MoveTemp(verteciesIn);  // Move the data instead of copying, creating an r value
}
/// @brief sets the data for all triangles, pass by r value reference
/// @param trianglesIn triangles to set for the mesh
void MeshData::setTriangles(TArray<int32> &&trianglesIn){
    triangles = MoveTemp(trianglesIn);
}

//join another mesh, vertecies add, triangles added with offset added to index
void MeshData::append(MeshData &other){
    int triangleOffset = triangles.Num();

    //copy triangles, apply offset
    for (int i = 0; i < other.triangles.Num(); i++){
        int32 copy = other.triangles[i];
        copy += triangleOffset;
        triangles.Add(copy);
    }

    //copy vertecies
    for(int i = 0; i < other.vertecies.Num(); i++){
        FVector &ref = other.vertecies[i];
        vertecies.Add(ref);
    }
}





/**
 * --- Data references ---
 */

/// @brief returns the reference to the mesh data vertecies, be carefull with modifying
/// @return mesh data vertecies by reference
TArray<FVector> &MeshData::getVerteciesRef(){
    return vertecies;
}

/// @brief returns the reference to the mesh data triangles, be carefull with modifying
/// @return mesh data triangles by reference
TArray<int32> &MeshData::getTrianglesRef(){
    return triangles;
}

TArray<FVector> &MeshData::getNormalsRef(){
    return normals;
}

TArray<FVector2D> &MeshData::getUV0Ref(){
    return UV0;
}
TArray<FProcMeshTangent> &MeshData::getTangentsRef(){
    return Tangents;
}
TArray<FColor> &MeshData::getVertexColorsRef(){
    return VertexColors;
}