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
        //neighbors = other.neighbors;
        location = other.location;
        edgesToNeighbors = other.edgesToNeighbors;
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

            //mark here the edge reason
            //struct {intersection, edge info?}


            return true;
        }
    }
    return false;
}



void Intersection::AppendEdges(
    TArray<std::pair<FVector2D, FVector2D>> &outEdges
){
    std::pair<FVector2D, FVector2D> locationPair(location, location);
    for (int n = 0; n < edgesToNeighbors.Num(); n++)
    {
        edgesToNeighbors[n].AppendEdge(outEdges);
    }
}






void Intersection::AddAsNeighborIfAdjacent(Intersection *ptr){
    if(this == ptr){
        return;
    }

    if(ptr){
        if(!HasNeighbor(*ptr)){

            //add edge here instead of raw intersection
            TArray<Edge> edgesMade;
            TArray<Edge*> markedForRemoval;

            //crosses ALL Roads, MORE THAN ONE EDGE CAN BE ADDED BETWEEN INTERSECTIONS
            if(IsAdjacent(
                *ptr,
                edgesMade,
                markedForRemoval
            )){
                DebugHelper::logMessage(
                    FString::Printf(
                        TEXT("Intersection::Adjacent EdgesMade(%d) EdgesRemoval(%d)"),
                        edgesMade.Num(),
                        markedForRemoval.Num()
                    )
                );

                //es wurden neue kanten erstellt
                //und markiert die beidseitig gelöscht werden sollen.

                //es wurde die neue node eingefügt, die alle nachbarschafts beziehungen braucht
                //welche hier schon angefügt wurden

                //add new edges to this, flip edges, add to new.
                for(Edge &edgeMade : edgesMade){
                    //edge made is setup with other (from this, to other)
                    edgesToNeighbors.Add(edgeMade); //copied

                    edgeMade.ChangeIntersection(this);
                    ptr->edgesToNeighbors.Add(edgeMade);
                }

                //replace this with ptr for all removed nodes
                //AddNeighborToAll(markedForRemoval, edgesMade); //irgendwie unklar was hier nochmal passieren solte
                //connect


                //must be updatded to new inetrsection node?
                //recursive call? - hasneighbor would be false
                //OLD ptr->AddAllNeighbors(markedForRemoval); 

                //was wenn nicht.
                /*for(Edge *currentRemoval : markedForRemoval){
                    if(currentRemoval){
                        Intersection *copyIntersectionPtr = currentRemoval->getIntersection();
                        //newly added: add removed edge data / try connect
                        ptr->AddAsNeighborIfAdjacent(copyIntersectionPtr);
                    }
                }*/




                //dissamble bi directional
                RemoveNeighborFromAll(markedForRemoval, this);
                RemoveAll(markedForRemoval);





                //cross this and ptr
                /*AddAllNeighbors(edgesMade);
                for(Edge &ref : edgesMade){
                    ref.ChangeIntersection(this);
                }
                ptr->AddAllNeighbors(edgesMade);*/
            }

            
        }
    }
}


//new: track all results
bool Intersection::IsAdjacent(
    Intersection &other, 
    TArray<Edge> &outEdges, //new edges created
    TArray<Edge*> &outMarkedForRemoval
){
    bool found = false;
    for (TwoRoadIntersection &intersection : intersectionsInternal)
    {
        for(TwoRoadIntersection &otherIntersection : other.intersectionsInternal){
            int roadIdFound = 0;
            if(intersection.IsAdjacentSharedRoad(otherIntersection, roadIdFound)){

                DebugHelper::logMessage("Intersection::Adjacent Found Adjacent Road");
                found = true;
                Edge edge;
                edge.Setup(&other, intersection, otherIntersection);
                outEdges.Add(edge);



                
                //add nodes marked for removal, if new edge closer
                int IndexOfRoadI = intersection.IndexForRoadId(roadIdFound);
                int otherIndexAtRoad = otherIntersection.IndexForRoadId(roadIdFound);
                TArray<Edge*> remove = IntersectionsToRemove(
                    roadIdFound, // int roadId,
                    IndexOfRoadI,
                    otherIndexAtRoad
                );
                outMarkedForRemoval.Append(remove);
            }
        }
    }
    
    if(!found){
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("Intersection::Adjacent NOT Adjacent internal %d %d"), //all 0.
                intersectionsInternal.Num(),
                other.intersectionsInternal.Num()
            )
        );
    }

    return found;
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


