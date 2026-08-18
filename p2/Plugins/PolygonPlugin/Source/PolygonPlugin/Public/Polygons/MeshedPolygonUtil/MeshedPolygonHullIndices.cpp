#include "MeshedPolygonHullIndices.h"
#include "StoragePlugin/Storage/Template/TemplateBufferStorageInterface.h"



void FMeshedPolygonHullIndices::AddAll(const TArray<FIntPoint> &hits){
    for (int i = 0; i < hits.Num(); i++){
        const FIntPoint &current = hits[i];
        AddPosition(
            current.X,
            current.Y
        );
    }
}

void FMeshedPolygonHullIndices::AddAll(const TArray<FVector2D> &points){
    for (int i = 0; i < points.Num(); i++){
        AddPosition(points[i]);
    }
}

//FMeshedPolygonHullIndices
void FMeshedPolygonHullIndices::AddPosition(const FVector &pos){
    int x = FMath::FloorToInt(pos.X);
    int y = FMath::FloorToInt(pos.Y);
    AddPosition(x, y);
}



void FMeshedPolygonHullIndices::AddPosition(const FVector2D &pos){
    int x = FMath::FloorToInt(pos.X);
    int y = FMath::FloorToInt(pos.Y);
    AddPosition(x, y);
}

void FMeshedPolygonHullIndices::AddPosition(int x, int y){
    std::pair<int, int> pair(x, y);
    AddPosition(pair);
}

void FMeshedPolygonHullIndices::AddPosition(const std::pair<int,int> &pos){
    indices.Add(pos);
}

//moves all indices with a offset
void FMeshedPolygonHullIndices::Move(int x, int y){
    for (int i = 0; i < indices.Num(); i++){
        std::pair<int, int> &current = indices[i];
        current.first += x;
        current.second += y;
    }
}

bool FMeshedPolygonHullIndices::RayIntersectPolygon(
    int x, 
    int y, 
    const FVector2D &dir,
    FIntPoint &outClosestHit,
    float &outT
){
    int ignoredEdgeIndex = 0;

    FMeshedPolygonHitTask result;
    result.SetDirection(dir);
    // return RayIntersectPolygon(x, y, dir, outClosestHit, outT, ignoredEdgeIndex);

    RayIntersectPolygon(x, y, result);

    outClosestHit = result.GetHitIntPoint();
    outT = result.GetT();
    return result.WasHit();
}

bool FMeshedPolygonHullIndices::RayIntersectPolygon(
    int x, 
    int y, 
    FMeshedPolygonHitTask &result
){
    //bounds check

    //edge check
    //FMeshedPolygonHitTask result;
    float maxT = result.GetT();

    for (int i = 0; i < indices.Num(); i++){
        //only check non ignored edges
        int next = (i + 1) % indices.Num();
        if(!result.IsIgnoredEdge(i, next)){
            
            if(RayIntersect(x, y, i, next, result)){
                result.UpdateHitEdgeIndex(i);

                /*DebugHelper::logMessage(
                    FString::Printf(
                        TEXT("FMeshedPolygonHullIndices Hit %d with T %.2f < %.2f"),
                        i,
                        result.GetT(),
                        maxT
                    )
                );*/
            }
        }
        /*
        else{
            DebugHelper::logMessage("FMeshedPolygonHullIndices Is Ignored Edge ", i);
        }
        */
    }
    return result.WasHit();
}


bool FMeshedPolygonHullIndices::RayIntersectPolygonFarHit(
    int x, 
    int y, 
    const FVector2D &dir,
    FIntPoint &outClosestHit,
    float &outT
){
    //bounds check
    FMeshedPolygonHitTask result;
    result.SetFarHit(true);
    result.SetDirection(dir);

    //edge check
    float t = FLT_MAX;
    bool wasHit = false;
    for (int i = 0; i < indices.Num(); i++){
        int j = (i + 1) % indices.Num();
        FVector2D hit;
        float hitT = FLT_MAX;
        bool b = RayIntersect(x, y, i, j, result);
    }
    outClosestHit = result.GetHitIntPoint();
    outT = result.GetT();
    return result.WasHit();
}





