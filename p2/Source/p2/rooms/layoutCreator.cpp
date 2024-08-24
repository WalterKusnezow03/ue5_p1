// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/rooms/room.h"
#include "p2/rooms/layoutCreator.h"
#include "p2/entityManager/RoomManager.h"
#include <list>

class RoomManager;

layoutCreator::layoutCreator(RoomManager *m)
{
    map = nullptr;
    number = 0;

    manager = m;
}

layoutCreator::~layoutCreator()
{
    for (int i = 0; i < created.size(); i++){
        if(created.at(i) != nullptr){
            delete created.at(i);
            created.at(i) = nullptr;
        }
    }
    created.clear();
    
    if (map != nullptr)
    {
        delete map;
    }
    worldPointer = nullptr;
}

// ---- room methods ----

layoutCreator::roomBounds::roomBounds(int xIn, int yIn, int num){
    xScale = xIn;
    yScale = yIn;
    number = num;
}

layoutCreator::roomBounds::~roomBounds(){
    //neighbors.clear();
}

int layoutCreator::roomBounds::xscale(){
    return xScale;
}
int layoutCreator::roomBounds::yscale(){
    return yScale;
}
int layoutCreator::roomBounds::xpos() { return xPos; }
int layoutCreator::roomBounds::ypos() { return yPos; }
int layoutCreator::roomBounds::xOuteredge(){
    return xPos + xScale;
}
int layoutCreator::roomBounds::yOuteredge(){
    return yPos + yScale;
}

void layoutCreator::roomBounds::updatePosition(int x, int y){
    xPos = x;
    yPos = y;
}
/// @brief adds a door position 
/// @param x x pos
/// @param y y pos
void layoutCreator::roomBounds::addDoorPosition(int x, int y){
    doorPositions.push_back(FVector(x, y, 0));
}

std::vector<FVector> &layoutCreator::roomBounds::readDoorPositions(){
    return doorPositions;
}

// ---- grid methods ----

layoutCreator::grid::grid(int x, int y){
    for (int i = 0; i < x; i++){
        TArray<layoutCreator::roomBounds *> ar;
        for (int j = 0; j < y; j++){
            ar.Add(nullptr);
        }
        data.Add(ar);
    }

    latestX = 0;
    latestY = 0;
}
layoutCreator::grid::~grid(){

    //clean the map and pointers, but dont delete, DOUBLE DELETION COULD HAPPEN ON LARGER TILES
    //DOuBLE DELETiON LEADS TO UNDEFINED BEHAIVIOUR!
    for(int i = 0; i < data.Num(); i++){
        for (int j = 0; j < data[i].Num(); j++){
            data[i][j] = nullptr;
        }
        data[i].Empty();
    }
    data.Empty();
}

/// @brief will fill an area without questioning from x to x from y to y if indices are valid
/// @param fromX 
/// @param fromY 
/// @param toX 
/// @param toY 
/// @param p to fill
void layoutCreator::grid::fill(int fromX, int fromY, int toX, int toY, layoutCreator::roomBounds *p){
    if(isValidIndex(fromX, fromY) && isValidIndex(toX, toY)){
        for (int i = fromX; i < toX; i++){
            for (int j = fromY; j < toY; j++){
                add(i, j, p);
            }
        }
    }
}

void layoutCreator::grid::add(int x, int y, layoutCreator::roomBounds *p){
    if(isValidIndex(x,y)){
        data[x][y] = p;
    }
}
bool layoutCreator::grid::isFree(int x, int y){
    if(isValidIndex(x,y)){
        return data[x][y] == nullptr;
    }
    return false;
}

/// @brief will return whether the given coordinates are within the 2D bounds
/// @param x pos in
/// @param y pos in
/// @return true or false, bounds or not
bool layoutCreator::grid::isValidIndex(int x, int y){
    if(x >= 0 && y >= 0 && x < data.Num() && y < data[x].Num()){
        return true;
    }
    return false;
}

bool layoutCreator::grid::isAreaFree(int x, int y, int x1, int y1){
    if(isValidIndex(x,y) && isValidIndex(x1, y1)){
        for(int i = x; i < x1; i++){
            for(int j = y; j < y1; j++){
                if(!isFree(i,j)){
                    return false;
                }
            }
        }
        return true; //area is all free
    }
    return false;
}

/// @brief will try to find a position and add the room, returns true on success, false on failure
/// @param x 
/// @param y 
/// @param p 
/// @return 
bool layoutCreator::grid::findAndAdd(layoutCreator::roomBounds *p){
    if(p != nullptr){
        int xSize = p->xscale();
        int ySize = p->yscale();

        for (int i = 0; i < data.Num(); i++){
            for (int j = 0; j < data[i].Num(); j++){
                if(isAreaFree(i,j, i + xSize, j + ySize)){
                    fill(i, j, i + xSize, j + ySize, p); //fill map

                    p->updatePosition(i, j); // update position in grid

                    return true; //found and filled
                }
            }
        }
        return false;
    }
    return false;
}
FString layoutCreator::grid::toString(){
    FString a = TEXT("");
    for (int i = 0; i < data.Num(); i++){
        FString s = TEXT("");
        for (int j = 0; j < data[i].Num(); j++){
            if(data[i][j] != nullptr){
                FString b = FString::Printf(TEXT("%d"), data[i][j]->number);
                s.Append(b);
            }else{
                FString b = FString::Printf(TEXT("-"));
                s.Append(b);
            }
        }
        a.Append(s);
        a.Append(TEXT("\n"));
    }
    return a;
}

