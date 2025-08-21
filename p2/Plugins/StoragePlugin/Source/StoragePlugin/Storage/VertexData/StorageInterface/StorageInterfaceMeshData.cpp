#include "StorageInterfaceMeshData.h"

#include "Misc/FileHelper.h"
#include "StoragePlugin/Storage/VertexData/TerrainVertex.h"
#include "StoragePlugin/Storage/VertexData/TerrainNormal.h"
#include "StoragePlugin/Storage/VertexData/TerrainUv.h"
#include "StoragePlugin/Storage/VertexData/TerrainVertexIndex.h"
#include "GameCore/DebugHelper.h"


//-------- SAVING --------

void StorageInterfaceMeshData::SaveMeshData(
    TArray<FVector> &Vertecies,
    TArray<FVector> &Normals,
    TArray<FVector2D> &UV0,
    TArray<int32> &Triangles,
    int chunkId,
    int layer,
    int lod
){
    //vertex buffer size, und normal buffer size sind gleich, muss codiert werden!
    //triangle buffer size muss codiert werden!

    TArray<uint8> Bytes;

    int32 vertexCount = Vertecies.Num();
    int32 normalCount = Normals.Num();
    int32 uvCount = UV0.Num();
    int32 triangleCount = Triangles.Num();

    //int32 is 4 bytes
    //MUST BE THE SAME IN LOADING METHOD!
    int infoBytesSize = getInfoBytesSize(); 
    int verteciesByteSize = getVertexBytesSize(Vertecies.Num());
    int normalsByteSize = getNormalsBytesSize(Normals.Num());
    int uv0ByteSize = getUVBytesSize(UV0.Num());
    int trianglesByteSize = getTrianglesBytesSize(Triangles.Num());

    int completeSize =
        infoBytesSize +
        verteciesByteSize +
        normalsByteSize +
        uv0ByteSize +
        trianglesByteSize;

    //write info and data size
    Bytes.SetNumUninitialized(completeSize); //SET SIZE VERY IMPORTANT

    //write info data
    TArray<int32> infos = {
        vertexCount,
        normalCount,
        uvCount,
        triangleCount
    };
    writeInfoData(Bytes, infos);
    

    //write mesh data
    uint8* Ptr = Bytes.GetData(); //get data pointer to work with
    Ptr += infoBytesSize;         // get to offset adress after info data!

    /*
    FMemory::Memcpy ( 
        void* Dest,
        const void* Src,
        SIZE_T Count (in bytes if using unit8*)
    )
    */
    FMemory::Memcpy(Ptr, (uint8 *)Vertecies.GetData(), verteciesByteSize); //copy casted data
    Ptr += verteciesByteSize; //increase pointer adress

    FMemory::Memcpy(Ptr, (uint8 *)Normals.GetData(), normalsByteSize); //copy
    Ptr += normalsByteSize; //increase pointer adress

    FMemory::Memcpy(Ptr, (uint8 *)UV0.GetData(), uv0ByteSize); //copy
    Ptr += uv0ByteSize; //increase pointer adress

    FMemory::Memcpy(Ptr, (uint8 *)Triangles.GetData(), trianglesByteSize); //copy
    //Ptr += trianglesByteSize; //increase pointer adress (not needed, end of data)

    






    // Save
    SaveBinaryData(
        makePath(chunkId, layer, lod),
        Bytes
    );

}


void StorageInterfaceMeshData::writeInfoData(TArray<uint8> &Bytes, TArray<int32> &info){
    uint8* Ptr = Bytes.GetData();
    for (int i = 0; i < info.Num(); i++){
        int32 *infoCurrent = &info[i];
        /*
        FMemory::Memcpy ( 
            void* Dest,
            const void* Src,
            SIZE_T Count
        )
        */
        FMemory::Memcpy(Ptr, infoCurrent, sizeof(int32)); //copy
        Ptr += sizeof(int32); //increase pointer adress
    }

    //how it works in theory:
    /*
    FMemory::Memcpy(Ptr, &vertexCount, sizeof(int32));
    Ptr += sizeof(int32);
    
    FMemory::Memcpy(Ptr, &normalCount, sizeof(int32));
    Ptr += sizeof(int32);

    FMemory::Memcpy(Ptr, &uvCount, sizeof(int32));
    Ptr += sizeof(int32);

    FMemory::Memcpy(Ptr, &triangleCount, sizeof(int32));*/
}


