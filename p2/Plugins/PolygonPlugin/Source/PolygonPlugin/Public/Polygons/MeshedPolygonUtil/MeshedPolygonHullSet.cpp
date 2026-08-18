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

void FMeshedPolygonHullSet::AddAll(TArray<FMeshedPolygonHullIndices> &hulls){
    for (int i = 0; i < hulls.Num(); i++){
        Add(hulls[i]);
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




bool FMeshedPolygonHullSet::IsVisibleHull(
    const TArray<FVector2D> &array
){
    if(array.Num() > 1){
        for (int i = 0; i < array.Num(); i++)
        {
            const FVector2D &current = array[i];
            const FVector2D &next = array[(i + 1) % array.Num()];
            if(!IsVisible(current, next)){
                return false;
            }
        }
    }
    return true;
}


bool FMeshedPolygonHullSet::IsVisible(const FVector2D &a, const FVector2D &b){
    int xA = FMath::FloorToInt(a.X);
    int yA = FMath::FloorToInt(a.Y);

    int xB = FMath::FloorToInt(b.X);
    int yB = FMath::FloorToInt(b.Y);
    return IsVisble(xA, yA, xB, yB);
}



bool FMeshedPolygonHullSet::IsVisble(
    int x,
    int y,
    int xB,
    int yB
){
    FVector2D dir(xB - x, yB - y);
    float tMax = dir.Size();
    float t = tMax;
    dir = dir.GetSafeNormal();

    FIntPoint outClosestHit;
    if (RayIntersectPolygons(x, y, dir, outClosestHit, t)){
        if(t < tMax){
            return false;
        }
    }
    return true;
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

    bool found = false;
    //float t = FLT_MAX;
    outT = FLT_MAX; //must be set to Float Max to work properly
    for (int i = 0; i < set.Num(); i++){
        //FIntPoint currentHitResult;
        if(RayIntersectPolygonUpdateT(x, y, dir, outClosestHit, outT, i)){
            found = true;
        }
    }
    return found;
}


bool FMeshedPolygonHullSet::RayIntersectPolygonUpdateT(
    int x, 
    int y, 
    const FVector2D &dir,
    FIntPoint &outClosestHit,
    float &outT,
    int layer
){
    if(layer >= 0 && layer < set.Num()){
        float tHitCurrent = FLT_MAX;
        FIntPoint currentHitResult;
        FMeshedPolygonHullIndices &hull = set[layer];
        if(hull.RayIntersectPolygon(x, y, dir, currentHitResult, tHitCurrent)){
            if(tHitCurrent < outT){
                outT = tHitCurrent;
                outClosestHit = currentHitResult;
                //DebugHelper::logMessage(FString::Printf(TEXT("FMeshedPolygonHullSet::ray update hit %.2f"), outT));
                return true;
            }
        }
    }
    return false;
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



/// ----- Eject self cutting polygons on outer hull-----
/// -- ear clipping like algorythm to cut loops --
void FMeshedPolygonHullSet::SplitSelfCuttingPolygonsFromOuterHull(){
    if(set.Num() > 0){
        SplitSelfCuttingPolygonsFromHullAt(0);
    }
}

void FMeshedPolygonHullSet::SplitSelfCuttingPolygonsFromHullAt(int index){
    if(index >= 0 && index < set.Num()){
        FMeshedPolygonHullIndices &toSplit = set[index];

        int num = toSplit.Num();

        //eject polygons
        TArray<FMeshedPolygonHullIndices> splitOff;
        toSplit.EjectSelfCuttingPolygons(splitOff);


        DebugHelper::logMessage(
            FString::Printf(
                TEXT("FMeshedPolygonHullSet::SplitSelfCuttingPolygonsFromHullAt index %d num %d ejected %d left over %d"),
                index,
                num,
                splitOff.Num(),
                toSplit.Num()
            )
        );

        //add to set
        AddAll(splitOff);
    }
}

void FMeshedPolygonHullSet::SortByArea() {
    set.Sort([](const FMeshedPolygonHullIndices& A, const FMeshedPolygonHullIndices& B) {
        return A.Area() > B.Area();
    });
}