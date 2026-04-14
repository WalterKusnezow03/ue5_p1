#include "PathFinderQuadrant.h"


/***
 * ---- QUADRANT METHODS ----
 */


PathFinderQuadrant::PathFinderQuadrant(int xSampleIn, int ySampleIn, APathFinder *parentIn){
    xSample = xSampleIn;
    ySample = ySampleIn;
    parent = parentIn;
}

PathFinderQuadrant::~PathFinderQuadrant(){
    for (int i = 0; i < map.size(); i++){
        for (int j = 0; j < map.at(i).size(); j++){
            PathFinderChunk *c = map.at(i).at(j);
            if(c != nullptr){
                delete (c);
                map.at(i).at(j) = nullptr;
            }
        }
    }
}

void PathFinderQuadrant::clear(){
    for (int i = 0; i < map.size(); i++){
        for (int j = 0; j < map.at(i).size(); j++){
            PathFinderChunk *c = map.at(i).at(j);
            if(c != nullptr){
                c->clear();
            }
        }
    }
}

/// @brief adds a node to the quadrant
/// @param n position to add
void PathFinderQuadrant::add(FVector n){
    int x = 0;
    int y = 0;
    if(MakeIndex(n, x, y)){
        fillMapTo(x, y);

        // Add the node to the appropriate chunk
        map[x][y]->add(n);
    }
}

bool PathFinderQuadrant::MakeIndex(const FVector &pos, int &x, int &y){
    if(parent){
        int chunksize = parent->GetChunkSize();
        //std::abs for flipping negatives obviosuly
        x = std::abs(pos.X / chunksize); //create new chunks?
        y = std::abs(pos.Y / chunksize);
        return true;
    }
    return false;
}

/// @brief adds a node to the quadrant (node node)
/// @param n position to add
void PathFinderQuadrant::add(PathFinderNode *n){
    if(n != nullptr && parent != nullptr){

        fillMapToPosition(n);

        int x = 0;
        int y = 0;
        if (MakeIndex(n->pos, x, y))
        {
            // Add the node to the appropriate chunk
            map[x][y]->add(n);
        }
    }   
}

void PathFinderQuadrant::add(FMeshedPolygon *polygon){
    if(polygon){
        FVector BottomLeft = polygon->BottomLeft();
        fillMapToPosition(BottomLeft);
        int x = 0;
        int y = 0;
        if (MakeIndex(BottomLeft, x, y))
        {
            map[x][y]->add(polygon);
        }
    }
}



void PathFinderQuadrant::addNoConnect(PathFinderNode *node){
    if(node && parent){
        fillMapToPosition(node);
        int x = 0;
        int y = 0;
        if (MakeIndex(node->pos, x, y))
        {
            // Add the node to the appropriate chunk
            map[x][y]->addNoConnect(node);
        }
    }
}

void PathFinderQuadrant::fillMapToPosition(PathFinderNode *node){
    if(node){
        fillMapToPosition(node->pos);
    }
}

void PathFinderQuadrant::fillMapToPosition(const FVector &pos){
    if(parent){
        int chunksize = parent->GetChunkSize();

        //std::abs for flipping negatives obviosuly
        int x = std::abs(pos.X / chunksize); //create new chunks?
        int y = std::abs(pos.Y / chunksize);
        
        fillMapTo(x, y);
    }
    
}

void PathFinderQuadrant::fillMapTo(int xIndex, int yIndex){
    
    if(parent){
        FCriticalSection &section = parent->GetDelegate_CriticalSectionA();
        FScopeLock Lock(&section); //new lock added from oath finder instance 
        while(map.size() <= xIndex) {
            map.push_back(std::vector<PathFinderChunk * >());
        }

        // Ensure all lists up to map.Count have enough chunks
        for (int i = 0; i < map.size(); i++) {
            while (map[i].size() <= yIndex) {
                map[i].push_back(new PathFinderChunk(parent));
            }
        }
    }
}