TArray<Edge*> Intersection::IntersectionsToRemove(
    int roadId,
    int indexOwn,
    int otherIndex
){
    TArray<Edge*> outArray;
    /// Todo: switch for edge check!
    for(int i = 0; i < edgesToNeighbors.Num(); i++){
        bool added = false;
        Edge &current = edgesToNeighbors[i];
        int indexA = current.IntersectionIndexA(roadId);
        if(indexA != -1){
            if(AIsCloser(
                indexOwn,
                otherIndex,
                indexA //is further away: mark for removal
            )){
                outArray.Add(&current);
                added = true;
            }
        }
        if(!added){
            int indexB = current.IntersectionIndexB(roadId);
            if(indexB != -1){
                if(AIsCloser(
                    indexOwn,
                    otherIndex,
                    indexB //is further away: mark for removal
                )){
                    outArray.Add(&current);
                    added = true;
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
    if(a == b){
        return true;
    }

    //debug
    if(target <= a && a < b){
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("Intersection::Acloser Target(%d) < A(%d) < old(%d)"),
                target, a, b
            )
        );
        return true;
    }
    if(target >= a && a > b){
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("Intersection::Acloser Target(%d) > A(%d) > Old(%d)"),
                target, a, b
            )
        );
        return true;
    }



    DebugHelper::logMessage(
        FString::Printf(
            TEXT("Intersection::Acloser False Target(%d) A(%d) Old(%d)"),
            target, a, b
        )
    );
    return false;

    // true, wenn a zwischen target und b liegt (in beide Richtungen)
    return 
    (target < a && a < b) || 
    (target > a && a > b);
    
    
    //(a > b && b > c);
}



void Intersection::PostCleanUpAdjacency(){
    //alle edges gegen jede edge prüfen und dann löschen wenn nötig
    int i = 0;
    while(true){
        if(i >= edgesToNeighbors.Num()){
            return;
        }
        if(i >= 0){
            bool bRemove = false;
            Edge &current = edgesToNeighbors[i];
            for (int j = 0; j < edgesToNeighbors.Num(); j++)
            {
                Edge &other = edgesToNeighbors[j];
                if(current.OtherIsCloser(other)){
                    bRemove = true;
                    break;
                }
            }   

            if(bRemove){
                RemoveEdgeAtIndex(i);
                i--; //go back because of swap popback
            }
        }
        i++;
    }
}













bool Intersection::HasNeighbor(Intersection &other){
    for(int f = 0; f < edgesToNeighbors.Num(); f++){
        if(edgesToNeighbors[f].getIntersection() == &other){
            return true;
        }
    }
    return false;
}


bool Intersection::RemoveNeighbor(Edge *other){
    if(other != nullptr){
        int i = -1;//neighbors.Find(other);
        for(int f = 0; f < edgesToNeighbors.Num(); f++){
            if(edgesToNeighbors[f].IsSame(other)){
                i = f;
                break;
            }
        }
        if(i != -1){
            DebugHelper::logMessage("Intersection::Removed By Ptr ", i);
            return RemoveEdgeAtIndex(i);
        }
    }
    return false;
}

bool Intersection::RemoveEdgeAtIndex(int i){
    if(i >= 0 && i < edgesToNeighbors.Num()){
        edgesToNeighbors[i] = edgesToNeighbors.Last();
        edgesToNeighbors.Pop();
        return true;
    }
    return false;
}

void Intersection::RemoveAll(TArray<Edge*> &others){
    for (int i = 0; i < others.Num(); i++){
        RemoveNeighbor(others[i]);
    }
}

void Intersection::RemoveAll(TArray<Edge> &others){
    for (int i = 0; i < others.Num(); i++){
        RemoveNeighbor(&others[i]);
    }
}


void Intersection::AddNeighborToAll(
    TArray<Edge*> &othersmarkedForRemoval, 
    TArray<Edge> &edgesMade
){
    for (int i = 0; i < othersmarkedForRemoval.Num(); i++){
        if (Edge *current = othersmarkedForRemoval[i]){
            if(Intersection *intersectionPtr = current->getIntersection()){
                intersectionPtr->AddAllNeighbors(edgesMade);
            }
        }
    }
}





