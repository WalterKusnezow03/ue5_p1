// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFinder.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include <cmath>
#include <algorithm>
#include "CoreMinimal.h"
#include <limits>
#include "DebugPlugin/DebugHelper.h"

#include "PathFinder/pathFinding/priorityQueue.h"
#include "PathFinder/pathFinding/raycastTasks/raycastTask.h"
#include "PathFinder/storageInterface/PathFinderStorageInterface.h"

#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"

#include "EdgeCollector.h"

//---- STATIC VARS ----
APathFinder* APathFinder::pathFinderInstance = nullptr; //very imporntant, do not delete!


void APathFinder::resetPathFinderPointer(){
    APathFinder::pathFinderInstance = nullptr;
}

bool APathFinder::alreadyHasInstance(){
    return APathFinder::pathFinderInstance != nullptr;
}

bool APathFinder::IdIsValid(int id){
    return id >= 0;
}

//---- Launch API ----
APathFinder::APathFinder(){
    PrimaryActorTick.bCanEverTick = true;
}

void APathFinder::makeInstance(UWorld *world, FString worldLevelName){
    if (world != nullptr)
    {
        UClass *toSpawn = APathFinder::StaticClass();
        if(toSpawn){
            FActorSpawnParameters SpawnParams;
            FVector Location;
            AActor *spawned = world->SpawnActor<AActor>(toSpawn, Location, FRotator::ZeroRotator, SpawnParams);   
            if(spawned){
                APathFinder *casted = Cast<APathFinder>(spawned);
                if(casted){
                    APathFinder::pathFinderInstance = casted;
                    casted->Setup(worldLevelName);
                }
            }
        }
    }
}

void APathFinder::Setup(FString worldname){
    worldLevelNameSaved = worldname;

    quadrantMap = new PathFinderQuadrantMap(this);
    

    DebugHelper::logMessage("PATHFINDER LAUNCHED");

    PathFinderStorageInterface interface;
    if(interface.Load(worldLevelNameSaved, this)){
        FString message = FString::Printf(
            TEXT("Storage Interface PathFinder Loaded Nodes from storage (%d)"),
            countNodesTrackedInIdMap()
        );
        DebugHelper::logMessage(message);
    }else{
        DebugHelper::logMessage("Storage Interface PathFinder failed load from storage");

        //collect all edges from world if pathfinder wasnt inited yet!
        EdgeCollector c = EdgeCollector();
        c.getAllEdges(GetWorld()); //pushes them to the navmesh on its own
	
    }
    wasLoaded = true;
}

void APathFinder::KillInstance(){
    if(wasLoaded){
        //save nodes to storage
        PathFinderStorageInterface interface;
        interface.Save(worldLevelNameSaved, this);
        wasLoaded = false;
        worldLevelNameSaved = TEXT("NONE");
    }

    //reset id map
    idMappedNodes.clear();

    //reset pointer
    resetPathFinderPointer();

    delete quadrantMap;
    quadrantMap = nullptr; //no double deletion




    clearDelegates();
}



void APathFinder::BeginPlay(){
    Super::BeginPlay();
}

void APathFinder::EndPlay(const EEndPlayReason::Type EndPlayReason){

    //Todo: Complete all tasks before saving to storage

    KillInstance();

    Super::EndPlay(EndPlayReason);
}

void APathFinder::Tick(float deltatime){
    Super::Tick(deltatime);

    if(rayTasksVec.size() > 0){
        int tasksPerTick = 10000;
        for (int i = 0; i < tasksPerTick; i++){
            if(rayTasksVec.size() <= 0){
                return;
            }
            raycastTask &current = rayTasksVec.back();
            current.execute();
            rayTasksVec.pop_back();
            if(i == tasksPerTick-1){
                DebugHelper::showScreenMessage("NEW SYNC TRACE BATCH MADE!",rayTasksVec.size(), FColor::Orange);
            }
        }
    }
}



int APathFinder::getNextStaticId(){
    int copy = staticId;
    staticId++;
    return copy;
}




//constructors end

//clear functions

/// @brief clears all nodes from the whole navmesh but doesnt delete the chunks
void APathFinder::clear(){
    if(quadrantMap){
        quadrantMap->clear();
    }
    clearDelegates();
}





void APathFinder::clearDelegates(){
    //delete all delegates
    for (int i = 0; i < released.size(); i++){
        FTraceDelegate *delegate = released.at(i);
        if (delegate != nullptr)
        {
            //DebugHelper::logMessage("debug deleted delegate");
            delete delegate;
            
        }
    }
    released.clear();
}

//clear functions end

//PATH FINDER METHODS



