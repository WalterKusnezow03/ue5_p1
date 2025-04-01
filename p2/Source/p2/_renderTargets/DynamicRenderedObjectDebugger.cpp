
#include "DynamicRenderedObject.h"
#include "DynamicRenderedObjectDebugger.h"


void DynamicRenderedObjectDebugger::createDebugObject(UWorld *world){
    if(world != nullptr){
        //hier würfel erstellen und textur laden
        //dbeug methode

        AActor *someActor = nullptr;
        if(someActor != nullptr){

            DynamicRenderedObject rendermaker(someActor);
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