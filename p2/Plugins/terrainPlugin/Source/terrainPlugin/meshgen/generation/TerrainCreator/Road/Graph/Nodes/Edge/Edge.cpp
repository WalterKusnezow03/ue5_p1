#include "Edge.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/Graph/Nodes/Intersection.h"

Edge::Edge(){

}

Edge::~Edge(){

}

Edge::Edge(const Edge &other){
    if(this != &other){
        *this = other;
    }
}

Edge &Edge::operator=(const Edge &other){
    if(this != &other){
        connectedNode = other.connectedNode;
        connectedA = other.connectedA;
        connectedB = other.connectedB;
    }
    return *this;
}

void Edge::Setup(Intersection *node, TwoRoadIntersection &a, TwoRoadIntersection &b){
    connectedNode = node;
    connectedA = a;
    connectedB = b;
}

Intersection *Edge::getIntersection(){
    return connectedNode;
}

bool Edge::IsSame(Edge *other){
    if(other){
        return 
        connectedA == other->connectedA && 
        connectedB == other->connectedB && 
        connectedNode == other->connectedNode;
    }
    return false;
}

int Edge::IntersectionIndexA(int roadId){
    return connectedA.IndexForRoadId(roadId);
}
int Edge::IntersectionIndexB(int roadId){
    return connectedB.IndexForRoadId(roadId);
}

void Edge::ChangeIntersection(Intersection *node){
    connectedNode = node;
}

void Edge::AppendEdge(
    TArray<std::pair<FVector2D, FVector2D>> &outEdges
){
    std::pair<FVector2D, FVector2D> newPair(
        connectedA.getLocation(),
        connectedB.getLocation()
    );
    outEdges.Add(newPair);
}





std::pair<int, int> Edge::IndexPairConnectedABForRoadId(int roadId){
    int a1 = connectedA.IndexForRoadId(roadId);
    int b1 = connectedB.IndexForRoadId(roadId);
    std::pair<int, int> outpair;
    outpair.first = std::min(a1, b1);
    outpair.first = std::max(a1, b1);
    return outpair;
}

//might not be true altogether...
bool Edge::OtherIsCloser(Edge &other){
    if(other.connectedNode == connectedNode){
        //points to same connecting node: check road id same
        int sharedRoadId = -1;
        if (SharedRoad(other, sharedRoadId)){
            //check index closer same side
            std::pair<int, int> pair1 = IndexPairConnectedABForRoadId(sharedRoadId);
            std::pair<int, int> pair2 = other.IndexPairConnectedABForRoadId(sharedRoadId);

            if(isBetween(pair1.first, pair1.second, pair2.first, pair2.second)){
                return true;
            }
            if(isBetween(pair2.first, pair2.second, pair1.first, pair1.second)){
                return true;
            }
        }
    }
    return false;
}

bool Edge::isBetween(int lower, int higher, int lowerOther, int higherOther){
    //lower lowerOther higherOther higher
    if(lower <= lowerOther && higherOther < higherOther){
        return true;
    }

    return false;
}

bool Edge::SharedRoad(Edge &other, int &outRoadId){
    int outId = -1;
    if(connectedA.IsAdjacentSharedRoad(other.connectedA, outId)){
        outRoadId = outId;
        return true;
    }
    if(connectedB.IsAdjacentSharedRoad(other.connectedA, outId)){
        outRoadId = outId;
        return true;
    }

    if(connectedA.IsAdjacentSharedRoad(other.connectedB, outId)){
        outRoadId = outId;
        return true;
    }
    if(connectedB.IsAdjacentSharedRoad(other.connectedB, outId)){
        outRoadId = outId;
        return true;
    }
    return false;
}