#pragma once

#include "CoreMinimal.h"
#include "StorageInterface.h"
#include "StoragePlugin/Storage/VertexData/StorageInterface/enum/EFileType.h"

class STORAGEPLUGIN_API StorageInterfaceMeshData : public StorageInterface {

public:
    StorageInterfaceMeshData() {};
    virtual ~StorageInterfaceMeshData() override {};

    void Test();

    void SaveMeshData(
        TArray<FVector> &Vertecies,
        TArray<FVector> &Normals,
        TArray<FVector2D> &UV0,
        TArray<int32> &triangles,
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
    void writeInfoData(TArray<uint8> &Bytes, TArray<int32> &info);
    void loadInfoData(
        TArray<uint8> &bytes,
        int32 &vertexCount,
        int32 &normalCount,
        int32 &uvCount,
        int32 &triangleCount
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

    void PrintBuffers(
        TArray<FVector> &Vertecies,
        TArray<FVector> &Normals,
        TArray<FVector2D> &UV0,
        TArray<int32> &Triangles
    );
};