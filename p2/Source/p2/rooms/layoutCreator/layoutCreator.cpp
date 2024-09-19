// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/rooms/room.h"
#include "layoutCreator.h"
#include "roomBounds.h"
#include "p2/rooms/RoomManager.h"
#include "p2/util/TTouple.h"
#include "p2/rooms/roomtypeEnum.h"
#include "p2/util/TVector.h"
#include <list>

class RoomManager;

roomBounds* layoutCreator::dummy = new roomBounds(1, 1, 1, nullptr);

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

/// @brief will check if an area is free - params must be ordered correctly:
/// @param x bottom left x
/// @param y bottom left y
/// @param x1 top right x
/// @param y1 top right y
/// @return is free or not, whole enclosed quad
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

/// @brief will tell if an area from 2 positions (enclosing a quad) has at least one neighbor
/// @param x bottom left x
/// @param y bottom left y
/// @param x1 top right x
/// @param y1 top right y
/// @return any neighbor found
bool layoutCreator::grid::areaHasAtLeastOneNeighbor(int x, int y, int x1, int y1){
    //first corner: return true, otherwise: issues
    //but outer must be valid too
    if(x == 0 && y == 0 && isValidIndex(x1, y1)){
        return true;
    }

    int count = 0;
    int least = 2; //new for checking more that those neigbors

    // all outer indices valid
    if(isValidIndex(x, y) && isValidIndex(x1, y1)){
        if(isValidIndex(x-1, y-1)){
            //vertical left
            for (int i = y; i < y1; i++){
                if(data[x-1][i] != nullptr && data[x-1][i] != dummy){ //not null AND NOT DUMMY TOO!
                    count++;
                    if(count >= least){
                        return true;
                    }
                }
            }
            //horizontal bottom
            for (int i = x; i < x1; i++){
                if(data[i][y-1] != nullptr && data[i][y-1] != dummy){
                    count++;
                    if(count >= least){
                        return true;
                    }
                }
            }
        }

        if(isValidIndex(x1 +1, y1 + 1)){
            //vertical right
            for (int i = y; i < y1; i++){
                if(data[x+1][i] != nullptr){
                    count++;
                    if(count >= least){
                        return true;
                    }
                }
            }
            //horizontal top
            for (int i = x; i < x1; i++){
                if(data[i][y+1] != nullptr){
                    count++;
                    if(count >= least){
                        return true;
                    }
                }
            }
        }
    
    
    }

    return false;
}
/// @brief will determine if the area is free and has at elast one neighbor
/// xy, x1y1 must be ordered correctly
/// @param x bottom left x
/// @param y bottom left y
/// @param x1 top right x
/// @param y1 top right y
/// @return return valid or not
bool layoutCreator::grid::areaIsValid(int x, int y, int x1, int y1){
    return isAreaFree(x, y, x1, y1) && areaHasAtLeastOneNeighbor(x, y, x1, y1);
}


