#pragma once

#include "CoreMinimal.h"

#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/Graph/Nodes/Edge/Edge.h"

class Intersection;

/// @brief class designed to hold the traversal result of right / left off test,
/// multiparam result.
class TERRAINPLUGIN_API IntersectionTraversalResult
{

public:
    IntersectionTraversalResult();
    ~IntersectionTraversalResult();

    IntersectionTraversalResult(const IntersectionTraversalResult &other);
    IntersectionTraversalResult &operator=(const IntersectionTraversalResult &other);

    void SetupResult(int edgeIndexIn, Edge *edgePtrIn, Intersection *resultIntersection);

    /// @brief intersection from result traversal
    Intersection *GetIntersection();

    ///@brief traversed edge
    Edge *GetEdge();

    ///@brief index in parenting intersection container array holding edges
    int GetEdgeIndex();

    ///@brief returns if the setup result params are valid
    bool ResultIsValid();

private:
    int edgeIndex = -1;
    Edge *edgePtr = nullptr;
    Intersection *intersectionPtr = nullptr;
};