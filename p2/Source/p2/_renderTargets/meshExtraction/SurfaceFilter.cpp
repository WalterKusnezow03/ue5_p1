
#include "StaticMeshMeshExtractor.h"
#include "p2/DebugHelper.h"
#include "SurfaceFilter.h"


SurfaceFilter::SurfaceFilter(){

}

SurfaceFilter::~SurfaceFilter(){

}



void SurfaceFilter::filter(
    TArray<FVector> &vertecies,
    TArray<FVector> &normals,
    TArray<int32> &trianlges
){
    if(normals.Num() != vertecies.Num()){
        return; //NO!
    }

    //dreiecke durchlaufen,
    //wenn normale in eine ähnliche richtung, 
    //triangles aufnehmen und kopieren

    //für jeden vertex die triangles finden
    std::vector<bool> indexCovered(vertecies.Num(), false);
    std::vector<TArray<int32>> buffers;
    for (int i = 0; i < vertecies.Num(); i++){
        
        if(indexCovered[i] == false){
            TArray<int32> newBuffer;

            findVerteciesInSimilarNormalDirection(
                i,
                indexCovered,
                newBuffer,
                normals
            );

            //add new buffer
            if(newBuffer.Num() > 0){
                buffers.push_back(newBuffer);
            }
        }
    }

    DebugHelper::logMessage("SurfaceFilter buffers made normal dir", buffers.size()); //9



    //jetzt sollten alle buffer gefiltert sein
    std::vector<FColor> colors = createColorBuffer(buffers.size());
    for (int i = 0; i < buffers.size(); i++)
    {
        //buffers durchlaufen und submeshes extrahieren
        //mit neuem vertex, normal und triangle buffer

        //erstens so trennen dass nur triangles die mit einander verbunden sind
        //auch in einen getrennten buffer kommen

        TArray<TArray<int32>> subBuffers;
        findConnectedSurfaces(
            buffers[i],
            subBuffers
        );
        
        DebugHelper::logMessage("SurfaceFilter subbuffers split up", subBuffers.Num()); //(?)

        //debug draw
        draw(subBuffers, vertecies, colors[i]);

        //process subbuffers, copy according normals and vertecies, create
        //sub meshdata class, simple, just buffer.
    }
}

std::vector<FColor> SurfaceFilter::createColorBuffer(int size){
    std::vector<FColor> baseColors = {
        FColor::Red,
        FColor::Blue,
        FColor::Cyan,
        FColor::Green
    };

    std::vector<FColor> outcolors;
    while(size > 0){
        size--;
        FColor color = baseColors[size % baseColors.size()];
        outcolors.push_back(color);
    }
    return outcolors;
}

//wenn normale da gegeben, checken mit dem skalarprodukt ob ähnliche richtung
//dann zum vertex buffer hinzufügen (so sind die vertecies in richtungen gefiltert)
//dann kann im triangle buffer tatsächlich geprüft werden ob sie verbunden sind
//und zu einem surface meshdata hinzugefügrt werden
void SurfaceFilter::findVerteciesInSimilarNormalDirection(
    int index,
    std::vector<bool> &indexCovered,
    TArray<int32> &foundVertecies,
    TArray<FVector> &normals
){
    FVector &normalAtIndex = normals[index];

    //first
    indexCovered[index] = true;
    foundVertecies.Add(index);


    for (int i = 0; i < normals.Num(); i++){
        if(i != index && (indexCovered[i] == false)){
            FVector &currentNormal = normals[i];

            FVector n1 = normalAtIndex.GetSafeNormal();
            FVector n2 = currentNormal.GetSafeNormal();

            float dotProduct = FVector::DotProduct(n1, n2);
            float parallellEpsilon = 0.8f;

            if(dotProduct >= parallellEpsilon){
                indexCovered[i] = true;
                foundVertecies.Add(i);
            }
        }
    }
}


void SurfaceFilter::findConnectedSurfaces(
    TArray<int32> &inbuffer,
    TArray<TArray<int32>> &outbuffers
){
    //buffer laufen
    //und überschneidende vertecies finden

    for (int i = 2; i < inbuffer.Num(); i += 3){

        //if a triangle is found in any buffer: add to that buffer, otherwise create a new one
        int v0 = inbuffer[i - 2];
        int v1 = inbuffer[i - 1];
        int v2 = inbuffer[i];

        int foundbuffer = canFindInBuffer(v0, outbuffers);
        if(foundbuffer != -1){
            TArray<int32> &foundBufferCurrent = outbuffers[foundbuffer];
            foundBufferCurrent.Add(v0);
            foundBufferCurrent.Add(v1);
            foundBufferCurrent.Add(v2);
        }
        else
        {
            TArray<int32> newBuffer;
            newBuffer.Add(v0);
            newBuffer.Add(v1);
            newBuffer.Add(v2);
            outbuffers.Add(newBuffer);
        }
    }
}



