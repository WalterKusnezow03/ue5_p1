// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/rooms/room.h"
#include "layoutCreator.h"
#include "roomBounds.h"
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


// ---- grid methods ----

layoutCreator::grid::grid(int x, int y){
    for (int i = 0; i < x; i++){
        TArray<roomBounds *> ar;
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
void layoutCreator::grid::fill(int fromX, int fromY, int toX, int toY, roomBounds *p){
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
void layoutCreator::grid::add(int x, int y, roomBounds *p){
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
bool layoutCreator::grid::findAndAdd(roomBounds *p){
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

/// @brief will try to find a free area in the desired size and output the indices to outX and outY by ref
/// @param xSize desired x size
/// @param ySize desired y size
/// @param outX out x pos will be saved here if true is returned
/// @param outY out y pos will be saved here if true is returned
/// @return will return true or false if area was found, out x and out y will be set if true
bool layoutCreator::grid::findFreeArea(int xSize, int ySize, int &outX, int &outY){
    for (int i = 0; i < data.Num(); i++){
        for (int j = 0; j < data[i].Num(); j++){
            if(isAreaFree(i,j, i + xSize, j + ySize)){
                outX = i;
                outY = j;
                return true; // found and filled
            }
        }
    }
    return false;
}

/// @brief force adds a room - designed for extending staircases
/// @param p staircase
void layoutCreator::grid::forceAdd(roomBounds *p){
    if(p != nullptr){
        int i = p->xpos();
        int j = p->ypos();
        int xSize = p->xscale();
        int ySize = p->yscale();
        fill(i, j, i + xSize, j + ySize, p); //fill map
        //p->updatePosition(i, j); // update position in grid
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
roomBounds *layoutCreator::grid::tryGetPosition(int x, int y){
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
    

    tryCreateRooms(); //NEW 
    connectNeighbors();
}

/// @brief main method. Creates rooms in a specified size and force adds all staircases
/// the passed 
/// @param x x grid size
/// @param y y grid size
/// @param staircases 
void layoutCreator::createRooms(int x, int y, std::vector<roomBounds> staircases){
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
            roomBounds current = staircases.at(i);
            roomBounds *copy = new roomBounds(
                current.xscale(),
                current.yscale(),
                current.number,
                current.readBp() //copy blueprint over
            );
            copy->updatePosition(current.xpos(), current.ypos());
            map->forceAdd(copy);
            created.push_back(copy); //added to both vectors!
            createdStaircases.push_back(copy);
        }
    }

    tryCreateRooms(); //NEW 
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
std::vector<roomBounds> layoutCreator::copyData(){
    std::vector<roomBounds> copy;
    if(created.size() > 0){
        for(roomBounds *r : created){
            if(r != nullptr){
                copy.push_back(*r);
            }
        }
    }
    return copy;
}

std::vector<roomBounds> layoutCreator::copyStaircaseData(){
    std::vector<roomBounds> copy;
    if(createdStaircases.size() > 0){
        for(roomBounds *r : createdStaircases){
            if(r != nullptr){
                copy.push_back(*r);
            }
        }
    }
    return copy;
}






/// @brief will print the map to the screen
void layoutCreator::debugPrintMap(){
    if(map != nullptr){
        FString s = map->toString();
        DebugHelper::showScreenMessage(s);
    }
    
}



// ---- new methods ----

void layoutCreator::tryCreateRooms(){

    int maxCount = 100;
    for (int i = 0; i < maxCount; i++)
    {
        if(staircasesLeft > 0){
        
            if(FVectorUtil::randomNumber(1,21) % 2 == 0){ //50% wahrscheinlichkeit ca
                bool success = createRoom(roomtypeEnum::staircase);
                if(success){
                    staircasesLeft--;
                }
            }
        }else{
            bool success = createRoom(roomtypeEnum::room); //nothing, ignore bool
        }
    }
}

/// @brief will try to create a room of a certain type
/// @param type type to create for example room, staircase ...
/// @return will return if the room was created or not / and to react if not, if wanted
bool layoutCreator::createRoom(roomtypeEnum type){

    if(manager != nullptr && map != nullptr){
        RoomManager::sizeData *someSize = manager->getAny(type);
        if(someSize != nullptr){
            int xsizeWanted = someSize->xSize();
            int ysizeWanted = someSize->ySize();
            UClass *bp = someSize->getBp();

            int outX = -1;
            int outY = -1;

            bool found = map->findFreeArea(xsizeWanted, ysizeWanted, outX, outY); //find free area and add later
            if (found && outX != -1 && outY != -1){

                roomBounds *s = new roomBounds(
                    xsizeWanted, 
                    ysizeWanted, 
                    number++, 
                    bp
                );
                
                
                s->updatePosition(outX, outY);
                map->forceAdd(s);
                created.push_back(s);
                if(type == roomtypeEnum::staircase){
                    createdStaircases.push_back(s);
                }

                DebugHelper::showScreenMessage("created room! ", FColor::Blue);
                return true;
            }
        }
    }
    return false;
}
// ---- new methods end ----


/// @brief will connect all rooms within the created list if aligned in the map
void layoutCreator::connectNeighbors(){
    //alle created durchgehen und die positionen abgreifen, dann in die map, 
    //dann x - 1 und y - 1 jeweils und verbinden 

    if(created.size() > 0){
        for (int i = 0; i < created.size(); i++){
            roomBounds *room = created.at(i);
            int x = room->xpos();
            int y = room->ypos();

            int xEdge = x + room->xscale();
            int yEgde = y + room->yscale();

            int lowerNeighbor = y - 1;
            int leftNeighbor = x - 1;

            //get and connect
            if(map->isValidIndex(x, lowerNeighbor)){
                roomBounds *lower = map->tryGetPosition(x, lowerNeighbor);
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
                roomBounds *left = map->tryGetPosition(leftNeighbor, y);
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