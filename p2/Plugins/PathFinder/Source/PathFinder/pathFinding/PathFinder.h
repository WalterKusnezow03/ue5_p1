// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <map>
#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"

#include "PathFinder/pathFinding/structure/PathFinderNode.h"
#include "PathFinder/pathFinding/structure/PathFinderChunk.h"
#include "PathFinder/pathFinding/structure/PathFinderQuadrant.h"

#include "PathFinder/pathFinding/structure/PathFinderQuadrantMap.h"

#include "PathFinder.generated.h"

class raycastTask;

/**
 * a modified a* version with tangential checks along convex polygons for minimzed graphs.
 * Nodes are found in O(1)!
 * 
 * 2 modes:
 * 
 * 1)
 * which operates on subgraphs and rechecks edges on runtime
 * the edges are build and checked on runtime because we want to operate on subgraphs
 * efficently. Also it adds supports for dynamic actors like vehicles blocking paths temporarily
 * 
 * it also eliminates all non tangential edges during runtime because they are never part 
 * of a shortest path!
 * 
 * 2)
 * A node which will prebuild all tangential edges when adding a node
 * toggle the according boolean in this header file:
 * will automatically connect nodes, reduces runtime overhead because minimal tangential graph is already build
 * 
 */

UCLASS()
class PATHFINDER_API APathFinder : public AActor
{
	GENERATED_BODY()

public:
	//---- Launch API ----
	APathFinder();
	static void makeInstance(UWorld *world, FString worldLevelName); // CALL TO LAUNCH
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float deltatime) override;

	void KillInstance();

	//---- Constants ----
	int GetChunkSize(){
		return 2000;
	}

	int GetOneMeter(){
		return 70;
	}

	/// @brief will tell whether the graph is build if nodes are added at any time
	/// (if true: added node instant connection, otherwise raycasts at runtime (expensive, bad!))
	/// ---> keep enabled! / true
	bool GetPrebuildEdgesEnabled(){
		return true;
	}

	
	





private:
	static void resetPathFinderPointer();
	static bool alreadyHasInstance();
	void Setup(FString worldname);

	static class APathFinder *pathFinderInstance;

	bool wasLoaded = false;
	FString worldLevelNameSaved;

	std::vector<FVector> prevPath;
	
	

public:
	void CollectNodePositions(
		const FVector &pos, 
		float radius,
		TArray<FVector> &outArray
	);


	static const bool debugDrawNodes = true; //false

	

	FCollisionQueryParams collsionParamsLowDetailAndFast();

	

	/// @brief returns the current pathfinder insatnce, launch throuh Public->Pathfinder module,
	/// must have a world level name!
	/// to avoid passing world argument!
	/// @return 
	static APathFinder *instance();
	

	void debugShowAllNodes(UWorld *world);

	void clear(); //clears ALL NODES

	void addNewNodeVector(std::vector<FVector> &vec, FVector &offset);
	void addNewNodeVector(std::vector<FVector> &vec);
	
protected:
	void addNewNode(FVector a);

public:
	void addConvexHull(std::vector<FVector> &vec);

	std::vector<FVector> getPath(FVector a, FVector b);

	FVector findFurthestConnectedNodeFrom(FVector &other);

	enum class PathTraceMode
	{
		SyncTrace,
		SyncTraceByTick,
		AsyncTrace
	};


	void addNode(PathFinderNode *node);
	
	void debugCountNodes();

	void draw(FVector &pos);

	bool passTangentailCheck(PathFinderNode *a, PathFinderNode *b);


	void connect(PathFinderNode *node);

private:
	

	bool reached(PathFinderNode *a, PathFinderNode *b);
	

	
	void screenMessage(int s);
	void screenMessage(FString s);

	PathFinderQuadrantMap *quadrantMap = nullptr;

	

	//A node is Found in O(1)!
	PathFinderNode *findNode(FVector pos);
	PathFinderNode *findNodeInDirection(FVector &node, FVector &dir);

	void showPos(FVector e);
	void showPos(FVector e, FColor c);

	
	

	float distance(PathFinderNode *A, PathFinderNode *B);
	float distance(FVector A, FVector B);

	

	std::vector<PathFinderNode *> getSubGraph(FVector a, FVector b);

	std::vector<FVector> findPath(
		PathFinderNode *start,
		PathFinderNode *end,
		std::vector<PathFinderNode *> &subgraph
	);

	std::vector<FVector> constructPath(
		PathFinderNode *end
	);

	bool canSeeTangential(PathFinderNode *A, PathFinderNode *B);
	bool canSee(FVector &a, FVector &b);

	bool isCloseAndTooVertical(PathFinderNode *a, PathFinderNode *b);

	int traceCount;

	/// @brief will tell whether the graph is build if nodes are added at any time
	static constexpr bool PREBUILD_EDGES_ENABLED = true;

	/// @brief will tell whether the raycast for adding nodes will be async or an synchron operation
	static constexpr bool ASYNC_EDGE_PREBUILDING = true;
	static constexpr int PREBUILD_MAXDISTANCE = 5000; // 10000 / 100 = 100 meter, keep to 50.
	
	void asyncCanSee(PathFinderNode *a, PathFinderNode *b);

	std::vector<FVector> findPath_prebuildEdges(
		PathFinderNode *start,
		PathFinderNode *end
	);

	bool isInBounds(FVector &a, FVector &b, PathFinderNode *check);


	
	FCriticalSection delegate_CriticalSection_a; 
	FCriticalSection delegate_CriticalSection_b;
public:
	FCriticalSection &GetDelegate_CriticalSectionA(){
		return delegate_CriticalSection_a;
	}
	FCriticalSection &GetDelegate_CriticalSectionB(){
		return delegate_CriticalSection_b;
	}

private:


	std::vector<FTraceDelegate *> released;
	FTraceDelegate *requestDelegate(PathFinderNode *a, PathFinderNode *b);

	void clearDelegates();

public:
	void freeDelegate(FTraceDelegate *d);

	PathTraceMode traceMode = PathTraceMode::AsyncTrace;



	void addActorToIgnoreRaycastParams(AActor *actor);
	FCollisionQueryParams getIgnoredRaycastParams();

private:
	void addNewRaytask(PathFinderNode *a, PathFinderNode *b);
	std::vector<raycastTask> rayTasksVec;


	//ignored actors
	FCollisionQueryParams collisionIgnoreParams;




	// --- STORAGE INTERFACE ---

	//map to save by id: load and save
	//static id must start from 0, -1 is a non available node as convex partner
	int staticId = 0;

	//save nodes by id to save adjacent nodes relation by id.
	std::map<int, PathFinderNode *> idMappedNodes;

	//will track the node inside the id map
	

	//will return whether a node is tracked in the id map
	bool NodeIsTrackedInIdMap(int id);

public:
	void TrackNodeInIdMap(PathFinderNode *node);

	static bool IdIsValid(int id);
	int getNextStaticId();

	//adds a node to the graph without connecting it at all
	//is only saved if id is valid
	void addNodeFromStorageInterfaceNoConnection(FVector &pos, int id);

	//adds all connections and convex neighbors to a given node by id - if id is valid
	void addConnectionsFromStorageInterfaceForNodeById(
		int id,
		TArray<int> connected,
		int convexA,
		int convexB
	);

	//returns the count of nodes which supposed to be saved
	int countNodesTrackedInIdMap();

	const std::map<int, PathFinderNode *> &IdMapReference();

	// --- STORAGE INTERFACE Section End ---


};
