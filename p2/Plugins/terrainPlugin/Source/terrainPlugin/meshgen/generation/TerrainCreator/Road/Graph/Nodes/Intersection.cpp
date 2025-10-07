#include "Intersection.h"

Intersection::Intersection(){

}
Intersection::~Intersection(){

}

Intersection::Intersection(const Intersection &other){
    if(this != &other){
        *this = other;
    }
}

Intersection &Intersection::operator=(const Intersection &other){
    if(this != &other){
        intersectionsInternal = other.intersectionsInternal;
        neighbors = other.neighbors;
        location = other.location;
    }
    return *this;
}

Intersection::Intersection(const TwoRoadIntersection &other){
    intersectionsInternal.Add(other);
    location = other.getLocation();
}



bool Intersection::Join(TwoRoadIntersection &other){
    //if(other.IsValidIntersection()) prevent same road id same index

    if(!HasSame(other) && CanJoin(other)){
        intersectionsInternal.Add(other);
        location = other.getLocation();
        return true;
    }
    return false;
}

bool Intersection::HasSame(TwoRoadIntersection &compare){
    //same if same road ids and same index
    for(TwoRoadIntersection &other : intersectionsInternal){
        if(other.IsSame(compare)){
            return true;
        }
    }
    return false;
}



bool Intersection::CanJoin(TwoRoadIntersection &compare){
    for(TwoRoadIntersection &other : intersectionsInternal){
        if(other.SharesSameRoadAndIndex(compare)){
            return true;
        }
    }
    return false;
}



void Intersection::AppendEdges(
    TArray<std::pair<FVector2D, FVector2D>> &outEdges
){
    std::pair<FVector2D, FVector2D> locationPair(location, location);
    for (int n = 0; n < neighbors.Num(); n++)
    {
        if(Intersection *current = neighbors[n]){
            locationPair.second = current->location;
            outEdges.Add(locationPair);
        }
    }
}






void Intersection::AddAsNeighborIfAdjacent(Intersection *ptr){
    if(this == ptr){
        return;
    }

    if(ptr){
        if(!HasNeighbor(ptr)){
            int roadId = -1;
            if(IsAdjacent(*ptr, roadId)){
                int ownIndexAtRoad = IntersectionIndexFor(roadId);
                int otherIndexAtRoad = ptr->IntersectionIndexFor(roadId);

                //check all neighbors for removal, since new intersection is closer
                //on the same road, double added before hand should not happen.
                TArray<Intersection *> markedForRemoval = IntersectionsToRemove(
                    roadId, ownIndexAtRoad, otherIndexAtRoad
                );
                RemoveAll(markedForRemoval);
                RemoveNeighborFromAll(markedForRemoval, this);


                //replace this with ptr for all removed nodes
                AddNeighborToAll(markedForRemoval, ptr);
                

                //connect
                ptr->AddAllNeighbors(markedForRemoval);
                
                //cross this and ptr
                AddNeighbor(ptr);
                ptr->AddNeighbor(this);
            }
        }
    }
}


//first result where road id matches for intersection which shares a road
bool Intersection::IsAdjacent(Intersection &other, int &outRoadId){
    for(TwoRoadIntersection &intersection : intersectionsInternal){
        for(TwoRoadIntersection &otherIntersection : other.intersectionsInternal){
            int id = 0;
            if(intersection.IsAdjacentSharedRoad(otherIntersection, id)){
                outRoadId = id;
                return true;
            }
        }
    }
    return false;
}

//first result where road id matches, returns index of bspline
int Intersection::IntersectionIndexFor(int roadId){
    for(TwoRoadIntersection &intersection : intersectionsInternal){
        int indexFound = intersection.IndexForRoadId(roadId);
        if(indexFound != -1){
            return indexFound;
        }
    }
    return -1;
}


TArray<Intersection*> Intersection::IntersectionsToRemove(
    int roadId,
    int indexOwn,
    int otherIndex
){
    TArray<Intersection*> outArray;
    if(roadId != -1){
        for(Intersection *ptr : neighbors){
            int iInSplineFound = ptr->IntersectionIndexFor(roadId);
            if(iInSplineFound != -1){
                if(AIsCloser(
                    indexOwn,
                    otherIndex,
                    iInSplineFound //is further away: mark for removal
                )){
                    outArray.Add(ptr);
                }
            }
        }
    }
    return outArray;
}

bool Intersection::AIsCloser(int target, int a, int b){
    //int deltaA = a - target; //AB = B - A
    //int deltaB = b - target;
    //return std::abs(deltaA) <= std::abs(deltaB);
    
    //debug
    if(target < a && a < b){
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("Intersection::Acloser Target(%d) < A(%d) < old(%d)"),
                target, a, b
            )
        );
    }
    if(target > a && a > b){
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("Intersection::Acloser Target(%d) > A(%d) > Old(%d)"),
                target, a, b
            )
        );
    }


    
    // true, wenn a zwischen target und b liegt (in beide Richtungen)
    return 
    (target < a && a < b) || 
    (target > a && a > b);
    
    
    //(a > b && b > c);
}




bool Intersection::HasNeighbor(Intersection *other){
    if(other){
        for(Intersection *current : neighbors){
            if(current == other){
                return true;
            }
        }
        return false;
    }
    return true;
}

void Intersection::AddNeighbor(Intersection *other){
    if(other != nullptr){
        if(neighbors.Contains(other) == false){
            neighbors.Add(other);
        }
    }
}

