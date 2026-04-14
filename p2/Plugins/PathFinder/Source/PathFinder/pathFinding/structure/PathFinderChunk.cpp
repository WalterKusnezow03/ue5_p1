#include "PathFinderChunk.h"
#include "PathFinder/pathFinding/PathFinder.h"


PathFinderChunk::PathFinderChunk(APathFinder *parentIn){
    parent = parentIn;
}
PathFinderChunk::~PathFinderChunk(){
    clear();
}

/// @brief adds a new node to the vector with a position
/// @param vec position of the node to be added
void PathFinderChunk::add(FVector vec){
    //find closest node near by
    

    if(!hasNode(vec)){
        PathFinderNode *node = new PathFinderNode(vec);
        nodes.push_back(node);
        //connects to all nodes if enabled in header
        if(parent){
            parent->connect(node);
        }
    }
}

/// @brief adds a new node to the node with a position
/// @param vec position of the node to be added
void PathFinderChunk::add(PathFinderNode *node){
    if(node != nullptr){

        addNoConnect(node);
        // connects to all nodes if enabled in header
        if(parent){
            if(parent->GetPrebuildEdgesEnabled()){
                parent->connect(node);
            }
        }    
    }
}



void PathFinderChunk::addNoConnect(PathFinderNode *node){
    if(node){
        //will only check for duplicate nodes by adress
        for (int i = 0; i < nodes.size(); i++){
            if(nodes.at(i) == node){
                return;
            }
        }
        nodes.push_back(node);
    }
}

void PathFinderChunk::add(FMeshedPolygon *polygon){
    if(polygon){
        //will only check for duplicate nodes by adress
        for (int i = 0; i < polygons.size(); i++){
            if(polygons.at(i) == polygon){
                return;
            }
        }
        polygons.push_back(polygon);
    }
}





/// @brief returns the chunk nodes as reference
/// @return vector<Node> nodes as &ref
std::vector<PathFinderNode*> &PathFinderChunk::getNodes(){
    return nodes;
}

std::vector<FMeshedPolygon *> &PathFinderChunk::getPolygons(){
    return polygons;
}

void PathFinderChunk::AppendAllPolygons(std::vector<FMeshedPolygon *> &polygonsOut){
    if(polygons.size() > 0){
        polygonsOut.insert(polygonsOut.end(), polygons.begin(), polygons.end());
    }
}

/// @brief tries to find a node from a chunk, if not found: created
/// @param pos position of the targetet node
/// @return returns the closest node near by
PathFinderNode* PathFinderChunk::findNode(FVector pos){
    FVector dir(0.0f, 0.0f, 0.0f); //none
    return findNodeInDirection(pos, dir); //will be ignored
}

PathFinderNode* PathFinderChunk::findNodeInDirection(FVector &pos, FVector &dir){
    dir.Z = 0.0f;
    dir = dir.GetSafeNormal();

    bool checkDirNone = dir.X == 0.0f && dir.Y == 0.0f;
    bool checkDir = !checkDirNone;

    //add node if didnt had any yet
    if(nodes.size() <= 0){
        DebugHelper::showScreenMessage("RETURNED ASYNC CONNECT NODE ", FColor::Yellow);
        return lateadd(pos);
    }

    //find the closest node 
    float closest = std::numeric_limits<float>::max();
    PathFinderNode *closestNode = nullptr;
    float prevDotProduct = -2.0f; //worst is -1.0f

    for (int i = 0; i < nodes.size(); i++)
    {
        PathFinderNode *current = nodes.at(i);
        if (current != nullptr)
        {
            if(current->hasAnyNeighbors()){ //no neighbors makes no sense.
                float Difference = FVector::Dist(pos, current->pos);

                if(Difference < closest){

                    //direction check
                    if(checkDir){
                        FVector dirFromBotLocation = current->pos - pos; // AB = B - A
                        dirFromBotLocation.Z = 0.0f;
                        dirFromBotLocation = dirFromBotLocation.GetSafeNormal();

                        //dot product similar: ok
                        float dotProduct = FVector::DotProduct(dir, dirFromBotLocation);
                        //wenn das skalarprodukt zweier normalisierter
                        //vektoren 1 ergibt sind sie paralell zu einander
                        if(dotProduct > prevDotProduct){
                            closest = Difference;
                            closestNode = current;
                            prevDotProduct = dotProduct;
                        }
                    }else{
                        closest = Difference;
                        closestNode = current;
                    }

                    
                }
            }
            
        }
    }

    //connect node or not.
    if(parent){
        if(parent->GetPrebuildEdgesEnabled() && closestNode == nullptr){
            DebugHelper::showScreenMessage("ASYNC LATE CONNECT NODE ", FColor::Yellow);
            return lateadd(pos);
        }
    }
    
    

    return closestNode;





}











/// @brief will add a new node to the chunk and connect the edges if the prebuild mode is enabled / forced
/// @param pos position to create the new node at
/// @return created node, may not be fully connected yet if operation is async.
PathFinderNode* PathFinderChunk::lateadd(FVector pos){
    PathFinderNode *nodeNew = new PathFinderNode(pos);
    nodes.push_back(nodeNew);
    
    if(parent){
        if(parent->GetPrebuildEdgesEnabled()){
            parent->connect(nodeNew);
        }
    }
    
    return nodeNew;
}





/// @brief tries to find a node from a chunk as BOOL
/// @param pos position of the targetet node
/// @return returns the closest node near by
bool PathFinderChunk::hasNode(FVector pos){
    if(nodes.size() <= 0){
        return false;
    }

    int oneMeter = 100.0f;
    if(parent){
        oneMeter = parent->GetOneMeter();
    }

    PathFinderNode *closestNode = nodes.at(0);
    float closest = closestNode->Dist(pos);

    for (int i = 0; i < nodes.size(); i++){
        if (PathFinderNode *current = nodes[i])
        {

            float Difference = current->Dist(pos);
            if(Difference < closest){
                closest = Difference;
                closestNode = current;
            }
        }
    }
    if(closest <= (oneMeter / 3.0f)){
        return true;
    }
    return false;
}


void PathFinderChunk::debugShowAllNodes(UWorld *world){
    for (int i = 0; i < nodes.size(); i++){
        PathFinderNode *node = nodes[i];
        if(node){
            node->show(world);
        }
    }
}


void PathFinderChunk::clear(){
    for (int i = 0; i < nodes.size(); i++){
        if(nodes.at(i) != nullptr){
            delete (nodes.at(i));
            nodes.at(i) = nullptr;
        }
    }
    nodes.clear(); //finally clear the nodes


    for (int i = 0; i < polygons.size(); i++){
        if(polygons.at(i) != nullptr){
            delete (polygons.at(i));
            polygons.at(i) = nullptr;
        }
    }
    polygons.clear();
}