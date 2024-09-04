// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/rooms/room.h"
#include "p2/rooms/layoutCreator.h"
#include "p2/rooms/RoomManager.h"
#include "p2/rooms/roomtypeEnum.h"
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
    createdStaircases.clear(); //holds the same as created list, do not delete! double deletion is bad!
    for (int i = 0; i < created.size(); i++)
    {
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

layoutCreator::roomBounds::roomBounds(int xIn, int yIn, int num, roomtypeEnum typeIn){
    xScale = xIn;
    yScale = yIn;
    number = num;
    type = typeIn;
}

layoutCreator::roomBounds::~roomBounds(){
    //neighbors.clear();
}

roomtypeEnum layoutCreator::roomBounds::readType(){
    return type;
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
/// @param x x pos relative to left bottom corner
/// @param y y pos relative to left bottom corner
void layoutCreator::roomBounds::addDoorPosition(int x, int y){
    //make relative: to left corner!
    x -= xPos;
    y -= yPos;

    doorPositions.push_back(FVector(x, y, 0));
}

/// @brief will return the relative door INDEX positions, relative to the left bottom corner
/// the index scale must be scaled up to meters (*100) and calculated to the bottom left corner
/// of the room to get the correct door position
/// @return vector<FVector> &reference
std::vector<FVector> &layoutCreator::roomBounds::readRelativeDoorPositions(){
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

/// @brief adds a room to the grid an an given pos
/// @param x x pos index
/// @param y y pos index
/// @param p room
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
/// @param p room bounds to add with size
/// @return added or not
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

/// @brief force adds a room - designed for extending staircases
/// @param p staircase
void layoutCreator::grid::forceAdd(layoutCreator::roomBounds *p){
    if(p != nullptr){
        int i = p->xpos();
        int j = p->ypos();
        int xSize = p->xscale();
        int ySize = p->yscale();
        fill(i, j, i + xSize, j + ySize, p); //fill map
        p->updatePosition(i, j); // update position in grid
    }
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



// ---- layout creator main methods ----

/// @brief main method. Creates rooms in a specified size
/// @param x x grid size
/// @param y y grid size
void layoutCreator::createRooms(int x, int y, int staircases){
    if(x < 2){
        x = 2;
    }
    if(y < 2){
        y = 2;
    }

    //clean up
    clean();
    staircasesLeft = staircases;

    map = new layoutCreator::grid(x, y); //MAP SIZE HERE
    fillLayout();
    connectNeighbors();
}

/// @brief main method. Creates rooms in a specified size and force adds all staircases
/// the passed 
/// @param x x grid size
/// @param y y grid size
/// @param staircases 
void layoutCreator::createRooms(int x, int y, std::vector<layoutCreator::roomBounds> staircases){
    if(x < 2){
        x = 2;
    }
    if(y < 2){
        y = 2;
    }
    clean();
    staircasesLeft = 0;
    map = new layoutCreator::grid(x, y); //MAP SIZE HERE

    // manually block staircases / force add
    for (int i = 0; i < staircases.size(); i++){
        if(map != nullptr){
            //must create a whole copy of the room
            //ggf muss das aber nicht passieren da das layout eh gecleart wird 
            //wenn man die staircase daten von aussen als referenz hergibt
            layoutCreator::roomBounds current = staircases.at(i);
            layoutCreator::roomBounds *copy = new layoutCreator::roomBounds(
                current.xscale(),
                current.yscale(),
                current.number,
                current.readType()
            );
            copy->updatePosition(current.xpos(), current.ypos());
            map->forceAdd(copy);
            created.push_back(copy); //added to both vectors!
            createdStaircases.push_back(copy);
        }
    }

    //procecced with default layouting
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

/// @brief deep copy the created rooms, their index position and index scale
/// @return vector of roomBounds
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

std::vector<layoutCreator::roomBounds> layoutCreator::copyStaircaseData(){
    std::vector<layoutCreator::roomBounds> copy;
    if(createdStaircases.size() > 0){
        for(roomBounds *r : createdStaircases){
            if(r != nullptr){
                copy.push_back(*r);
            }
        }
    }
    return copy;
}






/// @brief will fill the map
void layoutCreator::fillLayout(){

    int rooms = 70;

    for(int i = 0; i < rooms; i++){
        int num = FVectorUtil::randomNumber(1, 10);
        createRoomStartingFromSize(num, num); // max size of a room to downscale / step down from
    }

    //debugPrintMap();
}

/// @brief will print the map to the screen
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
    
    //staircase adding
    if(staircasesLeft > 0){
        //if(2 < 3){
        if(FVectorUtil::randomNumber(1,10) % 2 == 0){ //50% wahrscheinlichkeit ca
            //ignore x,y
            //create staircase instead
            //debug:
            x = 4;
            y = 1;

            if(manager != nullptr){
                bool found = false;
                while (!found && x > 0 && y > 0)
                {
                    found = canCreate(x, y, roomtypeEnum::staircase);
                    if(found){

                        layoutCreator::roomBounds *stairs = new layoutCreator::roomBounds(
                            x, 
                            y, 
                            number++,
                            roomtypeEnum::staircase
                        );
                        if(map->findAndAdd(stairs)){
                            created.push_back(stairs);
                            createdStaircases.push_back(stairs);
                            staircasesLeft--;
                        }else{
                            delete stairs;
                        }

                        
                        return;
                    }
                    x--;
                    y--;
                }
            }
        }
    }


    //default room adding
    layoutCreator::roomBounds *room = testRoom(x, y);

    int a = 0;
    int maxAttempts = 1000;
    bool ok = false;
    while(!ok){

        ok = map->findAndAdd(room); //will look for a free spot and add the room
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

/// @brief will return whether a size can be created or not, may be refactured to have type too
/// @param x size meters
/// @param y size meters
/// @param typeIn room type to check
/// @return 
bool layoutCreator::canCreate(int x, int y, roomtypeEnum typeIn){
    //FString s("try");
    //DebugHelper::showScreenMessage(s);

    return manager != nullptr && manager->contains(x, y, typeIn);
}

/// @brief creates a new room to be checked to be created
/// @param x 
/// @param y 
/// @return 
layoutCreator::roomBounds* layoutCreator::testRoom(int x, int y){

    if(canCreate(x,y, roomtypeEnum::room)){
        layoutCreator::roomBounds *s = new layoutCreator::roomBounds(x, y, number++, roomtypeEnum::room);
        return s;
    }
    return nullptr;

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

                    if(xstarting < xending){
                        //calculate middle in all map scale
                        int xmiddle = (int)((xstarting + xending) / 2.0f);

                        //set door for both? (might have extra class naming gap or door)
                        lower->addDoorPosition(xmiddle, lower->yOuteredge()); //y max, x kante
                        room->addDoorPosition(xmiddle, y); //y = 0, x kante
                    }
                    
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

                    if(ystarting < yending){
                        //calculate middle in MAP SCALE MUST BE DOWNSCALED
                        int ymiddle = (int)((ystarting + yending) / 2.0f);

                        //set door for both? (might have extra class naming gap or door)
                        left->addDoorPosition(left->xOuteredge(), ymiddle); //x max, y kante
                        room->addDoorPosition(x, ymiddle); //x = 0, y kante
                    }
                    
                }
            }
        }
    }

}