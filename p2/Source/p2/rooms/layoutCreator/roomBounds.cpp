
#include "roomBounds.h"
#include "p2/util/TTouple.h"
#include "CoreMinimal.h"



// ---- room methods ----

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

UClass *roomBounds::readBp(){
    return uclass;
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

/// @brief adds a window position and makes it relative
/// @param x x pos in grid all
/// @param y y pos in grid all
void roomBounds::addWindowPosition(int x, int y){
    //make relative: to left corner!
    x -= xPos;
    y -= yPos;

    //check all door positions for overlap, dont add if found
    for (int i = 0; i < doorPositions.size(); i++){
        FVector &ref = doorPositions.at(i);
        if(ref.X == x && ref.Y == y){
            return;
        }
    }

    windowPositions.push_back(FVector(x, y, 0));
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


