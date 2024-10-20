
#include "roomBounds.h"
#include "p2/util/TTouple.h"
#include "CoreMinimal.h"



/// @brief copy constructor
/// @param other 
roomBounds::roomBounds(const roomBounds &other){ //std vec, etc braucht const beim copy constructor!
    *this = other;
}

roomBounds &roomBounds::operator=(const roomBounds &other){ //std vec, etc braucht const beim copy constructor!
    if(this == &other){ //no self assign
        return *this;
    }

    xPos = other.xPos;
    yPos = other.yPos;
    xScale = other.xScale;
    yScale = other.yScale;
    windowPositions = other.windowPositions;
    doorPositions = other.doorPositions;

    return *this;

    //nachbarschaftsliste wird NICHT übernommen!
    //KEIN PROPERTY VOM ROOM!
}

roomBounds::roomBounds(int xpos, int ypos, int xScaleIn, int yScaleIn, int num){
    xScale = xScaleIn;
    yScale = yScaleIn;
    xPos = xpos;
    yPos = ypos;
    uclass = nullptr;
    number = num;
}

/// @brief default constructor of room
/// @param xIn xscale
/// @param yIn yscale
/// @param num number
/// @param uclassIn uclass prefab
roomBounds::roomBounds(int xIn, int yIn, int num, UClass *uclassIn){
    xScale = xIn;
    yScale = yIn;
    number = num;
    type = roomtypeEnum::room;
    uclass = uclassIn;
    floor = 0;
}

/// @brief default constructor of room
/// @param xIn xscale
/// @param yIn yscale
/// @param num number
/// @param uclassIn uclass prefab
/// @param floorIn floor (0 or 1 for lower or upper floor)
/// @param typeIn to differentiate between rooms and staircases, very important for different door alignment!
roomBounds::roomBounds(
    int xIn, 
    int yIn, 
    int num, 
    UClass *uclassIn, 
    int floorIn,
    roomtypeEnum typeIn
){
    xScale = xIn;
    yScale = yIn;
    number = num;
    uclass = uclassIn;

    //new
    type = typeIn;
    floor = floorIn;
    floor = std::clamp(floorIn, 0, 1);
    if (floor < 0)
    {
        floor = 0;
    }
    if(floor > 0){
        floor = 1;
    }
}

roomBounds::~roomBounds(){
    //neighbors.clear();
    uclass = nullptr;
}

/// @brief returns the blueprint for this room
/// @return blueprint
UClass *roomBounds::readBp(){
    return uclass;
}

/// @brief update the blueprint if needed at later point than constructor
/// @param ucl 
void roomBounds::updateBp(UClass* ucl){
    if(ucl != nullptr){
        uclass = ucl;
    }
}

roomtypeEnum roomBounds::readType(){
    return type;
}

int roomBounds::xscale(){
    return xScale;
}
int roomBounds::yscale(){
    return yScale;
}
int roomBounds::xpos() { return xPos; }
int roomBounds::ypos() { return yPos; }

int roomBounds::xOuteredge(){
    return xPos + xScale;
}
int roomBounds::yOuteredge(){
    return yPos + yScale;
}

void roomBounds::updatePosition(int x, int y){
    xPos = x;
    yPos = y;
}
/// @brief adds a door position and makes it relative
/// @param x x pos in grid all
/// @param y y pos in grid all
void roomBounds::addDoorPosition(int x, int y){
    //make relative: to left corner!
    x -= xPos;
    y -= yPos;

    doorPositions.push_back(FVector(x, y, 0));
}


/// @brief adds a window position and makes it relative
/// @param x x pos in grid all
/// @param y y pos in grid all
void roomBounds::addWindowPosition(int x, int y){
    //make relative: to left corner!
    x -= xPos;
    y -= yPos;

    /*
    //check all door positions for overlap, dont add if found
    for (int i = 0; i < doorPositions.size(); i++){
        FVector &ref = doorPositions.at(i);
        if(ref.X == x && ref.Y == y){
            return;
        }
    }*/
    windowPositions.push_back(FVector(x, y, 0));
}

