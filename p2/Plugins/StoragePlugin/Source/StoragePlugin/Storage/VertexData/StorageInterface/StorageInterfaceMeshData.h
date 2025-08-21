#pragma once

#include "CoreMinimal.h"
#include "StoragePlugin/Storage/BaseInterface/StorageInterface.h"


class STORAGEPLUGIN_API StorageInterfaceMeshData : public StorageInterface {

public:
    StorageInterfaceMeshData() {};
    virtual ~StorageInterfaceMeshData() override {};

    void Test();

    void SaveMeshData(
        TArray<FVector> &Vertecies,
        TArray<FVector> &Normals,
        TArray<FVector2D> &UV0,
        TArray<int32> &Triangles,
        FString path
    );

    void LoadMeshData(
        TArray<FVector> &Vertecies,
        TArray<FVector> &Normals,
        TArray<FVector2D> &UV0,
        TArray<int32> &Triangles,
        FString path
    );

    //specialized methods
    void SaveMeshData(
        TArray<FVector> &Vertecies,
        TArray<FVector> &Normals,
        TArray<FVector2D> &UV0,
        TArray<int32> &Triangles,
        int chunkId,
        int layer,
        int lod
    );

    void LoadMeshData(
        TArray<FVector> &Vertecies,
        TArray<FVector> &Normals,
        TArray<FVector2D> &UV0,
        TArray<int32> &Triangles,
        int chunkId,
        int layer,
        int lod
    );

private:
   
    //will write the info data and INCREASE THE POINTER TO START AFTER INFO DATA!
    void writeInfoData(
        int32 vertexCount,
        int32 normalCount,
        int32 uvCount,
        int32 triangleCount,
        uint8*& Ptr //immidiate byte offset for data Ptr, is increased!
    );

    void AppendIntoByteBuffer(
        TArray<uint8> &Bytes, // buffer size is increased after append!
        TArray<FVector> &Vertecies,
        TArray<FVector> &Normals,
        TArray<FVector2D> &UV0,
        TArray<int32> &Triangles
    );

    //will load the info data and INCREASE THE POINTER TO START AFTER INFO DATA!
    void loadInfoData(
        uint8*& Ptr, //Ptr already at given offset
        int32 &vertexCount,
        int32 &normalCount,
        int32 &uvCount,
        int32 &triangleCount
    );

    //will load mesh data into buffers, EXPECTS INFO DATA AT POINTER, 
    //WILL INCREASE POINTER ALL THE WAY TO START OF NEXT BUFFER CHUNK / MESHDATA
    //MIGHT BE OUT OF BOUNCE BY THEN!
    void LoadIntoMeshBuffers(
        TArray<uint8> &Bytes, // buffer size is increased after append!
        uint8*& Ptr,           // is increased after append, must be at correct offset starting with header bytes!
        TArray<FVector> &Vertecies,
        TArray<FVector> &Normals,
        TArray<FVector2D> &UV0,
        TArray<int32> &Triangles
    );

    FString makePath(
        int chunkId,
        int layer,
        int lod
    );

    int getInfoBytesSize();

    int getVertexBytesSize(int count);
    int getNormalsBytesSize(int count);
    int getUVBytesSize(int count);
    int getTrianglesBytesSize(int count);

    void PrintBinary(TArray<uint8> &bytes);
    void PrintBuffers(
        TArray<FVector> &Vertecies,
        TArray<FVector> &Normals,
        TArray<FVector2D> &UV0,
        TArray<int32> &Triangles);
};