#include "PathFinderQuadrantMap.h"

#include "DebugPlugin/DebugHelper.h"
#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"


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
    TArray<PathFinderQuadrant *> array = allQuadrants();
    for (int i = 0; i < array.Num(); i++)
    {
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
    TArray<PathFinderQuadrant *> array = allQuadrants();

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
        return askforQuadrant(node->pos);
    }
    return nullptr;
}

PathFinderQuadrant* PathFinderQuadrantMap::askforQuadrant(const FVector &pos){
    return askforQuadrant(pos.X, pos.Y);
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

void PathFinderQuadrantMap::addNewNodes(const TArray<FVector> &data){
    for (int i = 0; i < data.Num(); i++){
        addNewNode(data[i]);
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






void PathFinderQuadrantMap::addAllNodes(std::vector<PathFinderNode*> &nodes){
    for (int i = 0; i < nodes.size(); i++){
        if(nodes.at(i) != nullptr){
            addNode(nodes.at(i));
        }
    }
}


void PathFinderQuadrantMap::addGeometryCollection(FGeometryCollection &collection){
    addNewNodes(collection.GetRawNodes());
    addConvexHulls(collection.GetRawConvexHulls());
    

}

//is not in use
void PathFinderQuadrantMap::addNewHulls(TArray<FMeshedPolygonHullIndices *> &polygons){
    for (int i = 0; i < polygons.Num(); i++){
        FMeshedPolygonHullIndices *currentPtr = polygons[i];
        if(currentPtr){
            addNewHull(*currentPtr);
        }
    }
}


//is not in use
/// - internally generates a new meshed polygon with rasterization
/// to chunk node grid size targeted!
//generates a new downsampled hull: positions must be in world space!
void PathFinderQuadrantMap::addNewHull(const FMeshedPolygonHullIndices &polygon){
    TArray<FVector> rawVectorData;
    polygon.GetData3D(rawVectorData);
    
    //is not garantiued to be convex!
    std::vector<PathFinderNode *> outNodes;
    for (int i = 0; i < rawVectorData.Num(); i++){
        PathFinderNode *n = new PathFinderNode(rawVectorData[i]);
        outNodes.push_back(n);
    }
    
    addAllNodes(outNodes);

    //generate a new downsampled polygon, rasterized.
    GenerateRasterizedConvexHull(rawVectorData);
}




void PathFinderQuadrantMap::addConvexHulls(TArray<TArray<FVector>> &vec){
    for (int i = 0; i < vec.Num(); i++){
        addConvexHull(vec[i]);
    }
}



/// @brief expects the vector to be a convex hull of an object / grounded nodes! Do not ignore!
/// @param vector vector of positions, convex hull!
void PathFinderQuadrantMap::addConvexHull(TArray<FVector> &vec){
    if(vec.Num() <= 0){
        return;
    }

    //create all nodes
    std::vector<PathFinderNode *> outNodes;
    for (int i = 0; i < vec.Num(); i++){
        PathFinderNode *n = new PathFinderNode(vec[i]);
        outNodes.push_back(n);
    }

    // add the konvex neighbors
    for (int i = 0; i < vec.Num(); i++)
    {
        PathFinderNode *prev = nullptr;
        PathFinderNode *next = nullptr;

        if (i == 0)
        {
            prev = outNodes.at(outNodes.size() - 1);
        }else{
            prev = outNodes.at(i - 1);
        }


        if(i == outNodes.size() - 1){
            next = outNodes.at(0);
        }
        else{
            next = outNodes.at(i + 1);
        }


        PathFinderNode *current = outNodes.at(i);
        if(prev != nullptr && current != nullptr && next != nullptr){
            //current->nA = prev;
            //current->nB = next;
            current->setConvexNeighborA(prev); //es wird davon ausgegangen das sich nodes auf der hülle sehen
            current->setConvexNeighborB(next);
        }

    }



    //alle sofort in graphen ballern
    addAllNodes(outNodes);
    GenerateRasterizedConvexHull(vec);
}


void PathFinderQuadrantMap::GenerateRasterizedConvexHull(TArray<FVector> &polygon){
    if(polygon.Num() <= 0){
        return;
    }

    if(parent){
        float step = PolygonStepSize();
        TArray<FVector> copy = polygon;

        FMeshedPolygon *meshedPolygon = new FMeshedPolygon();
        if(meshedPolygon){
            //meshedPolygon->Init(copy, step);

            //force mine size
            meshedPolygon->InitForceSizeMin(copy, step);

            //add shape if valid
            if(meshedPolygon->IsValid()){
                if(PathFinderQuadrant *quadrant = askforQuadrant(meshedPolygon->BottomLeft())){
                    quadrant->add(meshedPolygon);
                    return;
                }
            }
            //delete otherwise
            delete meshedPolygon;
            meshedPolygon = nullptr;
        }
        
    }
}

float PathFinderQuadrantMap::PolygonStepSize(){
    if(parent){
        float asFloat = parent->GetOneMeter();
        return asFloat;
    }
    return 100.0f;
}

///// POLYGON BITMAP GENERATION FOR CONVOLUTIONAL NN
void PathFinderQuadrantMap::GetSubGraphPolygonMesh(
    const FVector &center, 
    float sizeSquare,
    FMeshedPolygon &polygon
){
    sizeSquare = std::abs(sizeSquare);
    float halfSize = sizeSquare / 2.0f;
    FVector dir(halfSize, halfSize, 0.0f);

    FVector a = center - dir;
    FVector b = center + dir;

    return GetSubGraphPolygonMesh(a, b, polygon);
}

void PathFinderQuadrantMap::GetSubGraphPolygonMesh(
    const FVector &a, 
    const FVector &b,
    FMeshedPolygon &polygon
){
    //get all
    std::vector<FMeshedPolygon *> collected;
    TArray<PathFinderQuadrant *> all = allQuadrants();
    for (int i = 0; i < all.Num(); i++){
        if(PathFinderQuadrant *current = all[i]){
            std::vector<FMeshedPolygon *> collectedCurrent = current->getPolygonsInArea(a, b);
            if(collectedCurrent.size() > 0){
                collected.insert(collected.end(), collectedCurrent.begin(), collectedCurrent.end());
            }
        }
    }

    //join into single meshed polygon 2.5 D,
    //clamped against edges!
    float step = PolygonStepSize();
    //FMeshedPolygon polygon;
    polygon.GenerateFrom(collected, a, b, step);
}

TArray<PathFinderQuadrant *> PathFinderQuadrantMap::allQuadrants(){
    TArray<PathFinderQuadrant *> array = {TopRight, BottomRight, TopLeft, BottomLeft};
    return array;
}



std::vector<FMeshedPolygon *> PathFinderQuadrantMap::GetAllPolygons(){
    std::vector<FMeshedPolygon *> outVector;
    TArray<PathFinderQuadrant *> quadrants = allQuadrants();
    for (int i = 0; i < quadrants.Num(); i++){
        if(PathFinderQuadrant *current = quadrants[i]){
            current->AppendAllPolygons(outVector);
        }
    }

    return outVector;
}


void PathFinderQuadrantMap::addAllPolygons(std::vector<FMeshedPolygon *> &polygons){
    for (int i = 0; i < polygons.size(); i++){
        addPolygon(polygons[i]);
    }
}

void PathFinderQuadrantMap::addPolygon(FMeshedPolygon *polygon){
    if(polygon){
        //add shape.
        if(PathFinderQuadrant *quadrant = askforQuadrant(polygon->BottomLeft())){
            quadrant->add(polygon);
        }
    }
}
