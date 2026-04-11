#include "PathFinderNode.h"
#include "PathFinder/pathFinding/PathFinder.h"



PathFinderNode::PathFinderNode(FVector posIn){
    gx = std::numeric_limits<float>::max(); //set to max for unknown status by default
    fx = gx;
    pos = posIn;
    camefrom = nullptr;
    closedFlag = false;
    nA = nullptr;
    nB = nullptr;


    if(APathFinder *ptr = APathFinder::instance()){
        id = ptr->getNextStaticId(); // ptr->staticId++; //copy id and post increment.
        ptr->TrackNodeInIdMap(this); //track in id map for later save
    }
}
PathFinderNode::PathFinderNode(int idIn, FVector posIn){
    gx = std::numeric_limits<float>::max(); //set to max for unknown status by default
    fx = gx;
    pos = posIn;
    camefrom = nullptr;
    closedFlag = false;
    nA = nullptr;
    nB = nullptr;

    //Save Id!
    id = idIn; 
}

PathFinderNode::PathFinderNode(PathFinderNode &other){
    *this = other;
}
PathFinderNode &PathFinderNode::operator=(PathFinderNode &other){
    if(this != &other){
        gx = other.gx; //set to max for unknown status by default
        fx = other.fx;
        pos = other.pos;
        camefrom = other.camefrom;
        closedFlag = other.closedFlag;
        nA = other.nA;
        nB = other.nB;
    }
    return *this;
}

PathFinderNode::~PathFinderNode(){
    camefrom = nullptr;
    visible_tangential_Neighbors.clear();
}



/**
 * 
 * --- NODE METHODS ---
 * 
 */
void PathFinderNode::setId(int inId){
    id = inId;
}

int PathFinderNode::getId(){
    return id;
}

TArray<int> PathFinderNode::NeighborsById(){
    TArray<int> outIds;
    for (int i = 0; i < visible_tangential_Neighbors.size(); i++){
        PathFinderNode *current = visible_tangential_Neighbors[i];
        if(current){
            int currentId = current->getId();
            if(APathFinder::IdIsValid(currentId)){
                outIds.Add(currentId);
            }
        }
    }
    return outIds;
}



int PathFinderNode::IdConvexNeighborA(){
    if(nA){
        return nA->getId();
    }
    return -1;
}

int PathFinderNode::IdConvexNeighborB(){
    if(nB){
        return nB->getId();
    }
    return -1;
}




/// @brief resets the nodes: gx, fx, camefrom neighbor and the closed flag
void PathFinderNode::reset(){
    camefrom = nullptr;
    gx = std::numeric_limits<float>::max(); //is set to max for unknown status
    fx = gx;
    closedFlag = false;
}

/// @brief will update the came from node and gx, and fx value for this node
/// @param gxIn 
/// @param hxEnd 
/// @param came 
void PathFinderNode::updateCameFrom(float gxIn, float hxEnd, PathFinderNode &came){
    this->camefrom = &came;

    oldfx = fx; //copy for no reason

    gx = gxIn;
    fx = gxIn + hxEnd;
}


void PathFinderNode::close(){
    this->closedFlag = true;
}


bool PathFinderNode::isClosed(){
    return closedFlag;
}

/// @brief returns if has convex hull neighbors
/// @return 
bool PathFinderNode::hasNeighbors(){
    return nA != nullptr && nB != nullptr;
}

bool PathFinderNode::hasAnyNeighbors(){
    bool hasN = hasNeighbors();
    return hasN || visible_tangential_Neighbors.size() > 0;
}

/// @brief will set the a neighbor and also add the other convex node to tangential neighbors
/// because IT IS CONVEX!
/// @param n node 
void PathFinderNode::setConvexNeighborA(PathFinderNode *n){
    if(n != nullptr){
        nA = n;
        addTangentialNeighbor(n);
    }
}
/// @brief will set the a neighbor and also add the other convex node to tangential neighbors
/// because IT IS CONVEX!
/// @param n node 
void PathFinderNode::setConvexNeighborB(PathFinderNode *n){
    if(n != nullptr){
        nB = n;
        addTangentialNeighbor(n);
    }
}

/// @brief adds a node to the tangential connected neighbors, will allow duplicate add. Node is closed means
/// its closed. No need for searching in O(n) when adding. 
/// will add thread safely
/// @param n must not be nullptr
void PathFinderNode::addTangentialNeighbor(PathFinderNode *n){
    if(n != nullptr){
        if(n->getId() == getId()){
            return;
        }

        FScopeLock Lock(&CriticalSection); //lock because of async raycasting

        for (int i = 0; i < visible_tangential_Neighbors.size(); i++){
            PathFinderNode *current = visible_tangential_Neighbors[i];
            if(current){
                if(current->getId() == n->getId()){
                    return;
                }
            }
        }

        visible_tangential_Neighbors.push_back(n);
    }
}



void PathFinderNode::show(UWorld *world){
    if(world){
        FVector posCopy = pos;
        FVector offset = posCopy + FVector(0, 0, 10000);
        DebugHelper::showLineBetween(world, posCopy, offset, FColor::Red, 100.0f);
    }
}



float PathFinderNode::Dist(const FVector &posOther){
    return FVector::Dist(posOther, pos);
}