/// @brief acces the pathFinder instance, WILL NOT CREATE ONE, launch pathfinder through pathfinder module!
/// CAN RETURN NULLPTR
/// DO NOT DELETE
/// @return
APathFinder* APathFinder::instance(){
    return pathFinderInstance;
}





//debug drawing
void APathFinder::showPos(FVector e){
    if(debugDrawNodes){
        showPos(e, FColor::Green);
    }
}

void APathFinder::showPos(FVector e, FColor c){
    if(debugDrawNodes){
        FVector End = e + FVector(0, 0, 10000);
        DebugHelper::showLineBetween(GetWorld(), e, End, c);
    }
}


/// @brief adds a new node vector (of locations) to the graph and applies an offset to each node
/// all nodes are independant
/// if wanted
/// @param vec vector to add to graph 
/// @param offset offset to apply to each node
void APathFinder::addNewNodeVector(std::vector<FVector> &vec, FVector &offset){
    for (int i = 0; i < vec.size(); i++){
        addNewNode(vec.at(i) + offset);
    }
}




/// @brief add nodes to the graph, all independant
/// @param vec vector to push completly
void APathFinder::addNewNodeVector(std::vector<FVector>& vec){
    for (int i = 0; i < vec.size(); i++){
        addNewNode(vec.at(i));
    }

}



/// @brief expects the vector to be a convex hull of an object / grounded nodes! Do not ignore!
/// @param vector vector of positions, convex hull!
void APathFinder::addConvexHull(TArray<FVector> &vec){
    if(quadrantMap){
        quadrantMap->addConvexHull(vec);
    }
}



/// @brief adds a single node to the graph
/// @param a node to add
void APathFinder::addNewNode(FVector a){
    if(quadrantMap){
        quadrantMap->addNewNode(a);
    }
}






// --- POLYGON TRACKING ---






// --- STORAGE INTERFACE METHODS ---

const std::map<int, PathFinderNode *> &APathFinder::IdMapReference(){
    return idMappedNodes;
}

void APathFinder::addNodeFromStorageInterfaceNoConnection(
    FVector &position,
    int id
){
    if(!IdIsValid(id)){
        return;
    }
    if(NodeIsTrackedInIdMap(id)){
        return;
    }
    if(quadrantMap){
        PathFinderNode *nodeNew = new PathFinderNode(id, position);
        quadrantMap->addNoConnect(nodeNew);
        TrackNodeInIdMap(nodeNew);
    }

}

void APathFinder::addConnectionsFromStorageInterfaceForNodeById(
    int id,
    TArray<int> connected,
    int convexA,
    int convexB
){
    //connect to all neighbors if possible!
    if(NodeIsTrackedInIdMap(id)){
        PathFinderNode *current = idMappedNodes[id];
        if(current){

            //add connected neighbors
            for (int i = 0; i < connected.Num(); i++){
                int neighborId = connected[i];
                if(NodeIsTrackedInIdMap(neighborId)){
        
                    //special convex hull ignored for now. / is not used at all.
                    PathFinderNode *other = idMappedNodes[neighborId];
        
                    other->addTangentialNeighbor(current);
                    current->addTangentialNeighbor(other);
                }
            }

            //set convex neighbors
            if(NodeIsTrackedInIdMap(convexA)){
                current->setConvexNeighborA(idMappedNodes[convexA]);
            }
            if(NodeIsTrackedInIdMap(convexB)){
                current->setConvexNeighborB(idMappedNodes[convexB]);
            }
        }
    }
}




void APathFinder::TrackNodeInIdMap(PathFinderNode *node){
    if(node){
        int idRead = node->getId();
        if(idMappedNodes.find(idRead) == idMappedNodes.end()){
            idMappedNodes[idRead] = node;
        }
    }
}

bool APathFinder::NodeIsTrackedInIdMap(int id){
    return idMappedNodes.find(id) != idMappedNodes.end();
}

int APathFinder::countNodesTrackedInIdMap(){
    return idMappedNodes.size();
}

// --- Storage interface methods end ---







/// @brief finds a node from the correct quadrant
/// @param node 
/// @return 
PathFinderNode* APathFinder::findNode(FVector node){
    if(quadrantMap){
        return quadrantMap->findNode(node);
    }

    return nullptr;
}

PathFinderNode* APathFinder::findNodeInDirection(FVector &node, FVector &dir){
    if(quadrantMap){
        return quadrantMap->findNodeInDirection(node, dir);
    }

    return nullptr;
}





std::vector<PathFinderNode *> APathFinder::getSubGraph(FVector a, FVector b){
    std::vector<PathFinderNode *> nodes;
    if(quadrantMap){
        return quadrantMap->getSubGraph(a, b);
    }
    return nodes;
}


