
#include "DynamicRenderedObject.h"
#include "p2/EntityManager/EntityManager.h"
#include "p2/meshgen/MeshData/MeshData.h"
#include "p2/meshgen/ELod.h"
#include "p2/gamestart/assetManager.h"
#include "DynamicRenderedObjectDebugger.h"


void DynamicRenderedObjectDebugger::createDebugObject(UWorld *world){
    if(world != nullptr){
        //hier würfel erstellen und textur laden
        //dbeug methode

        AActor *someActor = spawnDebugActor(world);
        if(someActor != nullptr){

            DynamicRenderedObject rendermaker(someActor);
            //set texture
            if(assetManager *am = assetManager::instance()){
                UTexture2D *texture = am->findTexture(textureEnum::patroneIcon);
                if(texture){
                    rendermaker.updateTexture(texture);
                }
            }


            //make material
            UMaterialInstanceDynamic *material = rendermaker.getMaterial();

            UStaticMeshComponent *mesh = findStaticMesh(someActor);
            if(mesh){
                mesh->SetMaterial(0, material);
            }
        }
    }
}

UStaticMeshComponent *DynamicRenderedObjectDebugger::findStaticMesh(AActor *actor){
    if(actor){
        if (UStaticMeshComponent* MeshComp = actor->FindComponentByClass<UStaticMeshComponent>())
        {
            return MeshComp;
        }
    }
    return nullptr;
}


AActor *DynamicRenderedObjectDebugger::spawnDebugActor(UWorld *world){
    if(world != nullptr){

        FVector Location(0, 0, 400);
        if (assetManager *am = assetManager::instance())
        {
            UClass *toSpawn = am->debugCubeBp();
            if(toSpawn){
                //check if the type to spawn is even aactor and the casting is valid
                //other wise things get messed up and different points created
                if(toSpawn->IsChildOf(AActor::StaticClass())){ 
                    //Initialize SpawnParams if needed
                    FActorSpawnParameters SpawnParams;

                    // Spawn the actor
                    AActor *spawned = world->SpawnActor<AActor>(toSpawn, Location, FRotator::ZeroRotator, SpawnParams);
                    if(spawned != nullptr){
                        return spawned;
                    }
                }
            }
        }
    }
    return nullptr;
}