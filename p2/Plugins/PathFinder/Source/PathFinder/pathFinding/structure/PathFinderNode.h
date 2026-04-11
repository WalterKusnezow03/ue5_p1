#pragma once


#include "CoreMinimal.h"

class PATHFINDER_API PathFinderNode{
public:
    static const int noneFx = -1;

    /// @brief will tell if the node is closed (on the closed list) or not
    bool closedFlag;
    /// @brief came from neighbor
    PathFinderNode *camefrom = nullptr;
    float fx;
    float gx;
    FVector pos;

    
    

    PathFinderNode(FVector posIn); //no id: setup needed
    PathFinderNode(int id, FVector posIn); //id from storage

    PathFinderNode(PathFinderNode &other);
    PathFinderNode &operator=(PathFinderNode &other);

    ~PathFinderNode();
    void reset();
    void updateCameFrom(float gxIn, float hxEnd, PathFinderNode &came);
    void close();
    bool isClosed();

    float oldfx;

    void setConvexNeighborA(PathFinderNode *n);
    void setConvexNeighborB(PathFinderNode *n);
    void addTangentialNeighbor(PathFinderNode *n);

    //convex neighbors A and B
    PathFinderNode *nA = nullptr;
    PathFinderNode *nB = nullptr;

    bool hasNeighbors(); //convex hull neighbors
    bool hasAnyNeighbors(); //any visible neighbors 

    std::vector<PathFinderNode *> visible_tangential_Neighbors;

    // new: hull index
    int hullindex = -1;
    bool sameHull(PathFinderNode *other);

    void show(UWorld *world);

    //storage interface helpers
    void setId(int id);
    int getId();

    //only returns valid ids
    TArray<int> NeighborsById();

    //returns -1 if not valid
    int IdConvexNeighborA();
    int IdConvexNeighborB();

    float Dist(const FVector &posOther);

private:
    FCriticalSection CriticalSection;

    //id system for saving
    int id = -1; //is invalid by default, not tracked in storage.
};