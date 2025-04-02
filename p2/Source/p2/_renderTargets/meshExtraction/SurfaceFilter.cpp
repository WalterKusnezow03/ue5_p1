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
        }
    }

    //jetzt sollten alle buffer gefiltert sein
    for (int i = 0; i < buffers.size(); i++){
        //buffers durchlaufen und submeshes extrahieren
        //mit neuem vertex, normal und triangle buffer

        //erstens so trennen dass nur triangles die mit einander verbunden sind
        //auch in einen getrennten buffer kommen

        TArray<TArray<int32>> subBuffers;
        findConnectedSurfaces(
            buffers[i],
            subBuffers
        );



        //process subbuffers, copy according normals and vertecies, create
        //sub meshdata class, simple, just buffer.

    }
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