bool FMeshedPolygonHullIndices::RayIntersect(
    int x, 
    int y, 
    int iStartEdge,
    int jEndEdge,
    FMeshedPolygonHitTask &result
){
    std::pair<int, int> &current = indices[iStartEdge];
    std::pair<int, int> &next = indices[jEndEdge];

    //FVector2D dirNormal(-dir.Y, dir.X); //left pointing normal
    FVector2D e0(current.first, current.second);
    FVector2D e1(next.first, next.second);
    FVector2D v(x, y);

    //signs different from each other means a hit
    //since e0 is sideA and e1 Side B
    return RayIntersectAt(v, e0, e1, result);
}


bool FMeshedPolygonHullIndices::RayIntersectAt(
    const FVector2D &v, //vertex start
    const FVector2D &e0,
    const FVector2D &e1,
    FMeshedPolygonHitTask &result
){
    float t = FLT_MAX;
    FVector2D hit;
    if (RayIntersectAt(v, result.GetDirection(), e0,e1, hit, t)){
        return result.Update(t, hit);
        //return true;
    }
    return false;
}

bool FMeshedPolygonHullIndices::RayIntersectAt(
    const FVector2D &v, //vertex start
    const FVector2D &dir,
    const FVector2D &e0,
    const FVector2D &e1,
    FVector2D &outHit,
    float &outT
){
    FVector2D dirN = dir.GetSafeNormal();
    FVector2D f = e1 - e0;
    FVector2D b = dirN;
    FVector2D ae = e0 - v;

    float det = -f.X * b.Y + b.X * f.Y;
    if(std::abs(det) > 0.0f){
        float s = 1.0f / det;

        FVector2D row1(b.Y, -b.X);
        FVector2D row2(f.Y, -f.X);

        float r = s * Dot(row1, ae);
        float t = s * Dot(row2, ae);
        if(r >= 0.0f && r <= 1.0f && t >= 0.0f){
            outT = t;
            outHit = v + dirN * t;
            return true;
        }
    }

    return false;
}





float FMeshedPolygonHullIndices::Dot(const FVector2D &a, const FVector2D &b){
    return a.X * b.X + a.Y * b.Y;
}

int32 FMeshedPolygonHullIndices::Num() const {
    return indices.Num();
}

std::pair<int, int>& FMeshedPolygonHullIndices::operator[](int i){
    return indices[i];
}

FVector FMeshedPolygonHullIndices::Get(int i){
    if(indices.Num() <= 0){
        return FVector(0, 0, 0);
    }
    i = std::max(0, i);
    i = i % indices.Num();

    FMeshedPolygonHullIndices &self = *this;
    const std::pair<int, int> &pair = self[i];
    return FVector(pair.first, pair.second, 0.0f);
}

void FMeshedPolygonHullIndices::AppendAsBinary(
    TArray<uint8> &buffer
){

    //so inordnung weil int kopierbar ist aus dem pair.
    TemplateBufferStorageInterface::TAppendBuffer<std::pair<int,int>>(indices, buffer);
}

void FMeshedPolygonHullIndices::LoadFromBinary(
    TArray<uint8> &buffer,
    uint8 *&Ptr // reference to a pointer. Pointer by reference.
){
    TemplateBufferStorageInterface::TLoadBuffer<std::pair<int,int>>(indices, Ptr);
}


void FMeshedPolygonHullIndices::Clear(){
    indices.Empty();
}


void FMeshedPolygonHullIndices::Triangulate(bool clockwise){
    triangulator.Triangulate(indices, clockwise);
}

//so nur für convexe polygone !
bool FMeshedPolygonHullIndices::InsideHull(int x, int y) const {
    

    //ein einfacher dot product test gegen
    //alle edges ist nicht hinreichend bei 
    //konkaven polygonen.

    //es muss ein ear clipping algorythmus verwendet werden
    return triangulator.InsideHull(x, y);
}

int FMeshedPolygonHullIndices::Dot(std::pair<int,int> &n, int x, int y) const {
    return n.first * x + n.second * y;
}


int FMeshedPolygonHullIndices::SafeIndex(int i){
    int count = Num();
    if (count > 1) {
        // Handle negative wrapping for i and j
        i = ((i % count) + count) % count;
        return i;
    }
    return 0;
}

