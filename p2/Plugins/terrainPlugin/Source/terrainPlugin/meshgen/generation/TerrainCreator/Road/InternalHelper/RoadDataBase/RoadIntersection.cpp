#include "RoadIntersection.h"
#include "GameCore/DebugHelper.h"

RoadIntersection::RoadIntersection(){

}

RoadIntersection::~RoadIntersection(){

}

RoadIntersection::RoadIntersection(const RoadIntersection &other){
    if(this != &other){
        *this = other;
    }
}

RoadIntersection &RoadIntersection::operator=(const RoadIntersection &other){
    if(this != &other){
        roadIdA = other.roadIdA;
        roadIdB = other.roadIdB;
        indexPairOfIntersection = other.indexPairOfIntersection;
        location = other.location;
    }
    return *this;
}

FString RoadIntersection::ToString(){
    FString msg = FString::Printf(
        TEXT("roadIdA(%d) roadIdB(%d) indexPairOfIntersection(%d,%d)"),
        roadIdA,
        roadIdB,
        indexPairOfIntersection.first,
        indexPairOfIntersection.second
    );
    return msg;
}

void RoadIntersection::Setup(
    int roadIdAIn, 
    int roadIdBIn, 
    const std::pair<int, int> &indexPair,
    FVector2D &locationIn
){
    roadIdA = roadIdAIn;
    roadIdB = roadIdBIn;
    indexPairOfIntersection = indexPair;
    location = locationIn;
}

bool RoadIntersection::IsSameByDistance(RoadIntersection &other, int maxDistance){
    if(IsAdjacent(other)){
        int maxDistSquared = maxDistance * maxDistance;

        //dist squared
        FVector2D connect = other.location - location; //AB = B - A
        return connect.X * connect.X + connect.Y * connect.Y <= maxDistSquared;
    }
    return false;
}

int RoadIntersection::RoadIdA(){
    return roadIdA;
}

int RoadIntersection::RoadIdB(){
    return roadIdB;
}

int RoadIntersection::indexA(){
    return IndexForRoadId(RoadIdA());
}

int RoadIntersection::indexB(){
    return IndexForRoadId(RoadIdB());
}

int RoadIntersection::IndexForRoadId(int someRoadId){
    if(someRoadId == roadIdA){
        return indexPairOfIntersection.first;
    }
    if(someRoadId == roadIdB){
        return indexPairOfIntersection.second;
    }
    return -1;
}




/// --- experimental push in before a node and re-adjust link---
void RoadIntersection::ConnectBiDirectionalIfIsAdjacentAndRelinkNodes(RoadIntersection &other){
    if(HasNeighbor(other)){
        return;
    }

    if(IsAdjacent(other)){
        RoadIntersection *laterNode = nullptr;
        if (HasLaterNode(other, laterNode))
        {
            if(laterNode){
                laterNode->RemoveNeighbor(*this);
                this->RemoveNeighbor(*laterNode);

                laterNode->AddNeighbor(other);
                this->AddNeighbor(other);
                other.AddNeighbor(*this);
            }
        }
        else
        {
            AddNeighbor(other);
            other.AddNeighbor(*this);
        }
    }
}

bool RoadIntersection::HasLaterNode(
    RoadIntersection &other,
    RoadIntersection *&outLaterNode
){
    int sharedRoadId = -1;
    bool resultFound = false;
    if (IsAdjacentSharedRoad(other, sharedRoadId))
    {
        int splineIndex = other.IndexForRoadId(sharedRoadId);
        
        //new
        int ownIndex = IndexForRoadId(sharedRoadId);
        int closestIndex = splineIndex;
        
        //forAll Neighbors
            //if shared road
                //if index closer: relink

        for(int i = 0; i < adjacentNeighbors.Num(); i++){
            RoadIntersection *currentNeighbor = adjacentNeighbors[i];
            int sharedRoadCurrent = -1;
            if(currentNeighbor->IsAdjacentSharedRoad(other, sharedRoadCurrent)){
                if(sharedRoadCurrent == sharedRoadId){
                    int currentSplineIndex = currentNeighbor->IndexForRoadId(sharedRoadCurrent);
                    if(ACloserThanB(ownIndex, splineIndex, currentSplineIndex)){
                        outLaterNode = currentNeighbor;

                        DebugHelper::logMessage(
                            FString::Printf(
                                TEXT("RoadIntersection::Found Farer Node own(%d) spline(%d) kicked(%d)"),
                                ownIndex,
                                splineIndex,
                                currentSplineIndex
                            )
                        );
                        return true;
                    }
                }
            }
        }

    }

    return resultFound;
    // outLaterNode != nullptr;
    //  return false;
}

bool RoadIntersection::ACloserThanB(
    int target,
    int A,
    int B
){
    int deltaA = A - target; // AB = B - A
    int deltaB = B - target;
    return std::abs(deltaA) <= std::abs(deltaB);
}





void RoadIntersection::ConnectBiDirectionalIfIsAdjacent(RoadIntersection &other){
    if(AddAsNeighborIfIsAdjacent(other)){
        other.AddNeighbor(*this);
    }
}