void APathFinder::debugCountNodes(){

    FVector a = FVector(-99999999999, 0, -99999999999);
    FVector b = FVector(99999999999, 0, 99999999999);

    std::vector<PathFinderNode *> nodes = getSubGraph(a, b);

    
    FString string2 = FString::Printf(TEXT("collected SUBGRAPH %d"), nodes.size());

    if (GEngine && false)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, string2);
    }



    //draw nodes
    for (int i = 0; i < nodes.size(); i++){
        //each neighbor, draw line
        PathFinderNode *current = nodes.at(i);
        if (current != nullptr)
        {
            for (int j = 0; j < current->visible_tangential_Neighbors.size(); j++){
                PathFinderNode *currNeighbor = current->visible_tangential_Neighbors.at(j);
                if(currNeighbor != nullptr){
                    DebugHelper::showLineBetween(
                        GetWorld(),
                        current->pos,
                        currNeighbor->pos,
                        FColor::Red,
                        100.0f
                    );

                }
            }
        }
    }
}



/// @brief returns a path from a given position a to an given position b
/// @param a fvector start point targeted
/// @param b fvector end point targeted
/// @return a path or an emtpy vector if no path was found
std::vector<FVector> APathFinder::getPath(FVector a, FVector b){

    //check if is last path
    if(prevPath.size() > 0){
        FVector s = prevPath.front();
        FVector e = prevPath.back();
        if(
            FVector::Dist(s, a) <= 500 && 
            FVector::Dist(e, b) <= 500
        ){
            return prevPath;
        }
    }

    PathFinderNode *start = nullptr;
    PathFinderNode *end = nullptr;

    FVector dir = b - a;
    start = findNodeInDirection(a, dir);
    //start = findNode(a);
    if(start == nullptr){
        DebugHelper::showScreenMessage("START NODE NOT FOUND", FColor::Red);
    }

    end = findNode(b);
    if(end == nullptr){
        DebugHelper::showScreenMessage("END NODE NOT FOUND", FColor::Red);
    }



    if(start != nullptr && end != nullptr){
        
        //PREBUILD EDGES
        if(GetPrebuildEdgesEnabled()){
            if(false){
                showPos(start->pos, FColor::Blue);
            }
            
            showPos(end->pos, FColor::Purple);
            return findPath_prebuildEdges(start, end);
        }

        //LIVE CREATED EGDES

        //find path
        std::vector<PathFinderNode *> graph = getSubGraph(a, b);
        
        if(debugDrawNodes && false){
            showPos(start->pos, FColor::Blue);
            showPos(end->pos, FColor::Red);
            DebugHelper::showLineBetween(GetWorld(), start->pos, end->pos, FColor::Yellow);
        }
        
        return findPath(start, end, graph);
    }



    return std::vector<FVector>();
}


/// @brief calculates the distance between 2 nodes and their positions
/// @param A 
/// @param B 
/// @return 
float APathFinder::distance(PathFinderNode* A, PathFinderNode *B){
    if(A != nullptr && B != nullptr){
        float d = FVector::Dist(A->pos, B->pos);
        return d;
    }
    return std::numeric_limits<float>::max();
}

float APathFinder::distance(FVector A, FVector B){
    return FVector::Dist(A, B);
}




/// @brief finds the path between 2 given nodes in the subgraph
/// @param start node from the graph
/// @param end node from the graph
/// @param subgraph graph enclosed by start and end node. Use Subgraph method
/// @return vector of positions: path
std::vector<FVector> APathFinder::findPath(
    PathFinderNode *start, 
    PathFinderNode *end, 
    std::vector<PathFinderNode*> &subgraph
){
    //screenMessage(FString::Printf(TEXT("subgraph size %d"), subgraph.size()));
    

    for (int i = 0; i < subgraph.size(); i++){
        PathFinderNode *n = subgraph.at(i);
        if(n != nullptr){
            n->reset();
            n->closedFlag = false;
        }
    }

    start->camefrom = nullptr;
    start->closedFlag = false;
    start->gx = 0;
    start->fx = 0;

    end->camefrom = nullptr;
    end->closedFlag = false;

    std::vector<PathFinderNode*> openList;

    
    priorityQueue openList_;
    openList_.add(start);

    while(openList_.hasNodes()){
    
        PathFinderNode *current = openList_.popLowestFx();

        if (current != nullptr)
        {
            if(reached(current, end)){
                //path found
                //screenMessage("found path");
                return constructPath(end);
            }

            //show opened nodes: debugging
            if(debugDrawNodes){
                showPos(current->pos, FColor::Blue);
            }
            

            current->close();
            for (int i = 0; i < subgraph.size(); i++)
            {
                PathFinderNode *n = subgraph.at(i);
                if(n != nullptr){
                    //bool wasClosed = n->closedFlag;
                    if(!n->isClosed()){
                        
                        //create edge at runtime: 
                        //all edges are rechecked during runtime 
                        //because otherwise it could not operate on subgraphs
                        //but is nesecarry to eliminate many nodes at once
                        if(canSeeTangential(current, n)){
                            

                            //gxnew = gx + hx, potentially smaller then prev of other
                            //is shorter path.
                            float gxNew = current->gx + distance(current->pos, n->pos); 
                            if(gxNew < n->gx){
                                //screenMessage(300);
                                float hxEnd = distance(n->pos, end->pos);
                                n->updateCameFrom(gxNew, hxEnd, *current);

                                
                            }
                            //ADD TO OPEN LIST!!
                            openList_.add(n); 
                        }

                        
                    }
                }
            }
        }
    }

    /*
    if (GEngine)
    {
        FString string = FString::Printf(TEXT("out of nodes"));
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, string);
    }*/

    return std::vector<FVector>();
}