/// @brief returns the room at a specified position if not nullptr, else nullptr
/// @param x 
/// @param y 
/// @return 
layoutCreator::roomBounds *layoutCreator::grid::tryGetPosition(int x, int y){
    if(isValidIndex(x,y)){
        return data[x][y];
    }
    return nullptr;
}

// ---- layout creator methods ----


/// @brief main method. Creates rooms in a specified size
/// @param x x grid size
/// @param y y grid size
void layoutCreator::createRooms(int x, int y){
    if(x < 2){
        x = 2;
    }
    if(y < 2){
        y = 2;
    }

    //clean up
    clean();

    map = new layoutCreator::grid(10, 10);
    fillLayout();
    connectNeighbors();
}


/// @brief aquivalent to destructor, will clean up the map and created list (delete all room bounds)
void layoutCreator::clean(){
    if(created.size() > 0){
        for (int i = 0; i < created.size(); i++){
            if(created.at(i) != nullptr){
                delete created.at(i);
                created.at(i) = nullptr;
            }
        }
        created.clear();
    }
    if (map != nullptr)
    {
        delete map;
        map = nullptr;
    }
}


std::vector<layoutCreator::roomBounds> layoutCreator::copyData(){
    std::vector<layoutCreator::roomBounds> copy;
    if(created.size() > 0){
        for(roomBounds *r : created){
            if(r != nullptr){
                copy.push_back(*r);
            }
        }
    }
    return copy;
}

/// @brief will fill the map
void layoutCreator::fillLayout(){

    int rooms = 50;

    for(int i = 0; i < rooms; i++){
        createRoomStartingFromSize(4, 4);
    }

    debugPrintMap();
}

void layoutCreator::debugPrintMap(){
    if(map != nullptr){
        FString s = map->toString();
        DebugHelper::showScreenMessage(s);
    }
    
}

/// @brief tries to create a room from a starting size
/// will only work if the Room manager can privide the targeted size
/// @param x 
/// @param y 
void layoutCreator::createRoomStartingFromSize(int x, int y){
    
    
    layoutCreator::roomBounds *room = testRoom(x, y);

    int a = 0;
    int maxAttempts = 1000;
    bool ok = false;
    while(!ok){

        ok = map->findAndAdd(room);
        if(ok){
            created.push_back(room);
            return;
        }else{
            //not okay
            delete room;
            room = nullptr;
            x--;
            y--;

            //to small
            if(x <= 0 || y <= 0){
                return;
            }

            //try new room
            room = testRoom(x,y);
        }

        //attempts limit
        a++;
        if(a > maxAttempts){
            return;
        }
    }
}

/// @brief will return whether a size can be created or not
/// @param x 
/// @param y 
/// @return 
bool layoutCreator::canCreate(int x, int y){
    FString s("try");
    DebugHelper::showScreenMessage(s);

    return manager != nullptr && manager->contains(x, y);
}

layoutCreator::roomBounds* layoutCreator::testRoom(int x, int y){

    if(canCreate(x,y)){
        layoutCreator::roomBounds *s = new layoutCreator::roomBounds(x, y, number++);
        return s;
    }
    return nullptr;

    //layoutCreator::roomBounds *s = new layoutCreator::roomBounds(x, y, number++);
    //return s;
}





/// @brief will connect all rooms within the created list if aligned in the map
void layoutCreator::connectNeighbors(){
    //alle created durchgehen und die positionen abgreifen, dann in die map, 
    //dann x - 1 und y - 1 jeweils und verbinden 

    if(created.size() > 0){
        for (int i = 0; i < created.size(); i++){
            layoutCreator::roomBounds *room = created.at(i);
            int x = room->xpos();
            int y = room->ypos();

            int xEdge = x + room->xscale();
            int yEgde = y + room->yscale();

            int lowerNeighbor = y - 1;
            int leftNeighbor = x - 1;

            //get and connect
            if(map->isValidIndex(x, lowerNeighbor)){
                layoutCreator::roomBounds *lower = map->tryGetPosition(x, lowerNeighbor);
                if(lower != nullptr){
                    
                    int lowerxpos = lower->xpos(); //x pos lower
                    int lowerxscale = lower->xscale(); //x scale lower
                    int lowerxOuterEdge = lower->xOuteredge(); //x outer edge lower

                    //find overlapping position / range
                    int xstarting = (lowerxpos > x ? lowerxpos : x); //larger xpos along axis
                    int xending = (lowerxOuterEdge < xEdge ? lowerxOuterEdge : xEdge); // smaller xpos alogn axis

                    //calculate middle
                    int xmiddle = (int)((xstarting + xending) / 2);

                    //set door for both? (might have extra class naming gap or door)
                    lower->addDoorPosition(xmiddle, lower->yOuteredge()); //y max, x kante
                    room->addDoorPosition(xmiddle, y); //y = 0, x kante
                }
            }

            if(map->isValidIndex(leftNeighbor, y)){
                layoutCreator::roomBounds *left = map->tryGetPosition(leftNeighbor, y);
                if(left != nullptr){

                    //calculate door position
                    int leftypos = left->ypos(); //y pos lower
                    int leftyscale = left->yscale();
                    int leftyOuterEdge = left->yOuteredge(); //x outer edge lower
                
                    int ystarting = (leftypos > y ? leftypos : y); //larger ypos along axis
                    int yending = (leftyOuterEdge < yEgde ? leftyOuterEdge : yEgde); // smaller xpos alogn axis

                    //calculate middle
                    int ymiddle = (int)((ystarting + yending) / 2);
                    
                    //set door for both? (might have extra class naming gap or door)
                    left->addDoorPosition(left->xOuteredge(), ymiddle); //x max, y kante
                    room->addDoorPosition(x, ymiddle); //x = 0, y kante
                }
            }
        }
    }

}