bool RoadIntersection::AddAsNeighborIfIsAdjacent(RoadIntersection &other){
    if(HasNeighbor(other)){
        return false;
    }

    if(IsAdjacent(other)){
        return AddNeighbor(other);
    }
    return false;
}

bool RoadIntersection::IsAdjacent(RoadIntersection &other){
    int ignored = -1;
    return IsAdjacentSharedRoad(other, ignored);
}

bool RoadIntersection::IsAdjacentSharedRoad(RoadIntersection &other, int &outId){
    TArray<int> idsThis = {RoadIdA(), RoadIdB()};
    TArray<int> idsOther = {other.RoadIdA(), other.RoadIdB()};
    for (int i = 0; i < idsThis.Num(); i++){
        int idCurrent = idsThis[i];
        for (int j = 0; j < idsOther.Num(); j++){
            if(idCurrent == idsOther[j]){
                outId = idCurrent;
                return true;
            }
        }
    }
    return false;
}


bool RoadIntersection::IsAdjacentBothSharedRoad(RoadIntersection &other){
    return 
    (RoadIdA() == other.RoadIdA() && RoadIdB() == other.RoadIdB()) ||
    (RoadIdA() == other.RoadIdB() && RoadIdB() == other.RoadIdA());
}




bool RoadIntersection::HasNeighbor(RoadIntersection &other){
    return adjacentNeighbors.Contains(&other);
}

bool RoadIntersection::AddNeighbor(RoadIntersection &other){
    if(HasNeighbor(other)){
        return false;
    }
    RoadIntersection *ptr = &other;
    adjacentNeighbors.Add(ptr);
    return true;
}


void RoadIntersection::RemoveNeighbor(RoadIntersection &other){
    int32 Index = adjacentNeighbors.Find(&other);
    if (Index != INDEX_NONE)
    {
        adjacentNeighbors[Index] = adjacentNeighbors.Last();
        adjacentNeighbors.Pop();
    }
}






//// ---- edges / traversal ----

void RoadIntersection::AppendEdges(TArray<std::pair<FVector2D, FVector2D>> &edges){
    bTraversed = true;

    std::pair<FVector2D, FVector2D> pairEdge;
    pairEdge.first = location;
    for (int i = 0; i < adjacentNeighbors.Num(); i++)
    {
        if(RoadIntersection *current = adjacentNeighbors[i]){
            if(!current->IsTraversed()){
                pairEdge.second = current->location;
                edges.Add(pairEdge);
            }
        }
    }
}


bool RoadIntersection::IsTraversed(){
    return bTraversed;
}






////// ---- TRAVERSAL DISSASSEMBLY SECTION ----


bool RoadIntersection::HasNeighbors(){
    return adjacentNeighbors.Num() > 0;
}

//// -- edge traverse: dissambly of connection one sided, right off CLW traversal by best chance--
RoadIntersection* RoadIntersection::TraverseRightAndDisassembleEdge(
    RoadIntersection *prev,
    TArray<RoadIntersection *> &traversed
){
    if(prev != nullptr){
        FVector2D dir = (location - prev->location).GetSafeNormal(); // AB = B - A
        //rigth off test (-pi/2 rot to right normal)
        FVector2D normal(dir.Y, -dir.X); //does need to be normalized to find most right neighbor by angle

        prev->RemoveNeighbor(*this); //forward remove only
        if(adjacentNeighbors.Num() == 0){
            return nullptr;
        }
        if(adjacentNeighbors.Num() == 1){
            return adjacentNeighbors[0];
        }

        //RoadIntersection*
        float minDot = 1.1f; //1.0would be fine to save here.
        float rightOffDot = -1.1f; //1.0would be fine to save here.
        RoadIntersection *minNext = nullptr;

        for (int i = 0; i < adjacentNeighbors.Num(); i++){
            RoadIntersection *nextPotential = adjacentNeighbors[i];
            if(
                ValidForTraversal(prev, traversed, nextPotential)
                //nextPotential != nullptr && 
                //nextPotential != prev
            ){
                
                //favoured by minimal dot to forward, and rigth off.
                FVector2D dirNext = (nextPotential->location - location).GetSafeNormal(); // AB = B - A

                float rightOffDotCurrent = dir.X * normal.X + dir.Y + normal.Y; //right off prev dir normal
                float directionNextDot = dir.X * dirNext.X + dir.Y + dirNext.Y;

                if(rightOffDotCurrent >= rightOffDot){
                    if(directionNextDot <= minDot){ //to back preferred
                        minDot = directionNextDot;
                        rightOffDot = rightOffDotCurrent;
                        minNext = nextPotential;
                    }
                }
            }
        }
        //most right next node.
        return minNext;
    }
    if(adjacentNeighbors.Num() > 0){
        return adjacentNeighbors[0];
    }

    return nullptr;
}


bool RoadIntersection::ValidForTraversal(
    RoadIntersection *prev,
    TArray<RoadIntersection *> &traversed,
    RoadIntersection *compare
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