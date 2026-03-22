#include "AnyMeshWidgetComponentBaseSerializable.h"

#include "StoragePlugin/Storage/VertexData/StorageInterface/StorageInterfaceMeshData.h"


void UAnyMeshWidgetComponentBaseSerializable::BeginPlay(){
    Super::BeginPlay();
}

void UAnyMeshWidgetComponentBaseSerializable::EndPlay(const EEndPlayReason::Type EndPlayReason){
    Super::EndPlay(EndPlayReason);
}

bool UAnyMeshWidgetComponentBaseSerializable::meshDataWasLoaded(){
    return wasLoaded;
}

void UAnyMeshWidgetComponentBaseSerializable::LoadMeshDataFromDisk(){
    if(loadSerialized){
        allowRender = false;

        //return;

        MeshData loaded;

        StorageInterfaceMeshData interface;

        //some thing is bricked here!
        FString nameIgnored;
        if(interface.LoadMeshData(
            loaded.getVerteciesRef(),
            loaded.getNormalsRef(),
            loaded.getUV0Ref(),
            loaded.getTrianglesRef(),
            makePath(), //FString path,
            nameIgnored //FString &outName
        )){
            if(loaded.getVerteciesRef().Num() > 0){
                loaded.RebuildBounds();
                DebugHelper::logMessage(
                    FString::Printf(
                        TEXT("UAnyMeshWidgetComponentBaseSerializable meshdata loaded: %s %d %d %d %d"),
                        *nameIgnored,
                        loaded.getVerteciesRef().Num(),
                        loaded.getNormalsRef().Num(),
                        loaded.getUV0Ref().Num(),
                        loaded.getTrianglesRef().Num()
                    )
                );
                wasLoaded = true;
                OverrideMeshData(loaded);


                if(AActor *owner = GetOwner()){
                    MMatrix transform(owner->GetActorLocation());
                    loaded.debugDrawMesh(transform, GetWorld(), FColor::Red);
                    //wasLoaded = false; //DEBUG!
                }
                


                //EINFACH MAL NICHT MACHEN!
                //OverrideMeshData(loaded);
            }
        }
        //DebugHelper::logMessage("UAnyMeshWidgetComponentBaseSerializable loaded: ", nameIgnored);

        allowRender = true;
    }
}


void UAnyMeshWidgetComponentBaseSerializable::SaveMeshDataToDisk(){
    
    if(alreadySaved)
    {
        return;
    }
    if(wasLoaded){
        return;
    }

    MeshData &data = GetMeshDataRef();
    if(data.getVerteciesRef().Num() == 0){
        return;
    }
    alreadySaved = true;




    allowRender = false;

    //MeshData &data = GetMeshDataRef();
    StorageInterfaceMeshData interface;

    MeshData copy = GetMeshDataRef();

    FString none;
    interface.SaveMeshData(
        copy.getVerteciesRef(),
        copy.getNormalsRef(),
        copy.getUV0Ref(),
        copy.getTrianglesRef(),
        makePath(),   // FString path,
        none // FString &outName
    );

    DebugHelper::logMessage(
        FString::Printf(
            TEXT("UAnyMeshWidgetComponentBaseSerializable meshdata saved: %s %d %d %d %d"),
            *none,
            copy.getVerteciesRef().Num(),
            copy.getNormalsRef().Num(),
            copy.getUV0Ref().Num(),
            copy.getTrianglesRef().Num()
        )
    );

    allowRender = true;
}