bool FMeshedPolygonHullIndices::EdgeDirection(int i, int j, FVector2D &outDir){
    if (Num() > 1) {
        // Handle negative wrapping for i and j
        i = SafeIndex(i);
        j = SafeIndex(j);
        
        const std::pair<int, int> &posA = indices[i]; // these are indices, (i,j)
        const std::pair<int, int> &posB = indices[j]; //these are indices, (i,j)

        if(DirValid(posA, posB, outDir)){
            return true;
        }

        //outDir = Dir(posA, posB);
        //return true;
    }
    return false;
}

//from i-1 to i+1
float FMeshedPolygonHullIndices::EdgeAngleDot(int i){
    if(Num() > 1){
        FVector2D edge0;
        FVector2D edge1;
        if(EdgeDirection(i-1, i, edge0) && EdgeDirection(i, i+1, edge1)){
            float angle = FVector2D::DotProduct(edge0.GetSafeNormal(), edge1.GetSafeNormal());
            return angle;
        }
    }
    return 0.0f; //irgend ein fallback
}

bool FMeshedPolygonHullIndices::DirValid(
    const std::pair<int, int> &a, 
    const std::pair<int, int> &b,
    FVector2D &outDir
){
    FVector2D asDir;
    asDir.X = b.first - a.first; //AB = B - A
    asDir.Y = b.second - a.second;
    bool valid = asDir.Size() > 0.0f;
    if(valid){
        outDir = asDir.GetSafeNormal();
    }
    return valid;
}


FVector2D FMeshedPolygonHullIndices::Dir(const std::pair<int, int> &a, const std::pair<int, int> &b){
    FVector2D asDir;
    asDir.X = b.first - a.first; //AB = B - A
    asDir.Y = b.second - a.second;
    asDir = asDir.GetSafeNormal();
    return asDir;
}




#include "CoreMath/algorithm/GrahamScan/GrahamScan2D.h"
void FMeshedPolygonHullIndices::ConvertToConvexHull(){
    GrahamScan2D scan;
    scan.ComputeConvexHull(indices);
}







/// ----- Eject self cutting polygons -----

void FMeshedPolygonHullIndices::EjectSelfCuttingPolygons(
    TArray<FMeshedPolygonHullIndices> &outArray
){
    TArray<FMeshedPolygonHullIndices> createdForEject;
    for (int i = 1; i < indices.Num(); i++)
    {
        int prevIndex = i - 1;
        std::pair<int, int> &prev = indices[prevIndex];
        std::pair<int, int> &current = indices[i];

        int outHitEdgeIndex = -1;
        if(!IsVisble(prev, current, outHitEdgeIndex, prevIndex)){

            DebugHelper::logMessage(
                FString::Printf(
                    TEXT("FMeshedPolygonHullIndices::Eject Not Visible (%d->%d) hit %d"),
                    i - 1, i,
                    outHitEdgeIndex
                )
            );

            //insert from hit index to i
            //for eject
            FMeshedPolygonHullIndices created;
            AppendRange(created, i, outHitEdgeIndex);
            createdForEject.Add(created);
        }
    }
    //remove ejected polygons from this one
    RemoveAll(createdForEject);
    for (int i = 0; i < createdForEject.Num(); i++){
        outArray.Add(createdForEject[i]);
    }
}

void FMeshedPolygonHullIndices::AppendRange(
    FMeshedPolygonHullIndices &outPolygon, int i, int j
){
    int start = std::min(i, j);
    int end = std::max(i, j);
    for (int index = start; index < end; index++){
        if(index >= 0 && index < Num()){
            const std::pair<int, int> &current = indices[index];
            outPolygon.AddPosition(current);
        }
    }
}

bool FMeshedPolygonHullIndices::IsVisble(
    std::pair<int, int> &posA,
    std::pair<int, int> &posB,
    int &outHitEdgeIndex,
    int ignoreEdge
){
    return IsVisble(
        posA.first,
        posA.second,
        posB.first,
        posB.second,
        outHitEdgeIndex,
        ignoreEdge
    );
}



bool FMeshedPolygonHullIndices::IsVisble(
    int x,
    int y,
    int xB,
    int yB,
    int &outHitEdgeIndex,
    int ignoreEdge
){
    FVector2D dir(xB - x, yB - y);
    float tMax = dir.Size();
    dir = dir.GetSafeNormal();

    FMeshedPolygonHitTask result(tMax);
    result.SetDirection(dir);
    result.AddIgnoredEdge(ignoreEdge - 1);
    result.AddIgnoredEdge(ignoreEdge);
    result.AddIgnoredEdge((ignoreEdge + 1) % Num());

    if(RayIntersectPolygon(x, y, result)){
        outHitEdgeIndex = result.GetHitEdgeIndex();
        /*float t = result.GetT();
        if(t < tMax && t > 0.0f){
            return false;
        }*/

        return false;
    }
    return true;
}