//finds a node from a quadrant
PathFinderNode* PathFinderQuadrant::findNode(FVector pos){
    if(parent){
        int chunksize = parent->GetChunkSize();
        int x1 = std::abs(pos.X / chunksize);
        int y1 = std::abs(pos.Y / chunksize);

        if(map.size() > x1 && map.at(x1).size() > y1){
            if(map.at(x1).at(y1) != nullptr){
                PathFinderNode* n = map.at(x1).at(y1)->findNode(pos);
                if(n != nullptr){
                    //DebugHelper::showScreenMessage("quadrant found node return!", FColor::Green);
                    return n;
                }
            }
        }
    }
    
    
        

    
    //DebugHelper::showScreenMessage("quadrant not found node return!", FColor::Red);
    return nullptr;
}



PathFinderNode *PathFinderQuadrant::findNodeInDirection(FVector &pos, FVector &dir){
    if(parent){
        int chunksize = parent->GetChunkSize();
        
        int x1 = std::abs(pos.X / chunksize);
        int y1 = std::abs(pos.Y / chunksize);

        if(map.size() > x1 && map.at(x1).size() > y1){
            if(map.at(x1).at(y1) != nullptr){
                PathFinderNode* n = map.at(x1).at(y1)->findNodeInDirection(pos, dir);
                if(n != nullptr){
                    //DebugHelper::showScreenMessage("quadrant found node return!", FColor::Green);
                    return n;
                }
            }
        }
    }
    
    
    
    //DebugHelper::showScreenMessage("quadrant not found node return!", FColor::Red);
    return nullptr;
}




/// @brief returns all lists from all chunks enclosed by an area
/// @param xA x value of first pos
/// @param yA z value of first pos
/// @param xB x value of second pos
/// @param zB z value of second pos
/// @return 
std::vector<PathFinderNode*> PathFinderQuadrant::nodesEnClosedBy(
    float xA, float yA, float xB, float yB
){
    std::vector<PathFinderNode *> nodes;
    TArray<PathFinderChunk *> chunksCollected = ChunksEnclosedBy(xA, yA, xB, yB);
    for (int i = 0; i < chunksCollected.Num(); i++){
        if(PathFinderChunk *currentChunk = chunksCollected[i]){
            std::vector<PathFinderNode *> &read = currentChunk->getNodes();
            if (read.size() > 0){
                nodes.insert(nodes.end(), read.begin(), read.end());
            }
        }
    }
    return nodes;
}

std::vector<FMeshedPolygon *> PathFinderQuadrant::polygonsEnClosedBy(
    float xA, float yA, float xB, float yB
){
    std::vector<FMeshedPolygon *> polygons;
    TArray<PathFinderChunk *> chunksCollected = ChunksEnclosedBy(xA, yA, xB, yB);
    for (int i = 0; i < chunksCollected.Num(); i++){
        if(PathFinderChunk *currentChunk = chunksCollected[i]){
            std::vector<FMeshedPolygon *> &read = currentChunk->getPolygons();
            if (read.size() > 0){
                polygons.insert(polygons.end(), read.begin(), read.end());
            }
        }
    }
    return polygons;
}

TArray<PathFinderChunk*> PathFinderQuadrant::ChunksEnclosedBy(
    float xA, float yA, float xB, float yB
){
    TArray<PathFinderChunk *> outChunks;
    if(parent){
        int chunksize = parent->GetChunkSize();
        //abs for flipping neg values for the quadrants
        int x1 = std::abs(xA / chunksize); //implicit conversion is allowed
        int y1 = std::abs(yA / chunksize);
        int x2 = std::abs(xB / chunksize); //implicit conversion is allowed
        int y2 = std::abs(yB / chunksize);

        int fromX = std::min(x1, x2);
        int toX = std::max(x1, x2);
        //toX = std::min(tg btoX, map.size());
        

        int fromY = std::min(y1, y2);
        int toY = std::max(y1, y2);
        //toY = std::min(toY, map.at(0).size());

        for(int i = fromX; i <= toX; i++){
            for(int j = fromY; j <= toY; j++){
                if(map.size() > i && map.at(i).size() > j){ //hier mit punkt weil call by value
                    if(PathFinderChunk *chunk = map.at(i).at(j)){
                        if(outChunks.Contains(chunk) == false){
                            outChunks.Add(chunk);
                        }
                    }
                }
            }
        }
    }
    return outChunks;
}