/// @brief will try to find a free area in the desired size and output the indices to outX and outY by ref
/// ---- MAIN USED METHOD FOR ROOM CREATION ----
/// @param xSize desired x size
/// @param ySize desired y size
/// @param outX out x pos will be saved here if true is returned
/// @param outY out y pos will be saved here if true is returned
/// @return will return true or false if area was found, out x and out y will be set if true
bool layoutCreator::grid::findFreeArea(int xSize, int ySize, int &outX, int &outY){
    for (int i = 0; i < data.Num(); i++){
        for (int j = 0; j < data[i].Num(); j++){
            //if(isAreaFree(i,j, i + xSize, j + ySize)){
            if(areaIsValid(i,j, i + xSize, j + ySize)){
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

/// @brief shorthand force add, additional pos params
/// @param p 
/// @param x 
/// @param y 
void layoutCreator::grid::forceAdd(roomBounds *p, int x, int y){
    if(p != nullptr){
        fill(x, y, x + p->xscale(), y + p->yscale(), p);
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


/// @brief will find all edge rooms 
/// @param roomToCheck room to get the edges for 
/// @param output output TVector, data will be overriden!
void layoutCreator::grid::getEdges(
    roomBounds &roomToCheck, 
    //std::vector<TTouple<int,int>> &output,
    TVector<TTouple<int,int>> &output
){

    output.clear(); //clear template vector first

    //int overrideIndex = 0;

    //roomBounds *current = data[i][j];
    int xstart = roomToCheck.xpos();
    int ystart = roomToCheck.ypos() - 1;
    int xend = roomToCheck.xOuteredge();
    int yend = roomToCheck.yOuteredge();


    getValidWindowPositions(xstart, ystart - 1, xend, ystart - 1, output); //horizontal bottom
    getValidWindowPositions(xstart, yend + 1, xend, yend + 1, output); //horizontal top
    getValidWindowPositions(xstart - 1, ystart, xstart - 1, yend, output); //vertical left
    getValidWindowPositions(xend + 1, ystart, xend + 1, yend, output); //vertical right

}

/// @brief creates valid window positions
/// @param x1 x of a pos (already adjusted away from original index)
/// @param y1 y of a pos
/// @param x2 x of b pos
/// @param y2 y of b pos
/// @param output output to save in
void layoutCreator::grid::getValidWindowPositions(
    int x1, int y1, int x2, int y2, //offset must be applied as wanted
    TVector<TTouple<int,int>> &output
){
    //entweder x oder y bleiben gleich
    if(x1 == x2){
        //vertical
        if(x1 <= 0){
            //all valid to left
            for (int i = y1; i <= y2; i++){
                TTouple<int, int> a(x1 + 1, i);
                output.push_back(a);
            }
            return;
        }
        if(x1 >= data.Num() - 1){
            //all valid to right
            for (int i = y1; i <= y2; i++){
                TTouple<int, int> a(x1 - 1, i);
                output.push_back(a);
            }

            return;
        }
    }

    if(y1 == y2){
        //horizontal
        if(y1 <= 0){
            //all valid to bottom
            for (int i = x1; i <= x2; i++){
                TTouple<int, int> a(i, y1);
                output.push_back(a);
            }

            return;
        }
        if(y1 >= data[0].Num() - 1){
            //all valid to top
            for (int i = x1; i <= x2; i++){
                TTouple<int, int> a(i, y1);
                output.push_back(a);
            }

            return;
        }
    }
    return; //return here for safety

    //ISSUES WINODWS ON INSIDE FOR WHATEVER REASON!
    //default adding / real check in map 
    if(isAreaFree(x1,y1,x2,y2)){
        for (int i = x1; i <= x2; i++){
            for (int j = y1; j <= y2; j++){
                TTouple<int, int> a(i, j);
                output.push_back(a);
            }
        }
    }


}

/// @brief read the inverse area of the grid, includes null and dummy data
/// @param output ttouples stores here of x y indices
void layoutCreator::grid::fillInverseBlock(std::vector<TTouple<int, int>> &output){
    for (int i = 0; i < data.Num(); i++){
        for (int j = 0; j < data[i].Num(); j++){
            if(data[i][j] == nullptr || data[i][j] == layoutCreator::dummy){
                output.push_back(TTouple<int, int>(i, j));
            }
        }
    }
}

//NOT USED
/// @brief will find the area which is filled and not dummy data and not staircase
/// @param output output to save in
void layoutCreator::grid::fillEmptyGapsRoof(std::vector<TTouple<int, int>> &output){
    for (int i = 0; i < data.Num(); i++){
        for (int j = 0; j < data[i].Num(); j++){
            if(data[i][j] == nullptr){
                output.push_back(TTouple<int, int>(i, j));
            }
        }
    }
}




// ---- LAYOUT CREATOR MAIN METHODS ----

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
    createWindows();
}

/// @brief main method. Creates rooms in a specified size and force adds all staircases
/// the passed 
/// @param x x grid size
/// @param y y grid size
/// @param staircases 
/// @param leaveGap tells if stairs should be created or a empty gap leaved.
void layoutCreator::createRooms(int x, int y, std::vector<roomBounds> staircases, bool leaveGap){
    std::vector<TTouple<int, int>> empty;
    createRooms(x, y, staircases, leaveGap, empty);
}

void layoutCreator::createRooms(
    int x, 
    int y, 
    std::vector<roomBounds> staircases, 
    bool leaveGap, 
    std::vector<TTouple<int,int>> &block //inverse block area
){
    if(x < 5){
        x = 5;
    }
    if(y < 5){
        y = 5;
    }
    clean();
    //staircases will be always 0 here because the old ones are extended instead
    //and build on top of!
    staircasesLeft = 0; 

    map = new layoutCreator::grid(x, y); //MAP SIZE HERE

    //fill inverse blockdata 
    for (int i = 0; i < block.size(); i++)
    {
        TTouple<int, int> &current = block.at(i);
        int xcopy = current.first(); //copy position
        int ycopy = current.last();
        map->forceAdd(layoutCreator::dummy, xcopy, ycopy);
    }

    // manually block staircases / force add
    for (int i = 0; i < staircases.size(); i++){
        if(map != nullptr){
            //must create a whole copy of the room
            roomBounds current = staircases.at(i);

            //stairs or gap
            UClass *bp = current.readBp();
            if(leaveGap){
                bp = nullptr;
            }

            //create new room
            roomBounds *copy = new roomBounds(
                current.xscale(),
                current.yscale(),
                current.number,
                bp,
                1, //passed staircase, floor not 0, but 1 or any larger
                roomtypeEnum::staircase
            );
            

            copy->updatePosition(current.xpos(), current.ypos());
            map->forceAdd(copy);
            created.push_back(copy); //added to both vectors!
            createdStaircases.push_back(copy);
        }
    }

    tryCreateRooms(); //NEW 
    connectNeighbors();
    createWindows();
    
    

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
        if(staircasesLeft > 0 && i > 0){ //debug testing where first room never is a staircase

            int random = FVectorUtil::randomNumber(0,100);
            if(random <= 25){
                bool success = createRoom(roomtypeEnum::staircase);
                if(success){
                    staircasesLeft--;
                }
            }

        }else{
            bool success = createRoom(roomtypeEnum::room); //nothing, ignore bool
        }
    }

    //fill empty single gaps

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
                
                //new refactured
                roomBounds *s = nullptr;
                if (type == roomtypeEnum::staircase)
                {
                    s = new roomBounds(
                        xsizeWanted, 
                        ysizeWanted, 
                        number++, 
                        bp,
                        0, //floor 0 because only called here when not using passed stairs
                        roomtypeEnum::staircase
                    );
                }
                else
                {
                    //other rooms
                    s = new roomBounds(
                        xsizeWanted, 
                        ysizeWanted, 
                        number++, 
                        bp
                    );
                }
                
                s->updatePosition(outX, outY);
                map->forceAdd(s);
                created.push_back(s);
                if(type == roomtypeEnum::staircase){
                    createdStaircases.push_back(s);
                }

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
                if(lower != nullptr && lower != layoutCreator::dummy){ //dummy room avoided for reverse blocking 
                    
                    if(lower->isStaircase()){

                        //TTouple<int, int> doorTup = lower->getmanualDoorPos();
                        TTouple<int, int> doorTup = lower->getmanualDoorPosFromTop();
                        int xCopy = doorTup.first();
                        int yCopy = doorTup.last();
                        if(xCopy != -1 && yCopy != -1){//error code -1 cant align
                            lower->addDoorPosition(xCopy, yCopy); 
                            room->addDoorPosition(xCopy, yCopy + 1);
                        }
                        

                    }else{
                        int lowerxpos = lower->xpos(); //x pos lower
                        int lowerxscale = lower->xscale(); //x scale lower
                        int lowerxOuterEdge = lower->xOuteredge(); //x outer edge lower

                        //find overlapping position / range
                        int xstarting = (lowerxpos > x ? lowerxpos : x); //larger xpos along axis
                        int xending = (lowerxOuterEdge < xEdge ? lowerxOuterEdge : xEdge); // smaller xpos alogn axis

                        if(xstarting < xending){
                            //calculate middle in all map scale
                            //int xmiddle = (int)((xstarting + xending) / 2.0f);
                            int xmiddle = xstarting + 1; //testing +1 

                            //set door for both? (might have extra class naming gap or door)
                            lower->addDoorPosition(xmiddle, lower->yOuteredge()); //y max, x kante
                            room->addDoorPosition(xmiddle, y); //y = 0, x kante
                        }
                    }
                    
                }
            }

            if(map->isValidIndex(leftNeighbor, y)){
                roomBounds *left = map->tryGetPosition(leftNeighbor, y);
                if(left != nullptr && left != layoutCreator::dummy){

                    if(left->isStaircase()){

                        //TTouple<int, int> doorTup = left->getmanualDoorPos();
                        TTouple<int, int> doorTup = left->getmanualDoorPosFromRight();
                        int xCopy = doorTup.first();
                        int yCopy = doorTup.last();
                        if(xCopy != -1 && yCopy != -1){//error code -1 cant align
                            left->addDoorPosition(xCopy, yCopy); 
                            room->addDoorPosition(xCopy+1, yCopy);
                        }
                    }else{
                        //calculate door position
                        int leftypos = left->ypos(); //y pos lower
                        int leftyscale = left->yscale();
                        int leftyOuterEdge = left->yOuteredge(); //x outer edge lower
                    
                        int ystarting = (leftypos > y ? leftypos : y); //larger ypos along axis
                        int yending = (leftyOuterEdge < yEgde ? leftyOuterEdge : yEgde); // smaller xpos alogn axis

                        if(ystarting < yending){
                            //calculate middle in MAP SCALE MUST BE DOWNSCALED
                            int ymiddle = (int)((ystarting + yending) / 2.0f);
                            ymiddle = ystarting + 1; //testing +1 

                            //set door for both? (might have extra class naming gap or door)
                            left->addDoorPosition(left->xOuteredge(), ymiddle); //x max, y kante
                            room->addDoorPosition(x, ymiddle); //x = 0, y kante
                        }
                    }
                    
                }
            }
        }
    }

}



/// @brief will create windows at edges of room if no neighbor found and random windows removed / more natural
void layoutCreator::createWindows(){
    //am rand lang gehen und fenster hinzufügen, oder an null angrenzend

    //nur soviele hinzufügen wie max windows erlaubt?
    //am rand
    if(map){

        TVector<TTouple<int, int>> output;

        //std::vector<TTouple<int, int>> output;
        int size = 0;
        for (int i = 0; i < created.size(); i++)
        {
            //alle räume, alle einsammeln, hinzufügen
            roomBounds *current = created.at(i);
            if(current != nullptr){
                output.clear();
                map->getEdges(*current, output); // size will be overriden correctly

                
                removeRandomElements(output, 0.5f);
                //DebugHelper::showScreenMessage("room debug counted windows ", output.size(), FColor::Red);

                for (int j = 0; j < output.size(); j++)
                {
                    TTouple<int, int> &cTupel = output.at(j);
                    current->addWindowPosition(cTupel.first(), cTupel.last());
                }
            }

            
        }
    }
}

/// @brief will remove random elements from the tvector
/// @tparam T type
/// @param t tvector to clear
/// @param percentToKeep percentage to keep, for example 60% to have in the end left
/// percent <= 0 will be ignored, > 100 clamped to 100
template <typename T>
void layoutCreator::removeRandomElements(TVector<T> &t, float fractionToKeep)
{
    if(fractionToKeep <= 0){
        return;
    }
    if(fractionToKeep >= 1){
        return;
    }
    if(t.size() < 2){
        return;
    }

    int full = t.size();
    int percentToElements = fractionToKeep * full;
    if (percentToElements == 0)
    {
        return;
    }

    while(t.size() > percentToElements){
        int random = FVectorUtil::randomNumber(0, t.size() - 1);
        if(t.empty()){
            return;
        }
        t.erase(random);
    }
}

/// @brief gets a deep copy of the inverse blocked area, (to use for the next layer to be the same layout or less)
/// see overloaded create rooms method
/// @return reverse block area
std::vector<TTouple<int,int>> layoutCreator::getInverseBlockList(){
    std::vector<TTouple<int, int>> out;
    if(map){
        map->fillInverseBlock(out);
    }
    
    return out;
}



//need to get the empty roof data some how
std::vector<roomBounds> layoutCreator::getRoofToCreate(UClass *singleTile){
    std::vector<roomBounds> output;
    if(map != nullptr && singleTile != nullptr){
        std::vector<TTouple<int,int>> touples;
        map->fillEmptyGapsRoof(touples);

        for (int i = 0; i < touples.size(); i++){
            TTouple<int, int> &t = touples.at(i);
            roomBounds r(1,1, 0, singleTile);
            r.updatePosition(t.first(), t.last());
            output.push_back(r);
        }
    }
    return output;
}