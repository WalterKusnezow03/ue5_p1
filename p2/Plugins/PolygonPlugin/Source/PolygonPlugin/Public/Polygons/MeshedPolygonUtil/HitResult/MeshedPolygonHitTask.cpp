#include "MeshedPolygonHitTask.h"

FMeshedPolygonHitTask::FMeshedPolygonHitTask(){
    t = FLT_MAX;
    wasHit = false;
    farHit = false;
}

FMeshedPolygonHitTask::~FMeshedPolygonHitTask(){

}

FMeshedPolygonHitTask::FMeshedPolygonHitTask(float maxT){
    SetMaxT(maxT);
    wasHit = false;
    farHit = false;
}


FMeshedPolygonHitTask::FMeshedPolygonHitTask(const FVector2D &direction, float MaxT){
    wasHit = false;
    farHit = false;
    SetMaxTAndDirection(direction, MaxT);
}

void FMeshedPolygonHitTask::SetMaxT(float maxT){
    t = maxT;
}

void FMeshedPolygonHitTask::SetDirection(const FVector2D &dir){
    direction = dir;
}


void FMeshedPolygonHitTask::SetMaxTAndDirection(FVector2D dir){
    float tMax = dir.Size();
    dir = dir.GetSafeNormal(); //after size T distance max: normalize direction for hit comparison
    SetMaxTAndDirection(dir, tMax);
}
void FMeshedPolygonHitTask::SetMaxTAndDirection(const FVector2D &dir, float maxT){
    SetMaxT(maxT);
    SetDirection(dir);
}




const FVector2D &FMeshedPolygonHitTask::GetDirection() const{
    return direction;
}

FVector2D FMeshedPolygonHitTask::GetHitPoint2D(){
    return hitPoint;
}

FVector FMeshedPolygonHitTask::GetHitPoint3D(){
    return FVector(hitPoint.X, hitPoint.Y, 0.0f);
}

FIntPoint FMeshedPolygonHitTask::GetHitIntPoint(){
    return FIntPoint(hitPoint.X, hitPoint.Y);
}

float FMeshedPolygonHitTask::GetT(){
    return t;
}

bool FMeshedPolygonHitTask::WasHit(){
    return wasHit;
}

bool FMeshedPolygonHitTask::Update(float tIn, FVector2D result){
    if(tIn >= 0.0f){
        //counter for hit updates (needed for inside point test)
        numHitsUpdated++;

        bool update = false;
        //searching for closest or furthest hit
        if(farHit){
            update = tIn > t;
        }else{
            update = tIn < t;
        }
        if(update){
            t = tIn;
            hitPoint = result;
            wasHit = true;
            return true;
        }
    }
    return false;
}

void FMeshedPolygonHitTask::SetFarHit(bool flag){
    farHit = flag;
}
    bool farHit = false;

void FMeshedPolygonHitTask::UpdateHitEdgeIndex(int index){
    outHitEdgeIndex = index;
}

int FMeshedPolygonHitTask::GetHitEdgeIndex(){
    return outHitEdgeIndex;
}
    

void FMeshedPolygonHitTask::AddIgnoredEdge(int index){
    ignoredEdges.Add(index);
}

bool FMeshedPolygonHitTask::IsIgnoredEdge(int index, int next){
    return IsIgnoredEdge(index) || IsIgnoredEdge(next);
}

bool FMeshedPolygonHitTask::IsIgnoredEdge(int index){
    if(ignoredEdges.Num() > 0){
        return ignoredEdges.Contains(index);
    }
    return false;
}


int FMeshedPolygonHitTask::NumHits(){
    return numHitsUpdated;
}

bool FMeshedPolygonHitTask::NumHitsIsEven(){
    return (NumHits() % 2) == 0;
}