#include "MeshDataAssetTask.h"


MeshDataAssetTask::MeshDataAssetTask(){

}

MeshDataAssetTask::~MeshDataAssetTask(){

}

MeshDataAssetTask::MeshDataAssetTask(const MeshDataAssetTask &other){
    if(this != &other){
        *this = other;
    }
}

MeshDataAssetTask &MeshDataAssetTask::operator=(const MeshDataAssetTask &other){
    if(this != &other){
        Update(other.vertexBufferSaved, other.triangleBufferSaved, other.uvBufferSaved);
        UpdatePathAndName(other.filePath, other.fileName);
    }
    return *this;
}

bool MeshDataAssetTask::IsValidTask(){
    if(filePath.Len() <= 0){
        return false;
    }
    if(fileName.Len() <= 0){
        return false;
    }
    if(vertexBufferSaved.Num() < 3){
        return false;
    }
    if(triangleBufferSaved.Num() < 3){
        return false;
    }
    return true;
}

void MeshDataAssetTask::Update(const TArray<FVector> &vertexBuffer, const TArray<int32> &triangleBuffer){
    vertexBufferSaved = vertexBuffer;
    triangleBufferSaved = triangleBuffer;
    uvBufferSaved.Empty();
}

void MeshDataAssetTask::Update(
    const TArray<FVector> &vertexBuffer, 
    const TArray<int32> &triangleBuffer,
    const TArray<FVector2D> &uvBufferIn 
){
    vertexBufferSaved = vertexBuffer;
    triangleBufferSaved = triangleBuffer;
    uvBufferSaved = uvBufferIn;
}

void MeshDataAssetTask::UpdatePathAndName(const FString path, const FString name){
    filePath = path;
    fileName = name;
}

//unclear if this is correct
FString MeshDataAssetTask::pathComplete(){
    FString output = filePath; //must have "/" at end
    output += fileName;
    //output += ".";
    //output += fileName;
    return output;
}

TArray<FVector> &MeshDataAssetTask::GetVertexBuffer(){
    return vertexBufferSaved;
}

TArray<int32> &MeshDataAssetTask::GetTriangleBuffer(){
    return triangleBufferSaved;
}

TArray<FVector2D> &MeshDataAssetTask::GetUV0(){
    return uvBufferSaved;
}