//------- SIZE OF HELPER ---------
int StorageInterfaceMeshData::getInfoBytesSize(){
    return sizeof(int32) * 4;
}

int StorageInterfaceMeshData::getVertexBytesSize(int count){
    return sizeof(FTerrainVertex) * count;
}

int StorageInterfaceMeshData::getNormalsBytesSize(int count){
    return sizeof(FTerrainNormal) * count;
}

int StorageInterfaceMeshData::getUVBytesSize(int count){
    return sizeof(FTerrainUV) * count;
}

int StorageInterfaceMeshData::getTrianglesBytesSize(int count){
    return sizeof(FTerrainVertexIndex) * count;
}





//-------- LOADING --------

void StorageInterfaceMeshData::LoadMeshData(
    TArray<FVector> &Vertecies,
    TArray<FVector> &Normals,
    TArray<FVector2D> &UV0,
    TArray<int32> &Triangles,
    int chunkId,
    int layer,
    int lod
){
    /*
    FMemory::Memcpy ( 
        void* Dest,
        const void* Src,
        SIZE_T Count
    )
    */
    FString path = makePath(chunkId, layer, lod);
    TArray<uint8> bytes;
    if(!LoadBinaryData(path, bytes)){
        DebugHelper::logMessage("Storage Interface ERROR LOADING BIN DATA");
    }

    //load info data
    int32 vertexCount = 0;
    int32 normalCount = 0;
    int32 uvCount = 0;
    int32 triangleCount = 0;

    loadInfoData(bytes, vertexCount, normalCount, uvCount, triangleCount);

    //SET SIZE VERY IMPORTANT
    Vertecies.SetNumUninitialized(vertexCount);
    Normals.SetNumUninitialized(normalCount);
    UV0.SetNumUninitialized(uvCount);
    Triangles.SetNumUninitialized(triangleCount);

    //copy mesh data based on infoData (make void* its cooler)
    int infoBytesSize = getInfoBytesSize();


    int verteciesByteSize = getVertexBytesSize(vertexCount);
    int normalsBytesSize = getNormalsBytesSize(normalCount);
    int uvBytes = getUVBytesSize(uvCount);
    int trianglesByteSize = getTrianglesBytesSize(triangleCount);

    //verify size
    int totalSize =
        infoBytesSize +
        verteciesByteSize +
        normalsBytesSize +
        uvBytes +
        trianglesByteSize;

    if(totalSize != sizeof(uint8) * bytes.Num()){
        return;
    }

    uint8 *Ptr = bytes.GetData();
    Ptr += infoBytesSize;
    /*
    FMemory::Memcpy (
        void* Dest,
        const void* Src,
        SIZE_T Count
    )
    */

    FMemory::Memcpy((uint8 *)Vertecies.GetData(), Ptr, verteciesByteSize); // copy casted data
    Ptr += verteciesByteSize;                                             // increase pointer adress

    
    FMemory::Memcpy((uint8 *)Normals.GetData(), Ptr, normalsBytesSize); // copy casted data
    Ptr += normalsBytesSize;                                            // increase pointer adress

    FMemory::Memcpy((uint8 *)UV0.GetData(), Ptr, uvBytes); // copy casted data
    Ptr += uvBytes;                                            // increase pointer adress

    FMemory::Memcpy((uint8 *)Triangles.GetData(), Ptr, trianglesByteSize); // copy casted data
    


}




void StorageInterfaceMeshData::loadInfoData(
    TArray<uint8> &bytes,
    int32 &vertexCount,
    int32 &normalCount,
    int32 &uvCount,
    int32 &triangleCount
){
    uint8 *Ptr = bytes.GetData();

    TArray<int32 *> infoData = {
        &vertexCount,
        &normalCount,
        &uvCount,
        &triangleCount
    };


    /*
    FMemory::Memcpy ( 
        void* Dest,
        const void* Src,
        SIZE_T Count
    )
    */
    for (int i = 0; i < infoData.Num(); i++){
        FMemory::Memcpy(infoData[i], Ptr, sizeof(int32));
        Ptr += sizeof(int32);
    }
}