void Intersection::AddAllNeighbors(TArray<Edge> &others){
    for(Edge &ref : others){
        AddNeighbor(ref);
    }
}

void Intersection::AddAllNeighbors(TArray<Edge*> &others){
    for(Edge *ptr : others){
        if(ptr){
            AddNeighbor(*ptr);
        }
        
    }
}



void Intersection::AddNeighbor(Edge &other){
    if(!HasNeighbor(other)){
        edgesToNeighbors.Add(other);
    }
}


bool Intersection::HasNeighbor(Edge &other){
    for(Edge &ref : edgesToNeighbors){
        if(ref.IsSame(&other)){
            return true;
        }
    }
    return false;
}

void Intersection::RemoveNeighborFromAll(TArray<Edge*> &others, Intersection *neighbor){
    for (int i = 0; i < others.Num(); i++){
        if(Edge *currentEdge = others[i]){
            if(Intersection *ptr = currentEdge->getIntersection()){
                //ptr->RemoveAll(neighbor->edgesToNeighbors);
                //ptr->RemoveNeighbor(neighbor);

                ptr->RemoveAllEdgesWithNeighbor(neighbor);
            }
        }        
    }
}

void Intersection::RemoveAllEdgesWithNeighbor(Intersection *neighbor){
    if(!neighbor){
        return;
    }

    int i = 0;
    while(true){
        if(i >= edgesToNeighbors.Num()){
            return;
        }
        Edge &edgeToCheck = edgesToNeighbors[i];
        if(edgeToCheck.getIntersection() == neighbor){
            RemoveEdgeAtIndex(i);
            i--;
        }

        i++;
    }
}

//// traversal

bool Intersection::HasNeighbors(){
    return edgesToNeighbors.Num() > 0;
}

Intersection* Intersection::TraverseRightAndDisassembleEdge(
    Intersection *prev,
    Intersection *targetedEnd,
    FRoadSectionList &sectionListAppend
){


    if(prev != nullptr){
        FVector2D dir = (location - prev->location).GetSafeNormal(); // AB = B - A
        //rigth off test (-pi/2 rot to right normal)
        FVector2D normal(dir.Y, -dir.X); //does need to be normalized to find most right neighbor by angle
        normal = normal.GetSafeNormal();

        
        if(edgesToNeighbors.Num() == 0){
            return nullptr;
        }
        if(edgesToNeighbors.Num() == 1){

            Intersection *copy = edgesToNeighbors[0].getIntersection();
            IntersectionTraversalResult result;
            result.SetupResult(
                0,
                &edgesToNeighbors[0],
                copy
            );
            if(ProcessResult(result, sectionListAppend)){
                Intersection *resultIntersection = result.GetIntersection();
                return resultIntersection;
            }
        }


        //default right left test.
        IntersectionTraversalResult resultRightOffTest = FindMostRightOff(
            prev,
            targetedEnd,
            normal,
            dir//FVector &dirFromPrev
        );
        if(ProcessResult(resultRightOffTest, sectionListAppend)){
            Intersection *resultIntersection = resultRightOffTest.GetIntersection();
            return resultIntersection;
        }

        normal *= -1.0f;
        //dir *= -1.0f; //not needed, small test
        IntersectionTraversalResult resultLeftOffTest = FindMostRightOff(
            prev,
            targetedEnd,
            normal,
            dir // FVector &dirFromPrev
        );
        if(ProcessResult(resultLeftOffTest, sectionListAppend)){
            Intersection *resultIntersection = resultLeftOffTest.GetIntersection();
            return resultIntersection;
        }
    }

    //has no prev, pick random. (or maybe dir as mean from all locations...)
    if(edgesToNeighbors.Num() > 0){

        //next best for traversal
        for (int i = 0; i < edgesToNeighbors.Num(); i++){
            Edge &currentEdge = edgesToNeighbors[i];
            Intersection *nextPotential = currentEdge.getIntersection();
            if(ValidForTraversal(nextPotential)){
                AppendSharedRoadSection(&currentEdge, sectionListAppend);
                RemoveEdgeAtIndex(i);
                return nextPotential;
            }
        }
    }



    return nullptr;
}