bool APathFinder::reached(PathFinderNode *a, PathFinderNode *b){
    if(a == nullptr || b == nullptr){
        return false;
    }
    if(a == b){
        return true;
    }
    //neighbor

    //distance
    if(FVector::Dist(a->pos, b->pos) <= 500){
        return true;
    }

    //can see
    if (GetPrebuildEdgesEnabled() == false && canSee(a->pos, b->pos)){
        return true;
    }
    return false;
}

void APathFinder::screenMessage(int s){
    if (GEngine)
    {
        FString string = FString::Printf(TEXT("text %d"), s);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, string);
    }
}

void APathFinder::screenMessage(FString s) {
    if (GEngine) {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, s);
    }
}



/// @brief checks if 2 nodes can see each other, damagable interfaces are excluded for now.
/// @param A position A
/// @param B position B
/// @return can see without interrupt
bool APathFinder::canSeeTangential(PathFinderNode *A, PathFinderNode*B){
    if(A && B){

        // if edge is too vertical and to high: ignore, cant climb walls.
        // AB = B - A;
        if(isCloseAndTooVertical(A, B)){
            return false;
        }

        //pass tangental check to check for raycast or not
        if (passTangentailCheck(A,B) == false){
            return false; //not tangential, do not check
        }

        // vision checking
        FVector Start = A->pos;
        FVector End = B->pos;
        //testing with more raycasts to ensure realibilty
        if (canSee(Start, End)){
            return true;
        }
        



        //adjustments if any entity might be in way
        Start.Z += GetOneMeter() * 1.7f;
        End.Z += GetOneMeter() * 1.7f;
        if (canSee(Start, End)){
            return true;
        }

    }
    return false; //issue: can see false.
}

/// @brief checks with a simple raycasts if nodes can see each other
/// @param Start start node
/// @param End end node
/// @return edge of interest to pass / existent, efficent
bool APathFinder::canSee(FVector &Start, FVector &End){
    if(UWorld *worldPointer = GetWorld()){
        FHitResult HitResult;
        FCollisionQueryParams Params = collsionParamsLowDetailAndFast();
        //Params.bTraceComplex = false; // HIER LOWER RAYCAST DETAIL-> FASTER

        //add params from entity manager (contains all bots for example, which can be ignored)
        //part of a bigger context im working on, comment out or provide your own params
        /*if(EntityManager *e = AworldLevel::entityManager()){
            Params = e->getIgnoredRaycastParams();
        }*/
        Params = collisionIgnoreParams;

        //default casting
        bool hittedAnything = worldPointer->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
        bool canSee = !hittedAnything;
        return canSee;
    }
    return false;
}

std::vector<FVector> APathFinder::constructPath(PathFinderNode *end){
    std::vector<FVector> list;
    if (end != nullptr)
    {
        while(end != nullptr){
            list.push_back(end->pos);
            end = end->camefrom;
        }

        std::reverse(list.begin(), list.end());
    }
    prevPath = list; //save copy
    return list;
}

/// @brief if edge is too vertical and to high: ignore, cant climb walls.
/// @param a node a 
/// @param b node b
/// @return too close and vertical true or false
bool APathFinder::isCloseAndTooVertical(PathFinderNode *a, PathFinderNode *b){

    if(a != nullptr && b != nullptr){
        // AB = B - A;
        FVector AB = b->pos - a->pos;
        float upZ = std::abs(AB.GetSafeNormal().Z); //if the normalized vector, Z is close to 1 its paralell to up axis
        float scale = std::abs(AB.Z);
        if (upZ > 0.8f)
        {
            // skalarproduct up is near one / paralell to z axis
            if (AB.Z > GetOneMeter())
            {
                return true;
            }
        }
    }
    return false;
}











