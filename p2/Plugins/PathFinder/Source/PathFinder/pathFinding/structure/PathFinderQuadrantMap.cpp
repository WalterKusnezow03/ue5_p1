#include "PathFinderQuadrantMap.h"

#include "DebugPlugin/DebugHelper.h"

PathFinderQuadrantMap::PathFinderQuadrantMap(APathFinder *finder){
    parent = finder;

    TopRight = new PathFinderQuadrant(1, 1, finder);
    BottomRight = new PathFinderQuadrant(1,-1, finder);
	TopLeft = new PathFinderQuadrant(-1,1, finder);
	BottomLeft = new PathFinderQuadrant(-1,-1, finder);


}

PathFinderQuadrantMap::~PathFinderQuadrantMap(){
    //delete all
    delete (TopRight); 
    delete (BottomRight); 
    delete (BottomLeft); 
    delete (TopLeft);
    TopRight = nullptr;
    TopLeft = nullptr;
    BottomRight = nullptr;
    BottomLeft = nullptr;

}


void PathFinderQuadrantMap::clear(){
    TArray<PathFinderQuadrant*> array = {TopLeft, BottomLeft, TopRight, BottomRight};
    for (int i = 0; i < array.Num(); i++){
        if(array[i] != nullptr){
            array[i]->clear();
        }
    }
}

    


std::vector<PathFinderNode *> PathFinderQuadrantMap::getSubGraph(FVector a, FVector b){

    std::vector<PathFinderNode *> nodes;
    std::vector<PathFinderNode *> asked;

    //why is this just iterating over all quadrants:
    //the "askForArea(a,b)" method is clamping the coordinates by it self
    //to the correct values to properly get all nodes in the correct area.
    TArray<PathFinderQuadrant*> array = {TopLeft, BottomLeft, TopRight, BottomRight};
    
    
    
    for (int i = 0; i < array.Num(); i++){
        if(array[i] != nullptr){

            asked = array[i]->askForArea(a, b);
            if(asked.size() > 0){
                nodes.insert(nodes.end(), asked.begin(), asked.end());
            }

            
        }
    }
    return nodes;
}


PathFinderQuadrant* PathFinderQuadrantMap::askforQuadrant(PathFinderNode *node){
    if(node){
        FVector pos = node->pos;
        return askforQuadrant(pos.X, pos.Y);
    }
    return nullptr;
}

PathFinderQuadrant* PathFinderQuadrantMap::askforQuadrant(int xIndex, int yIndex){
    //top left
    if(xIndex < 0 && yIndex >= 0){
        return this->TopLeft;
    }
    //top right
    if(xIndex >= 0 && yIndex >= 0){
        return this->TopRight; 
    }
    //bottom left
    if(xIndex < 0 && yIndex < 0){
        return this->BottomLeft;
    }
    //bottom right
    if(xIndex >= 0 && yIndex < 0){
        return this->BottomRight;
    }
    return nullptr;
}



void PathFinderQuadrantMap::debugShowAllNodes(UWorld *world){
    int count = 0;
    if (TopRight)
    {
        TopRight->debugShowAllNodes(world);
        count += TopRight->chunkCount();
    }
    if(BottomRight){
        BottomRight->debugShowAllNodes(world);
        count += TopRight->chunkCount();
    }
    if(TopLeft){
        TopLeft->debugShowAllNodes(world);
        count += TopRight->chunkCount();
    }
    if(BottomLeft){
        BottomLeft->debugShowAllNodes(world);
        count += TopRight->chunkCount();
    }

    DebugHelper::logMessage("debugPathfinder chunk count", count);
}

PathFinderNode* PathFinderQuadrantMap::findNode(FVector node){
    int x = (int) node.X;
    int y = (int) node.Y;
    PathFinderQuadrant *q = askforQuadrant(x, y);
    if(q != nullptr){
        PathFinderNode *nodeFound = q->findNode(node);
        if(nodeFound != nullptr){
            DebugHelper::showScreenMessage("node found path finder", FColor::Green);
            return nodeFound;
        }
    }else{
        DebugHelper::showScreenMessage("QUADRANT IS NULL!!!!!", FColor::Red);
    }
    
    return nullptr;
}

PathFinderNode* PathFinderQuadrantMap::findNodeInDirection(FVector &node, FVector &dir){
    int x = (int) node.X;
    int y = (int) node.Y;
    PathFinderQuadrant *q = askforQuadrant(x, y);
    if(q != nullptr){
        PathFinderNode *nodeFound = q->findNodeInDirection(node, dir);
        if(nodeFound != nullptr){
            DebugHelper::showScreenMessage("node found path finder", FColor::Green);
            return nodeFound;
        }
    }else{
        DebugHelper::showScreenMessage("QUADRANT IS NULL!!!!!", FColor::Red);
    }
    
    return nullptr;
}


void PathFinderQuadrantMap::addNode(PathFinderNode * node){
    if(node != nullptr){
        PathFinderQuadrant *q = askforQuadrant(node);
        if(q != nullptr){
            //FScopeLock Lock(&delegate_CriticalSection_a); //new lock added
            q->add(node);
        }
    }
}

void PathFinderQuadrantMap::addNewNode(FVector a){
    PathFinderQuadrant *q = askforQuadrant(a.X, a.Y);
    if(q != nullptr){
        //FScopeLock Lock(&delegate_CriticalSection_a); //new lock added
        q->add(a);
    }
}

void PathFinderQuadrantMap::addNoConnect(PathFinderNode *node){
    PathFinderQuadrant *q = askforQuadrant(node);
    if(q != nullptr){
        q->addNoConnect(node);
    }
}