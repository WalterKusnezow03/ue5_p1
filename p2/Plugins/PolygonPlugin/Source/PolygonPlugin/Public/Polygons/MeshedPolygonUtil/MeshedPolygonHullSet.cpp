#include "MeshedPolygonHullSet.h"

#include "StoragePlugin/Storage/Template/TemplateBufferStorageInterface.h"

void FMeshedPolygonHullSet::AppendAsBinary(
    TArray<uint8> &buffer
){
    int32 count = set.Num();
    TemplateBufferStorageInterface::AppendCount(count, buffer);
    for (int i = 0; i < set.Num(); i++){
        set[i].AppendAsBinary(buffer);
    }
}

void FMeshedPolygonHullSet::LoadFromBinary(
    TArray<uint8> &buffer,
    uint8 *&Ptr // reference to a pointer. Pointer by reference.
){
    int32 count = -1;
    TemplateBufferStorageInterface::LoadCount(count, Ptr);
    if(count > 0){
        set.SetNum(count);
        for (int i = 0; i < set.Num(); i++){
            set[i].LoadFromBinary(buffer, Ptr);
        }
    }
    
}

void FMeshedPolygonHullSet::Add(FMeshedPolygonHullIndices &hull){
    set.Add(hull);
}

void FMeshedPolygonHullSet::AddSetWithOffset(const FMeshedPolygonHullSet &other, int offsetX, int offsetY){
    for (int i = 0; i < other.set.Num(); i++){
        FMeshedPolygonHullIndices hullCopy = other.set[i];
        hullCopy.Move(offsetX, offsetY);
        Add(hullCopy);
    }
}




FMeshedPolygonHullIndices &FMeshedPolygonHullSet::operator[](int32 i){
    if(i >= set.Num()){
        set.SetNum(i + 1);
    }
    return set[i];
}


bool FMeshedPolygonHullSet::RayIntersectPolygons(
    int x, 
    int y, 
    const FVector2D &dir,
    FIntPoint &outClosestHit
){
    float tIgnored = 0.0f;
    return RayIntersectPolygons(x, y, dir, outClosestHit, tIgnored);
}

bool FMeshedPolygonHullSet::RayIntersectPolygons(
    int x, 
    int y, 
    const FVector2D &dir,
    FIntPoint &outClosestHit,
    float &outT
){
    //DebugHelper::logMessage(FString::Printf(TEXT("FMeshedPolygonHullSet::RayIntersectPolygons (%d)"), set.Num()));

    bool found = false;
    float t = FLT_MAX;
    for (int i = 0; i < set.Num(); i++){
        float tHitCurrent = FLT_MAX;
        FIntPoint currentHitResult;
        FMeshedPolygonHullIndices &hull = set[i];
        if(hull.RayIntersectPolygon(x, y, dir, currentHitResult, tHitCurrent)){
            if(tHitCurrent < t){
                t = tHitCurrent;
                outClosestHit = currentHitResult;
                found = true;
            }
        }
    }
    if(found){
        outT = t;
    }
    return found;
}



void FMeshedPolygonHullSet::Clear(){
    set.Empty();
}


int FMeshedPolygonHullSet::NumEdges(){
    int num = 0;
    for (int i = 0; i < set.Num(); i++)
    {
        num += set[i].Num();
    }
    return num;
}