void APathFinder::draw(FVector &pos){
    FVector up = pos + FVector(0, 0, 10000);
    DebugHelper::showLineBetween(GetWorld(), pos, up, FColor::Red, 5.0f);
}














/**
 * 
 * 
 * CONNECT SECTION
 * 
 * 
 */



/// @brief connects a node in all quadrants IF ENABLED BOOL IN HEADER FILE
///
/// --- ISSUE I AM AWARE OF : ---
/// if 2 nodes are compared to each other which are part of 1 convex hull the condition
/// of tangentiality wont be ever true. 
/// since it still brings some computanional overhead
/// the plottet nodes will maybe tracked in polygonal shape-objects some time in future.
///
/// it is unclear for me whether i will implement that because searching for the node
/// in a list brings with O(n) rougly the same overhead as checking the tangential edges for
/// all points, which is also already greatly reduced by limiting a max distance and 
/// the subgraph functionality.
/// @param node node to connect
void APathFinder::connect(PathFinderNode *node){
    if(node != nullptr && GetPrebuildEdgesEnabled()){

        //find min max x and y for distance
    
        int lowerX = node->pos.X - PREBUILD_MAXDISTANCE;
        int lowerY = node->pos.Y - PREBUILD_MAXDISTANCE;
        int higherX = node->pos.X + PREBUILD_MAXDISTANCE;
        int higherY = node->pos.Y + PREBUILD_MAXDISTANCE;

        FVector a(lowerX, lowerY, 0);
        FVector b(higherX, higherY, 0);

        std::vector<PathFinderNode *> enclosedByMaxDistance = getSubGraph(a, b);



        for (int i = 0; i < enclosedByMaxDistance.size(); i++){
            PathFinderNode *compare = enclosedByMaxDistance.at(i);
            if(compare != nullptr && compare != node){
                // includes tangential check if possible!
                if(APathFinder *p = APathFinder::instance()){

                    

                    if(traceMode == APathFinder::PathTraceMode::AsyncTrace){
                        asyncCanSee(node, enclosedByMaxDistance.at(i));
                    }
                    if(traceMode == APathFinder::PathTraceMode::SyncTraceByTick){
                        addNewRaytask(node, enclosedByMaxDistance.at(i));
                    }
                    if(traceMode == APathFinder::PathTraceMode::SyncTrace){
                        if (p->canSeeTangential(node, enclosedByMaxDistance.at(i))) 
                        {
                            node->addTangentialNeighbor(compare);
                            compare->addTangentialNeighbor(node);
                        }
                    }



                    
                }
            }
            
        }



    }
}

/// @brief connects two nodes asynchronosly if those are tangential
/// @param a node a
/// @param b node b
/// both nodes must not be nullptr,
/// dont delete the nodes, they are passed into the lambda!
void APathFinder::asyncCanSee(PathFinderNode *a, PathFinderNode *b){
    if(a != nullptr && b != nullptr){

        //needs still to pass the tangential check. Remember: if no convex hull, automaically passes the test
        if(!passTangentailCheck(a,b)){
            return;
        }

        if(UWorld *worldPointer = GetWorld()){

            //async cast if prebuild
            if(GetPrebuildEdgesEnabled()){

                FHitResult HitResult;
                FCollisionQueryParams Params = collsionParamsLowDetailAndFast();

                //add params from entity manager (contains all bots for example, which can be ignored)
                //part of a bigger context im working on, comment out or provide your own params
                /*if(EntityManager *e = AworldLevel::entityManager()){
                    Params = e->getIgnoredRaycastParams();
                }*/
                Params = collisionIgnoreParams;
                Params.bTraceComplex = false;

                //bool result = false;
                FVector start = a->pos;
                FVector end = b->pos;

                /**
                 * EAsyncTraceType::Single: Use this for a single line trace (just one raycast).
                 * EAsyncTraceType::Multi: Use this if you want to collect multiple hits along the ray
                 */
                

                FTraceDelegate *MyTraceDelegate = requestDelegate(a, b);
                if(MyTraceDelegate != nullptr){
                    // Now pass the delegate by reference
                    worldPointer->AsyncLineTraceByChannel(
                        EAsyncTraceType::Single,    // Or Multi, depending on what you need
                        start,                      // Start point (FVector)
                        end,                        // End point (FVector)
                        ECC_Visibility,             // Collision channel
                        Params,            // Collision query parameters
                        FCollisionResponseParams(), // Response parameters
                        //&MyTraceDelegate //call back 
                        MyTraceDelegate //call back 
                    );
                }else{
                    DebugHelper::showScreenMessage("issue with delegate occured");
                }

                
            }
        }
    }
}