bool Intersection::ProcessResult(
    IntersectionTraversalResult &result,
    FRoadSectionList &sectionListAppend
){
    if(result.ResultIsValid()){
    
        //IntersectionTraversalResult::
        //Intersection *GetIntersection();
        //Edge *GetEdge();
        //int GetEdgeIndex();


        //append road section
        Edge *edgeMinNext = result.GetEdge();
        AppendSharedRoadSection(edgeMinNext, sectionListAppend); // next is always build

        //remove edge to next, traversed now, immidiate!
        RemoveEdgeAtIndex(result.GetEdgeIndex());
        return true;
    }
    return false;
}

bool Intersection::IsPrev(Intersection *prev, Intersection *next){
    return prev == next;
}

bool Intersection::ValidForTraversal(
    Intersection *intersection
){
    if(intersection){
        TArray<Edge> &otherRef = intersection->edgesToNeighbors;
        int numEdges = otherRef.Num();
        if (numEdges == 1)
        {
            Intersection *pointingIntersection = otherRef[0].getIntersection();
            if(pointingIntersection == this){
                return false;
            }
        }

        return numEdges > 0; //more than one next edge for traversal can be used.
    }
    return false;
}

IntersectionTraversalResult Intersection::FindMostRightOff(
    Intersection *prev,
    Intersection *targetedEndNode,
    FVector2D &normal,
    FVector2D &dirFromPrev //to be the smallest, most anti paralell
){
    //RoadIntersection*
    float bestDotBack = 1.1f; //1.0would be fine to save here.
    float rightOffDot = -1.1f; //1.0would be fine to save here.
    Intersection *minNext = nullptr;
    Edge *edgeMinNext = nullptr;
    int indexMinNext = -1;

    for (int i = 0; i < edgesToNeighbors.Num(); i++){
        Edge &currentEdge = edgesToNeighbors[i];
        Intersection *nextPotential = currentEdge.getIntersection();
        
        
        //reached target end
        if(nextPotential){
            if(nextPotential == targetedEndNode){
                DebugHelper::logMessage("Intersection::traverse made Circle!");
                minNext = nextPotential;
                indexMinNext = i;
                edgeMinNext = &currentEdge;
                break;
            }
        }

        //best sort
        if(nextPotential && ValidForTraversal(nextPotential) && !IsPrev(nextPotential, prev)){
            


            /**
             * Wenn Alle tests in einem:
             * So nicht hinreichend, wenn keine
             * right off true ist, muss es left sein,
             * und nach einer anderen methode sortiert werden,
             * welche denn am rechtesten ist, von denen die eben links sind.
             * 
             * So NUR Right off!
             */

            
            //favoured by minimal dot to forward, and rigth off.
            FVector2D dirNext = (nextPotential->location - location).GetSafeNormal(); // AB = B - A

            float rightOffDotCurrent = Dot2D(normal, dirNext); //next dir right off test
            if(rightOffDotCurrent >= 0.0f){

                float directionNextDot = Dot2D(dirFromPrev, dirNext);
                if(directionNextDot < bestDotBack){ //to back preferred
                    bestDotBack = directionNextDot;
                    rightOffDot = rightOffDotCurrent;
                    minNext = nextPotential;
                    indexMinNext = i;
                    edgeMinNext = &currentEdge;
                }
            }
        }
    }

    IntersectionTraversalResult result;
    result.SetupResult(
        indexMinNext,
        edgeMinNext,
        minNext
    );
    return result;
}

float Intersection::Dot2D(FVector2D &a, FVector2D &b){
    return a.X * b.X + a.Y * b.Y;
}

//nicht unbedingt klar ob das so funktioniert aber sollte es.
void Intersection::AppendSharedRoadSection(
    Edge *someNeighbor,
    FRoadSectionList &sectionListAppend
){
    if(!someNeighbor){
        return;
    }

    //find shared internal section and append
    
    //Iterate overall all inernal 2 road intersections
        //if shared road 
            //get i0 i1 of shared road
    TwoRoadIntersection &currentI = someNeighbor->getConnectedA();
    TwoRoadIntersection &currentJ = someNeighbor->getConnectedB();

    int sharedRoad = -1;
    if (currentI.IsAdjacentSharedRoad(currentJ, sharedRoad))
    {
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