// ------- MAKE PATH ------
FString StorageInterfaceMeshData::makePath(
    int chunkId,
    int layer,
    int lod
){
    FString fileName = FString::Printf(TEXT("Terrain/Chunk_%.d_Layer_%.d_Lod_%d.bin"), chunkId, layer, lod);
    FString Path = FPaths::ProjectSavedDir() + fileName;
    return Path;
}


// ------- TEST ---------
void StorageInterfaceMeshData::Test(){
    TArray<FVector> vertexBuffer = {
        FVector(0, 0, 1),
        FVector(1, 0, 2),
        FVector(2, 0, 3)
    };

    TArray<FVector> normalBuffer = {
        FVector(0, 2, 0),
        FVector(0, 3, 0),
        FVector(0, 4, 0)
    };

    TArray<FVector2D> uvBuffer = {
        FVector2D(0,0),
        FVector2D(0,1),
        FVector2D(1,0)
    };

    TArray<int32> triangleBuffer = {
        0,1,2,
        0,2,1
    };

    //before save
    PrintBuffers(vertexBuffer, normalBuffer, uvBuffer, triangleBuffer);

    //save
    SaveMeshData(
        vertexBuffer,
        normalBuffer,
        uvBuffer,
        triangleBuffer,
        0,
        0,
        0
    );

    vertexBuffer.Empty();
    normalBuffer.Empty();
    uvBuffer.Empty();
    triangleBuffer.Empty();

    //load
    LoadMeshData(
        vertexBuffer,
        normalBuffer,
        uvBuffer,
        triangleBuffer,
        0,
        0,
        0
    );

    //print
    PrintBuffers(vertexBuffer, normalBuffer, uvBuffer, triangleBuffer);
}

void StorageInterfaceMeshData::PrintBuffers(
    TArray<FVector> &Vertecies,
    TArray<FVector> &Normals,
    TArray<FVector2D> &UV0,
    TArray<int32> &Triangles
){
    FString output = TEXT("StorageInterfaceMeshData: ");
    output += TEXT("VertexBuffer: ");
    for (int i = 0; i < Vertecies.Num(); i++)
    {
        FVector &vertex = Vertecies[i];
        FString part = FString::Printf(TEXT("(%.2f %.2f %2f)"), vertex.X, vertex.Y, vertex.Z);
        output += part;
    }
    output += TEXT("NormalBuffer: ");
    for (int i = 0; i < Normals.Num(); i++)
    {
        FVector &normal = Normals[i];
        FString part = FString::Printf(TEXT("(%.2f %.2f %2f)"), normal.X, normal.Y, normal.Z);
        output += part;
    }

    output += TEXT("UVBuffer: ");
    for (int i = 0; i < UV0.Num(); i++)
    {
        FVector2D &uv = UV0[i];
        FString part = FString::Printf(TEXT("(%.2f %.2f)"), uv.X, uv.Y);
        output += part;
    }

    output += TEXT("triangleBuffer: ");
    for (int i = 0; i < Triangles.Num(); i++)
    {
        int32 &t = Triangles[i];
        FString part = FString::Printf(TEXT("(%d)"), t);
        output += part;
    }

    DebugHelper::logMessage(output);
}

/*
void StorageInterface::SaveVertexData(
    FString Path, 
    TArray<FVector> &Vertices
){
    // Save
    int byteSize = Vertices.Num() * sizeof(FTerrainVertex);
    FFileHelper::SaveArrayToFile(
        TArrayView<uint8>((uint8 *)Vertices.GetData(), byteSize),
        *Path
    );
}

void StorageInterface::LoadVertexData(
    FString Path,
    TArray<FVector> &Vertices
){
    // Load
    TArray<uint8> Bytes;
    FFileHelper::LoadFileToArray(Bytes, *Path);
    int sizeAsFVector = Bytes.Num() / sizeof(FTerrainVertex);
    Vertices.SetNum(sizeAsFVector); // zurück zu FVector size
    FMemory::Memcpy(Vertices.GetData(), Bytes.GetData(), Bytes.Num());
}*/