/// @brief request trace delegate to connect nodes a and b on 
/// @param a 
/// @param b 
/// @return 
FTraceDelegate *APathFinder::requestDelegate(PathFinderNode *a, PathFinderNode *b){

    if(a != nullptr && b != nullptr){
        FScopeLock Lock(&delegate_CriticalSection_a);

        FTraceDelegate *delegate = nullptr;
        if (released.size() > 0)
        {
            delegate = released.back();
            released.pop_back();
        }
        if(delegate == nullptr){
            delegate = new FTraceDelegate();
        }
        if(delegate != nullptr){
            delegate->BindLambda(
                [a, b, delegate]
                (const FTraceHandle &TraceHandle, FTraceDatum &TraceData){

                // Lambda logic for handling the trace result
                bool bHit = TraceData.OutHits.Num() > 0;

                if(bHit){
                    FHitResult hitP = TraceData.OutHits[0];
                    FVector hitPos = hitP.ImpactPoint;

                    //if distance close enough to target, forgive the hit.
                    if(FVector::Dist(hitPos, b->pos) <= 25){ //50cm
                        bHit = false; //testing needed
                        //DebugHelper::showScreenMessage("async trace false positive", FColor::Purple);
                    }

                    
                }

                //no hit, can see.
                if(!bHit){
                    a->addTangentialNeighbor(b);
                    b->addTangentialNeighbor(a);

                    //DebugHelper::showScreenMessage("async trace made new", FColor::Yellow);
                    /*
                    if(this->GetWorld() ){
                        DebugHelper::showLineBetween(
                            GetWorld(),
                            a->pos,
                            b->pos,
                            FColor::Blue,
                            1.0f
                        );
                    }*/
                }

                

                if(APathFinder *i = APathFinder::instance()){
                    i->freeDelegate(delegate);
                }        
            });
        }
        
        return delegate;
    }
    return nullptr;
}

/// @brief synchronously releases the ftrace delegate for re usal
/// @param d ftracedelegate to realease
void APathFinder::freeDelegate(FTraceDelegate *d){
    if(d != nullptr){

        d->Unbind();
        FScopeLock Lock(&delegate_CriticalSection_b); //other lock so the locks dont block each other
        released.push_back(d);
    }
}







/// @brief custom path finding method for graph with prebuild edges
/// @param start start node
/// @param end end node
/// @return the path if the minimal one found
std::vector<FVector> APathFinder::findPath_prebuildEdges(
    PathFinderNode *start,
	PathFinderNode *end
){
    if(start == nullptr || end == nullptr){
        std::vector<FVector> o;
        return o;
    }
    //DebugHelper::showScreenMessage("ask path");
    start->camefrom = nullptr;
    start->closedFlag = false;
    start->gx = 0;
    start->fx = 0;
    end->reset();
    end->camefrom = nullptr;
    end->closedFlag = false;


    //bounding box for traversed nodes
    /*
    FVector center = (start->pos + end->pos) / 2;
    int lowerX = std::min(start->pos.X, end->pos.X);
    int lowerY = std::min(start->pos.Y, end->pos.Y);
    int higherX = std::max(start->pos.X, end->pos.X);
    int higherY = std::max(start->pos.Y, end->pos.Y);

    FVector lower(lowerX, lowerY, 0);
    FVector higher(higherX, higherY, 0);
    */

    //int boundingBoxIncreaseFrac = 2;
    //lower += (center - lower) * boundingBoxIncreaseFrac; // AB = B - A
    //higher += (center - higher) * boundingBoxIncreaseFrac; // AB = B - A



    std::vector<PathFinderNode *> markedForCleanUp; //must be cleaned before returning path
    priorityQueue open;
    open.add(start);

    while(open.hasNodes()){
        PathFinderNode *current = open.popLowestFx();
        

        if (current != nullptr)
        {
            //debugDraw
            PathFinderNode *prevNode = current->camefrom;
            if(prevNode != nullptr && false){
                DebugHelper::showLineBetween(
                    GetWorld(),
                    current->pos,
                    prevNode->pos,
                    FColor::Cyan,
                    5.0f
                );
            }

        
            markedForCleanUp.push_back(current); // dont forget to push back for cleaning up later
            
            if (current == end){

                std::vector<FVector> outputPath = constructPath(end);
                
                //draw
                for (int i = 1; i < outputPath.size(); i++){
                    FVector nodeA = outputPath.at(i);
                    FVector nodeB = outputPath.at(i-1);
                    
                    if(false){
                        DebugHelper::showLineBetween(
                            GetWorld(),
                            nodeA,
                            nodeB,
                            FColor::Red,
                            5.0f
                        );
                    }
                    
                }

                // clean all status to prevent issues, everything must be cleared
                for (int i = 0; i < markedForCleanUp.size(); i++)
                {
                    PathFinderNode *n = markedForCleanUp.at(i);
                    if (n != nullptr)
                    {
                        n->reset();
                    }
                }
                DebugHelper::showScreenMessage("Path Found", FColor::Purple);
                return outputPath;
            }

            //not opened yet
            current->close(); //close node

            //DebugHelper::showScreenMessage("neighbors", current->visible_tangential_Neighbors.size(), FColor::Red);
            
            //traverse NOT opened neighbors and add to open if not added yet (will be handeld by queue automatically)
            for (int i = 0; i < current->visible_tangential_Neighbors.size(); i++){
                PathFinderNode *neighbor = current->visible_tangential_Neighbors[i];
                if(neighbor != nullptr){
                    if(!neighbor->isClosed()){ //open only if not closed

                        float gxNew = current->gx + distance(current->pos, neighbor->pos);
                        //DebugHelper::showScreenMessage("try gx ", gxNew, FColor::Purple);
                        //DebugHelper::showScreenMessage("compare to gx ", neighbor->gx, FColor::Purple);
                        if(gxNew < neighbor->gx){
                            //DebugHelper::showScreenMessage("update gx ", gxNew, FColor::Green);
                            float hxEnd = distance(neighbor->pos, end->pos);
                            neighbor->updateCameFrom(gxNew, hxEnd, *current);
                        }
                        //ADD TO OPEN LIST!! //if readded is bubbled up automatically!
                        open.add(neighbor);

                        //even if a node wasnt the lowest it must be cleaned later!
                        markedForCleanUp.push_back(neighbor);
                    }else{
                        //DebugHelper::showScreenMessage("is closed", FColor::Purple);
                    }
                }
            }
        }else{
            //issue
            break;
        }
    }

    //no path found, make sure to reset all nodes flag status
    for (int i = 0; i < markedForCleanUp.size(); i++){
        PathFinderNode *n = markedForCleanUp.at(i);
        if(n != nullptr){
            n->reset();
        }
    }

    DebugHelper::showScreenMessage("no path found");


    std::vector<FVector> placeholder;
    return placeholder;
}