void roomBounds::addDoorPosition(FVector pos){
    pos.X -= xPos;
    pos.Y -= yPos;
    clampLocalPosition(pos);
    if(vecContains(pos, windowPositions) || vecContains(pos, doorPositions)){
        return;
    }
    doorPositions.push_back(pos);
}

void roomBounds::addWindowPosition(FVector pos){
    pos.X -= xPos;
    pos.Y -= yPos;
    clampLocalPosition(pos);
    if(vecContains(pos, windowPositions) || vecContains(pos, doorPositions)){
        return;
    }
    windowPositions.push_back(pos);
}


/// @brief expects position to be relative to bottom left corner (xpos and ypos)
/// position reference will be edited
/// @param pos position to clamp to the edges
void roomBounds::clampLocalPosition(FVector &pos){
    int xCopy = pos.X;
    int yCopy = pos.Y;
    xCopy = std::clamp(xCopy, 0, xScale);
    yCopy = std::clamp(yCopy, 0, yScale);
    pos.X = xCopy;
    pos.Y = yCopy;
}

bool roomBounds::vecContains(FVector &pos, std::vector<FVector> &vec){
    //check all door positions for overlap, dont add if found
    for (int i = 0; i < vec.size(); i++){
        FVector &ref = vec.at(i);
        if(ref.X == pos.X && ref.Y == pos.Y){
            return true;
        }
    }
    return false;
}

/// @brief will return the relative door INDEX positions, relative to the left bottom corner
/// the index scale must be scaled up to meters (*100) and calculated to the bottom left corner
/// of the room to get the correct door position
/// @return vector<FVector> &reference
std::vector<FVector> &roomBounds::readRelativeDoorPositions(){
    return doorPositions;
}

std::vector<FVector> &roomBounds::readRelativeWindowPositions(){
    return windowPositions;
}



// die überlegung ist dass die staircases
// die position selber übergeben
// es muss gepeichert werden ob oben oder unten

bool roomBounds::isStaircase(){
    return type == roomtypeEnum::staircase;
}

/// @brief will get the staircase positions
/// @return if return is -1,-1 an issue occured
TTouple<int,int> roomBounds::getmanualDoorPos(){
    if(isStaircase()){
        if(floor == 0){
            //bottom right pos (asked from right)
            TTouple<int, int> t(xPos + xScale, yPos); //bottom part
            return t;
        }
        if(floor == 1){
            //top right pos, asked from right
            TTouple<int, int> t(xPos + xScale, yPos + yScale); //top part
            return t;
        }


        //new side aligned positions
        if(floor == 0){
            if(xScale < yScale){
                //oriented upwards
                /**
                 * | |
                 * | |
                 * | |
                 */
                TTouple<int, int> t(xPos + xScale, yPos);
                return t;
            }else{
                /**
                 * -----
                 * -----
                 */
                TTouple<int, int> t(xPos + xScale, yPos);
                return t;

            }
        }


    }
    TTouple<int, int> none(-1, -1);
    return none;
}







/// @brief 
/// @return 
TTouple<int,int> roomBounds::getmanualDoorPosFromRight(){
    if(isStaircase()){

        if(floor == 0){
            if(xScale < yScale){
                //oriented upwards
                /**
                 * | |
                 * | |
                 * | |.-> YES bottom here
                 */
                /**
                * -----
                * -----.-> NO top here
                */
                TTouple<int, int> t(xPos + xScale, yPos + 2); //corrected little offset
                return t;
            }
        }
        if(floor == 1){
                
            //top right both
            /**
            * | |.-> YES top here
            * | |
            * | |
            */
                    
            /**
             * -----.-> YES top here
             * -----
            */
            TTouple<int, int> t(xPos + xScale, yPos + yScale - 1);
            return t;

        }


    }
    TTouple<int, int> none(-1, -1);
    return none;
}