int SurfaceFilter::canFindInBuffer(int index, TArray<TArray<int32>> &buffers){
    int bufferindex = -1;
    for (int i = 0; i < buffers.Num(); i++){
        if(canFind(index, buffers[i])){
            bufferindex = i;
            return i;
        }
    }
    return bufferindex;
}

bool SurfaceFilter::canFind(int index, TArray<int32> &buffer){
    for (int i = 0; i < buffer.Num(); i++){
        if(index == buffer[i]){
            return true;
        }
    }
    return false;
}


/**
 * 
 * 
 * --- DEBUG ---
 * 
 */
void SurfaceFilter::draw(
    TArray<TArray<int32>> &subBuffers, 
    TArray<FVector> &vertecies,
    FColor color
){
    for (int i = 0; i < subBuffers.Num(); i++){
        draw(
            subBuffers[i],
            vertecies,
            color
        );
    }
}



void SurfaceFilter::draw(
    TArray<int32> &indices,
    TArray<FVector> &vertecies,
    FColor color
){
    for (int i = 2; i < indices.Num(); i += 3){
        int v0 = indices[i - 2];
        int v1 = indices[i - 1];
        int v2 = indices[i];

        if(v0 < vertecies.Num() && v1 < vertecies.Num() && v2 < vertecies.Num()){
            DebugHelper::showLineBetween(
                worldPointer, 
                vertecies[v0], 
                vertecies[v1], 
                color, 
                100.0f
            );
            DebugHelper::showLineBetween(
                worldPointer, 
                vertecies[v0], 
                vertecies[v2], 
                color, 
                100.0f
            );
        }
    }
}






/**
 * 
 * 
 * ----- STATIC TEST ------
 * 
 * 
 */
void SurfaceFilter::Test(UWorld *world){
    UClass *objectUClass = loadUClassBluePrint(TEXT(
        "Blueprint'/Game/Prefabs/testing/weirdShapeActor.weirdShapeActor_C'"
    ));
    if(objectUClass != nullptr){
        AActor *actor = spawnDebugActor(world, objectUClass);

        DebugHelper::logMessage("SurfaceFilter loaded uclass");
        if(actor){
            DebugHelper::logMessage("SurfaceFilter spawned actor");   
        }

        
        if(actor){
            UStaticMeshComponent *mesh = findStaticMesh(actor);
            if(mesh){
                StaticMeshMeshExtractor extractor;

                TArray<FVector> vertecies;
                TArray<FVector> normals;
                TArray<int32> triangles;

                extractor.extract(
                    mesh,
                    vertecies,
                    normals,
                    triangles
                );
                DebugHelper::logMessage("SurfaceFilter extracted data vertecies", vertecies.Num());   
                DebugHelper::logMessage("SurfaceFilter extracted data normals", normals.Num());   
                DebugHelper::logMessage("SurfaceFilter extracted data triangles", triangles.Num());   

                //bis hier ok
                //auffällig: normals, vertecies, triangles all same size buffer...



                //process
                SurfaceFilter filterObject;
                filterObject.worldPointer = world;

                filterObject.filter(
                    vertecies,
                    normals,
                    triangles
                );
            }
        }
    }
}

UClass* SurfaceFilter::loadUClassBluePrint(FString path){
    // Load the class object dynamically
    UClass* bpClass = StaticLoadClass(UObject::StaticClass(), nullptr, *path);
    return bpClass;
}

UStaticMeshComponent *SurfaceFilter::findStaticMesh(AActor *actor){
    if(actor){
        if (UStaticMeshComponent* MeshComp = actor->FindComponentByClass<UStaticMeshComponent>())
        {
            return MeshComp;
        }
    }
    return nullptr;
}


AActor *SurfaceFilter::spawnDebugActor(UWorld *world, UClass *toSpawn){
    if(world != nullptr){
        FVector Location(0, 0, 400);
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
    return nullptr;
}