/// @brief checks if a node is within the bounding box enclosed by a and b (ordering is not important)
/// @param a pos a
/// @param b pos b
/// @param check node to check, must not be nullptr
/// @return within box or not. If no node provided, default is false
bool APathFinder::isInBounds(FVector &a, FVector &b, PathFinderNode *check){
    if(check != nullptr){
        FVector c = check->pos;
        return (a.X <= c.X && a.Y <= c.Y && c.X <= b.X && c.Y <= b.Y);
    }
    return false;
}

/**
 * 
 * --- TANGENTIAL CHECKING ---
 * 
 */


/// @brief needs to pass the tangential check before being a node of interest
/// requires both passed nodes to have their very next neighbors and both neighbors being
/// part of the convex hull of the polygon the nodes / edges were collected from
/// ---> if a or b dont have neighbors (not part of a convex hull, true is automatically returned, test passed.)
/// @param a node a
/// @param b node b
/// @return is tangential: no intersection of polygons or not
bool APathFinder::passTangentailCheck(PathFinderNode *a, PathFinderNode *b){
    if(a != nullptr && b != nullptr)
    {
        //must be tangential no intersect on both sides to be an connection of intersect
        if(a->hasNeighbors() && b->hasNeighbors())
        {
            bool dirAB_ok = false;
            FVector v0 = b->pos - a->pos;
            FVector vN(v0.Y, -v0.X, 0); // 90 grad drehen: - für eine var, xy tauschen, einen component negieren

            FVector v1 = b->nA->pos - b->pos; // AB = B - A connect to first neighbor
            FVector v2 = b->nB->pos - b->pos; //connect to second neighbor

            //(vN dot v1) * (vN dot v2) > 0 damit beide vektoren auf der selben seite sind. tangentiale konvexe verbindung
            float dotProduct1 = (vN.X * v1.X) + (vN.Y * v1.Y);
            float dotProduct2 = (vN.X * v2.X) + (vN.Y * v2.Y);
            if(dotProduct1 * dotProduct2 >= 0) // >=
            {//vorzeichen gleich, >0 selbe seite
                dirAB_ok = true;
            }


            bool dirBA_ok = false;
            v0 = a->pos - b->pos;

            v1 = (a->nA->pos - a->pos); // AB = B - A connect to first neighbor
            v2 = (a->nB->pos - a->pos); //connect to second neighbor

            //(vN dot v1) * (vN dot v2) > 0 damit beide vektoren auf der selben seite sind. tangentiale konvexe verbindung
            dotProduct1 = (vN.X * v1.X) + (vN.Y * v1.Y);
            dotProduct2 = (vN.X * v2.X) + (vN.Y * v2.Y);
            if(dotProduct1 * dotProduct2 >= 0){//vorzeichen gleich, >0 selbe seite
                dirBA_ok = true;
            }



            
            if(false && dirAB_ok && dirBA_ok){
                
                DebugHelper::showLineBetween(GetWorld(), a->pos, b->pos, FColor::Green, 100.0f);
                
            }

            return dirAB_ok && dirBA_ok;
        }

        if(false){
            DebugHelper::showLineBetween(GetWorld(), a->pos, b->pos, FColor::Yellow, 100.0f);
        }
        return true; //if is not part of a convexx hull, true by default
    }
    
    return false; //issue, not valid nodes passed!
}