void Intersection::RemoveNeighbor(Intersection *other){
    if(other != nullptr){
        int i = neighbors.Find(other);
        if(i != INDEX_NONE){
            neighbors[i] = neighbors.Last();
            neighbors.Pop();
        }
    }
}

void Intersection::RemoveAndAddNeighbor(Intersection *remove, Intersection *add){
    RemoveNeighbor(remove);
    AddNeighbor(add);
}


void Intersection::RemoveAll(TArray<Intersection*> &others){
    for (int i = 0; i < others.Num(); i++){
        RemoveNeighbor(others[i]);
    }
}


void Intersection::AddAllNeighbors(TArray<Intersection*> &others){
    for (int i = 0; i < others.Num(); i++){
        if(Intersection *ptr = others[i]){
            AddNeighbor(ptr);
        }
    }
}

void Intersection::AddNeighborToAll(TArray<Intersection*> &others, Intersection *neighbor){
    for (int i = 0; i < others.Num(); i++){
        if(Intersection *ptr = others[i]){
            ptr->AddNeighbor(neighbor);
        }
    }
}

void Intersection::RemoveNeighborFromAll(TArray<Intersection*> &others, Intersection *neighbor){
    for (int i = 0; i < others.Num(); i++){
        if(Intersection *ptr = others[i]){
            ptr->RemoveNeighbor(neighbor);
        }
    }
}






//// traversal

bool Intersection::HasNeighbors(){
    return neighbors.Num() > 0;
}

//// -- edge traverse: dissambly of connection one sided, right off CLW traversal by best chance--
Intersection* Intersection::TraverseRightAndDisassembleEdge(
    Intersection *prev,
    TArray<Intersection *> &traversed,
    FRoadSectionList &sectionListAppend
){
    if(prev != nullptr){
        FVector2D dir = (location - prev->location).GetSafeNormal(); // AB = B - A
        //rigth off test (-pi/2 rot to right normal)
        FVector2D normal(dir.Y, -dir.X); //does need to be normalized to find most right neighbor by angle

        prev->RemoveNeighbor(this); //forward remove only
        if(neighbors.Num() == 0){
            return nullptr;
        }
        if(neighbors.Num() == 1){
            return neighbors[0];
        }

        //RoadIntersection*
        float minDot = 1.1f; //1.0would be fine to save here.
        float rightOffDot = -1.1f; //1.0would be fine to save here.
        Intersection *minNext = nullptr;

        for (int i = 0; i < neighbors.Num(); i++){
            Intersection *nextPotential = neighbors[i];
            if(
                ValidForTraversal(prev, traversed, nextPotential)
                //nextPotential != nullptr && 
                //nextPotential != prev
            ){
                
                //favoured by minimal dot to forward, and rigth off.
                FVector2D dirNext = (nextPotential->location - location).GetSafeNormal(); // AB = B - A

                float rightOffDotCurrent = Dot2D(dir, normal);
                //dir.X *normal.X + dir.Y *normal.Y; // right off prev dir normal
                float directionNextDot = Dot2D(dir, dirNext);
                //dir.X * dirNext.X + dir.Y * dirNext.Y;

                if(rightOffDotCurrent >= rightOffDot){
                    if(directionNextDot <= minDot){ //to back preferred
                        minDot = directionNextDot;
                        rightOffDot = rightOffDotCurrent;
                        minNext = nextPotential;
                    }
                }
            }
        }

        //append road section
        AppendSharedRoadSection(minNext, sectionListAppend);

        //most right next node.
        return minNext;
    }
    if(neighbors.Num() > 0){
        AppendSharedRoadSection(neighbors[0], sectionListAppend);
        return neighbors[0];
    }

    return nullptr;
}

float Intersection::Dot2D(FVector2D &a, FVector2D &b){
    return a.X * b.X + a.Y * b.Y;
}

//nicht unbedingt klar ob das so funktioniert aber sollte es.
void Intersection::AppendSharedRoadSection(
    Intersection *someNeighbor,
    FRoadSectionList &sectionListAppend
){
    if(!someNeighbor){
        return;
    }

    //find shared internal section and append
    
    //Iterate overall all inernal 2 road intersections
        //if shared road 
            //get i0 i1 of shared road
    for (int i = 0; i < intersectionsInternal.Num(); i++){
        TwoRoadIntersection &currentI = intersectionsInternal[i];
        for (int j = 0; j < someNeighbor->intersectionsInternal.Num(); j++){
            TwoRoadIntersection &currentJ = someNeighbor->intersectionsInternal[j];

            int sharedRoad = 0;
            // same road
            if(currentI.IsAdjacentSharedRoad(currentJ, sharedRoad)){
                FRoadSection sectionNew;


                sectionNew.Setup(
                    sharedRoad, 
                    currentI.IndexForRoadId(sharedRoad), //might be wrong order. Re order needed in sectionlist.
                    currentJ.IndexForRoadId(sharedRoad)
                );
                //sectionListAppend.Add(sectionNew);
                sectionListAppend.AddOrdered(sectionNew);

            }
        }
    }
}





bool Intersection::ValidForTraversal(
    Intersection *prev,
    TArray<Intersection *> &traversed,
    Intersection *compare
){
    if(prev){
        if(prev == compare){
            //return false; //instant return
            return traversed.Num() > 1; //at least 1 without prev added ?
        }
    }
    if(compare){
        if(!traversed.Contains(compare)){
            return true;
        }
    }
    return false;
}