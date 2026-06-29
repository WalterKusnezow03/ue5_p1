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

//FMeshedPolygonHullIndices
void FMeshedPolygonHullIndices::AddPosition(int x, int y){
    std::pair<int, int> pair(x, y);
    indices.Add(pair);
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
    //bounds check

    //edge check
    float t = FLT_MAX;
    bool wasHit = false;
    for (int i = 0; i < indices.Num(); i++){
        int j = (i + 1) % indices.Num();
        FVector2D hit;
        float hitT = FLT_MAX;
        if(RayIntersect(x,y,dir, i,j, hit, hitT)){
            if(hitT < t){ //NEAREST
                t = hitT;
                outClosestHit = FIntPoint(hit.X, hit.Y);
                wasHit = true;
            }
        }
        
    }
    outT = t;
    return wasHit;
}


bool FMeshedPolygonHullIndices::RayIntersectPolygonFarHit(
    int x, 
    int y, 
    const FVector2D &dir,
    FIntPoint &outClosestHit,
    float &outT
){
    //bounds check

    //edge check
    float t = FLT_MAX;
    bool wasHit = false;
    for (int i = 0; i < indices.Num(); i++){
        int j = (i + 1) % indices.Num();
        FVector2D hit;
        float hitT = FLT_MAX;
        if(RayIntersect(x,y,dir, i,j, hit, hitT)){
            if(hitT > t){ //FAR HIT
                t = hitT;
                outClosestHit = FIntPoint(hit.X, hit.Y);
                wasHit = true;
            }
        }
        
    }
    outT = t;
    return wasHit;
}





bool FMeshedPolygonHullIndices::RayIntersect(
    int x, 
    int y, 
    const FVector2D &dir,
    int iStartEdge,
    int jEndEdge,
    FVector2D &outHit,
    float &outT
){
    std::pair<int, int> &current = indices[iStartEdge];
    std::pair<int, int> &next = indices[jEndEdge];

    FVector2D dirNormal(-dir.Y, dir.X); //left pointing normal
    FVector2D e0(current.first, current.second);
    FVector2D e1(next.first, next.second);
    FVector2D v(x, y);

    //signs different from each other means a hit
    //since e0 is sideA and e1 Side B
    return RayIntersectAt(v,dir,e0,e1, outHit, outT);
}

bool FMeshedPolygonHullIndices::RayIntersectAt(
    const FVector2D &v, //vertex start
    const FVector2D &dir,
    const FVector2D &e0,
    const FVector2D &e1,
    FVector2D &outHit,
    float &outT
){
    /*
    
    */
    
    
    
    
    
    
    FVector2D f = e1 - e0;
    FVector2D b = dir;
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
            outHit = v + dir * t;
            return true;
        }
    }

    return false;
}


float FMeshedPolygonHullIndices::Dot(const FVector2D &a, const FVector2D &b){
    return a.X * b.X + a.Y * b.Y;
}

int32 FMeshedPolygonHullIndices::Num(){
    return indices.Num();
}

std::pair<int, int>& FMeshedPolygonHullIndices::operator[](int i){
    return indices[i];
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