/// @brief collision params with trace complex false: Faster
/// @return 
FCollisionQueryParams APathFinder::collsionParamsLowDetailAndFast(){
    FCollisionQueryParams params;
    params.bTraceComplex = false;
    return params;
};

// ------ bot helper -------
FVector APathFinder::findFurthestConnectedNodeFrom(FVector &other){
    PathFinderNode *foundTargetNode = findNode(other);
    if(foundTargetNode != nullptr){
        std::vector<PathFinderNode *> &neighborRef = foundTargetNode->visible_tangential_Neighbors;
        if (neighborRef.size() > 0)
        {
            PathFinderNode *farthest = neighborRef[0];
            FVector farthestPos;
            float dist = 99999999;
            if(farthest != nullptr){
                dist = FVector::Dist(farthest->pos, foundTargetNode->pos);
                farthestPos = farthest->pos;
            }
            for (int i = 1; i < neighborRef.size(); i++){
                PathFinderNode *current = neighborRef[i];
                if(current != nullptr){
                    float newdist = FVector::Dist(current->pos, farthestPos);
                    if(newdist > dist){
                        dist = newdist;
                        farthestPos = current->pos;
                    }
                }
            }

            
            DebugHelper::showLineBetween(
                GetWorld(),
                farthestPos,
                foundTargetNode->pos,
                FColor::Cyan,
                2.0f
            );
            

            return farthestPos;
        }
    }
    return other;
}

/**
 * 
 * 
 * ----- TICKED SYNCHORNONIZED CONNECTOR, WORKS WORSE THAN ASYNC TRACE, DO NOT USE ------
 * 
 * 
 */


void APathFinder::addNewRaytask(PathFinderNode *a, PathFinderNode *b){
    
    if(a != nullptr && b != nullptr){
        raycastTask newTask;
        newTask.setup(GetWorld(), a, b);
        rayTasksVec.push_back(newTask);
    }
}





//debug
void APathFinder::debugShowAllNodes(UWorld *world){
    if(quadrantMap){
        quadrantMap->debugShowAllNodes(world);
    }
}







//ignore params
void APathFinder::addActorToIgnoreRaycastParams(AActor *actor){
    if(actor != nullptr){
        collisionIgnoreParams.AddIgnoredActor(actor);
    }
}

FCollisionQueryParams APathFinder::getIgnoredRaycastParams(){
    return collisionIgnoreParams;
}





void APathFinder::CollectNodePositions(
    const FVector &pos, 
    float radius,
    TArray<FVector> &outArray
){
    radius *= 0.5f;
    FVector halfDir(radius, radius, 0.0f);

    FVector posBottom = pos - halfDir;
    FVector posTop = pos + halfDir;

    std::vector<PathFinderNode *> partial = getSubGraph(posBottom, posTop);
    outArray.Empty();
    outArray.SetNumUninitialized(partial.size());
    for (int i = 0; i < partial.size(); i++){
        if(PathFinderNode *current = partial[i]){
            outArray[i] = current->pos;
        }
    }
}



std::vector<FMeshedPolygon *> APathFinder::GetAllPolygons(){
    if(quadrantMap){
        return quadrantMap->GetAllPolygons();
    }
    return std::vector<FMeshedPolygon *>();
}

void APathFinder::addAllPolygons(std::vector<FMeshedPolygon *> &polygons){
    if(quadrantMap){
        quadrantMap->addAllPolygons(polygons);
    }
}





bool APathFinder::TryGetSubGraphPolygonMesh(
    FMeshedPolygon &outData,
    FVector center, 
    float sizeSquare
){
    if(quadrantMap){
        quadrantMap->GetSubGraphPolygonMesh(center, sizeSquare, outData);
        return true;
    }
    return false;
}