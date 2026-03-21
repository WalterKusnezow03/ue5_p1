#include "AnyMeshWidgetComponentBaseSerializable.h"

#include "StoragePlugin/Storage/VertexData/StorageInterface/StorageInterfaceMeshData.h"


void UAnyMeshWidgetComponentBaseSerializable::BeginPlay(){
    Super::BeginPlay();
    LoadMeshDataFromDisk();
}

void UAnyMeshWidgetComponentBaseSerializable::EndPlay(const EEndPlayReason::Type EndPlayReason){
    Super::EndPlay(EndPlayReason);
    SaveMeshDataToDisk();
}

void UAnyMeshWidgetComponentBaseSerializable::LoadMeshDataFromDisk(){
    if(loadSerialized){
        MeshData &data = GetMeshDataRef();

        StorageInterfaceMeshData interface;

        FString nameIgnored;
        interface.LoadMeshData(
            data.getVerteciesRef(),
            data.getNormalsRef(),
            data.getUV0Ref(),
            data.getTrianglesRef(),
            makePath(), //FString path,
            innerFileName //FString &outName
        );
    }
}


void UAnyMeshWidgetComponentBaseSerializable::SaveMeshDataToDisk(){
    MeshData &data = GetMeshDataRef();
    StorageInterfaceMeshData interface;

    FString nameIgnored;
    interface.SaveMeshData(
        data.getVerteciesRef(),
        data.getNormalsRef(),
        data.getUV0Ref(),
        data.getTrianglesRef(),
        makePath(), //FString path,
        innerFileName //FString &outName
    );

}