void PathFinderQuadrant::MakeCollectBoundingBoxClamped(
    const FVector &a, 
    const FVector &b,
    float &lowerX,
    float &lowerY,
    float &higherX,
    float &higherY
){
    //generate BottomLeft and TopRight
    lowerX = std::min(a.X, b.X); //-CHUNKSIZE; //+ extension
    lowerY = std::min(a.Y, b.Y); //- CHUNKSIZE;
    higherX = std::max(a.X, b.X); //+ CHUNKSIZE;
    higherY = std::max(a.Y, b.Y); //+ CHUNKSIZE;

    //extend if needed
    bool extendBounds = false;
    if(extendBounds && parent){
        int chunksize = parent->GetChunkSize();
        
        lowerX += - chunksize; //+ extension
        lowerY += - chunksize;
        higherX += chunksize;
        higherY += chunksize;
    }

    //clamp against axis regions
    float inf = std::numeric_limits<float>::infinity();
    if(xSample == 1 && ySample == 1){
        // Bottom-left quadrant
        lowerX = std::clamp(lowerX, 0.0f, inf); //(val, lowerclamp, higherclamp)
        lowerY = std::clamp(lowerY, 0.0f, inf);
        return;
    }
    if(xSample == -1 && ySample == -1){
        // Top-right quadrant
        higherX = std::clamp(higherX, -inf, 0.0f); 
        higherY = std::clamp(higherY, -inf, 0.0f);
        return;
    }
    if(xSample == -1 && ySample == 1){
        // Bottom-right quadrant
        higherX = std::clamp(higherX, -inf, 0.0f);
        lowerY = std::clamp(lowerY, 0.0f, inf);
        return;
    }
    if(xSample == 1 && ySample == -1){
        // Top-left quadrant
        lowerX = std::clamp(lowerX, 0.0f, inf);
        higherY = std::clamp(higherY, -inf, 0.0f);
        return;
    }
}

std::vector<PathFinderNode*> PathFinderQuadrant::askForArea(FVector a, FVector b){
    float lowerX = 0;
    float lowerY = 0;
    float higherX = 0;
    float higherY = 0;
    MakeCollectBoundingBoxClamped(a, b, lowerX, lowerY, higherX, higherY);
    return nodesEnClosedBy(lowerX, lowerY, higherX, higherY);
}

std::vector<FMeshedPolygon *> PathFinderQuadrant::getPolygonsInArea(FVector a, FVector b){
    float lowerX = 0;
    float lowerY = 0;
    float higherX = 0;
    float higherY = 0;
    MakeCollectBoundingBoxClamped(a, b, lowerX, lowerY, higherX, higherY);
    return polygonsEnClosedBy(lowerX, lowerY, higherX, higherY);
}

void PathFinderQuadrant::debugShowAllNodes(UWorld *world){
    for (int i = 0; i < map.size(); i++){
        std::vector<PathFinderChunk *> &current = map[i];
        for (int j = 0; j < current.size(); j++){
            PathFinderChunk *currentChunk = current[j];
            if(currentChunk){
                currentChunk->debugShowAllNodes(world);
            }
        }
    }
}

int PathFinderQuadrant::chunkCount(){
    int count = 0;
    for (int i = 0; i < map.size(); i++)
    {
        std::vector<PathFinderChunk *> &current = map[i];
        for (int j = 0; j < current.size(); j++){
            PathFinderChunk *currentChunk = current[j];
            if(currentChunk){
                count++;
            }
        }
    }
    return count;
}


void PathFinderQuadrant::AppendAllPolygons(std::vector<FMeshedPolygon *> &polygonsOut){
    for (int i = 0; i < map.size(); i++)
    {
        std::vector<PathFinderChunk *> &current = map[i];
        for (int j = 0; j < current.size(); j++){
            PathFinderChunk *currentChunk = current[j];
            if(currentChunk){
                currentChunk->AppendAllPolygons(polygonsOut);
            }
        }
    }
}