/// @brief 
/// @return 
TTouple<int,int> roomBounds::getmanualDoorPosFromTop(){
    if(isStaircase()){
        
        if(floor == 0){
            if(xScale > yScale){
                //oriented upwards
                /**
                 * ^ NO top here
                 * .
                 * | |
                 * | |
                 * | |
                */

                /**
                 * ^ yes
                 * -----
                 * -----
                */
                TTouple<int, int> t(xPos + 1, yPos + yScale);
                return t;
            }
        }
        if(floor == 1){
                
            if(xScale < yScale){
                //oriented upwards
                /**
                 * ^ YES top here
                 * .
                 * | |
                 * | |
                 * | |
                 */
                TTouple<int, int> t(xPos + xScale, yPos);
                return t;
            }else{
                /**
                 *     ^ yes top here
                 * -----
                 * -----
                */
                TTouple<int, int> t(xPos + xScale - 1, yPos + yScale);
                return t;
            }
                
        }

    }
    TTouple<int, int> none(-1, -1);
    return none;
}






int roomBounds::xMax(){
    return xPos + xScale;
}

int roomBounds::yMax(){
    return yPos + yScale;
}


/**
 * 
 * 
 * CONNECT SECTION
 * 
 * 
 */

/// @brief will ignore double connection, will ignore rooms which dont touch each other
/// @param other 
void roomBounds::connectTo(roomBounds *other){
    if(other == nullptr){
        return;
    }

    //check if already connected, ignore if yes
    for (int i = 0; i < neighborRooms.size(); i++){
        if(neighborRooms.at(i) == other){
            return;
        }
    }

    // findShared axis

    //find connected bounds first
    int lowerY = std::max(yPos, other->yPos); //flipped on purpose!
    int higherY = std::min(yMax(), other->yMax());
    int lowerX = std::max(xPos, other->xPos); //flipped on purpose!    
    int higherX = std::min(xMax(), other->xMax());
    
    int Xmidpoint = (int) (lowerX + higherX) / 2.0f; //changed from float to int, damit es auf die meter geclamped ist
    int Ymidpoint = (int) (lowerY + higherY) / 2.0f;
    

    if (xMax() == other->xPos){
        //shared right y axis
        finishConnection(
            other,
            FVector(other->xPos, Ymidpoint, 0)
        );
        return;
    }

    if(yMax() == other->yPos){
        //shared upper x axis
        finishConnection(
            other,
            FVector(Xmidpoint, other->yPos, 0)
        );
        return;
    }


    if(xPos == other->xMax()){
        //shared left y axis
        finishConnection(
            other,
            FVector(xPos, Ymidpoint, 0)
        );
        return;
    }

    if(yPos == other->yMax()){
        //shared lower x axis
        finishConnection(
            other,
            FVector(Xmidpoint, yPos, 0)
        );
        return;
    }

    //add door in middle of both wall -->wall bounds!



}


void roomBounds::finishConnection(roomBounds *other, FVector doorPos){
    if(other == nullptr){
        return;
    }

    //pos offset wieder jeweils von door pos abziehen, dann zu doors adden
    FVector relativeDoorPosForThis = doorPos - FVector(xPos, yPos, 0);
    FVector relativeDoorPosForOther = doorPos - FVector(other->xPos, other->yPos, 0);
    
    
    //jeweils nachbar hinzufügen
    neighborRooms.push_back(other);
    other->neighborRooms.push_back(this);

    doorPositions.push_back(relativeDoorPosForThis);
    other->doorPositions.push_back(relativeDoorPosForOther);
    
}



/**
 * 
 * CONVERT SECTION
 * 
 */
std::vector<FVector> roomBounds::relativeDoorPositionsCm(){
    std::vector<FVector> vec;
    for (int i = 0; i < doorPositions.size(); i++){
        FVector copy = doorPositions[i] * 100;
        vec.push_back(copy);
    }
    return vec;
}



std::vector<FVector> roomBounds::relativeWindowPositionsCm(){
    std::vector<FVector> vec;
    for (int i = 0; i < windowPositions.size(); i++){
        FVector copy = windowPositions[i] * 100;
        vec.push_back(copy);
    }
    return vec;
}