void FMeshedPolygonHullIndices::RemoveAll(
    const TArray<FMeshedPolygonHullIndices> &array
){
    for (int i = 0; i < array.Num(); i++){
        const FMeshedPolygonHullIndices &current = array[i];
        Remove(current);
    }
}

void FMeshedPolygonHullIndices::Remove(
    const FMeshedPolygonHullIndices &removeIndices //remove by equal indices
){
    TArray<int> keepAt;

    const TArray<std::pair<int, int>> &otherIndices = removeIndices.indices;
    for(int i = 0; i < indices.Num(); i++){
        const std::pair<int, int> &current = indices[i];

        //
        bool found = false;
        for (int j = 0; j < otherIndices.Num(); j++){
            const std::pair<int, int> &otherCurrent = otherIndices[j];
            if(current == otherCurrent){
                found = true;
                break;
            }
        }
        if(!found){
            keepAt.Add(i);
        }
    }
    TArray<std::pair<int, int>> kept;
    kept.SetNum(keepAt.Num());
    for(int i = 0; i < keepAt.Num(); i++){
        kept[i] = indices[keepAt[i]];
    }
    indices = kept;
}


//area of polygon - for non intersecting, counter clockwise
float FMeshedPolygonHullIndices::Area() const {
    if(Num() < 3){
        return 0.0f;
    }

    float sum = 0.0f;
    for (int i = 0; i < Num(); i++){
        const std::pair<int, int> &a = indices[i];
        const std::pair<int, int> &b = indices[(i + 1) % Num()];

        //cross product: area of paralellogram
        float subArea = a.first * b.second - a.second * b.first;
        sum += subArea;
    }
    return sum * 0.5f;
}

bool FMeshedPolygonHullIndices::IsInside(
    const TArray<FVector2D> &array,
    FVector &notInside
){
    for (int i = 0; i < array.Num(); i++){
        const FVector2D &current = array[i];
        if(!IsInside(current)){
            notInside = FVector(current.X, current.Y, 0.0f);
            return false;
        }
    }
    return true;
}

bool FMeshedPolygonHullIndices::IsInside(const FVector2D &pos){
    int x = FMath::FloorToInt(pos.X);
    int y = FMath::FloorToInt(pos.Y);
    return IsInside(x, y);
}

bool FMeshedPolygonHullIndices::IsInside(int x, int y){
    //odd: 1,3,5... inside polygon
    //even:0,2,4,6... outside polygon
    FVector2D dir(1, 0);
    FMeshedPolygonHitTask result(dir, FLT_MAX);
    bool found = RayIntersectPolygon(x, y, result);
    return !result.NumHitsIsEven();
}


bool FMeshedPolygonHullIndices::IsVisibleHull(
    const TArray<FVector2D> &array
){
    FVector result;
    return IsVisibleHull(array, result);
}

bool FMeshedPolygonHullIndices::IsVisibleHull(const TArray<FVector2D> &array, FVector &outHit){
    for (int i = 0; i < array.Num(); i++){
        int next = (i + 1) % array.Num();
        const FVector2D &prevPos = array[i];
        const FVector2D &nextPos = array[next];
        if(!CanSee(prevPos, nextPos, outHit)){
            return false;
        }
    }
    return true;
}

bool FMeshedPolygonHullIndices::CanSee(const FVector2D &a, const FVector2D &b, FVector &outHit){
    FMeshedPolygonHitTask result;
    FVector2D dir = b - a;
    result.SetMaxTAndDirection(dir);
    float tCopy = result.GetMaxT();

    int xA = FMath::FloorToInt(a.X);
    int yA = FMath::FloorToInt(a.Y);

    bool bResult = RayIntersectPolygon(xA, yA, result);

    if(result.WasHit()){
        DebugHelper::logMessage(
            FString::Printf(TEXT("FMeshedPolygonHullIndices::Cant See Reason: %.2f < %2f"), result.GetMaxT(), tCopy)
        );
        outHit = result.GetHitPoint3D();
    }

    return !result.WasHit();
}