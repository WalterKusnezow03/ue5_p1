
#include "roomBounds.h"
#include "CoreMinimal.h"



// ---- room methods ----

roomBounds::roomBounds(int xIn, int yIn, int num, roomtypeEnum typeIn){
    xScale = xIn;
    yScale = yIn;
    number = num;
    type = typeIn;
    uclass = nullptr;
}
roomBounds::roomBounds(int xIn, int yIn, int num, UClass *uclassIn){
    xScale = xIn;
    yScale = yIn;
    number = num;
    type = roomtypeEnum::room;
    uclass = uclassIn;
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
/// @brief adds a door position 
/// @param x x pos relative to left bottom corner
/// @param y y pos relative to left bottom corner
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

void roomBounds::addWindowPosition(int x, int y){
    //make relative: to left corner!
    x -= xPos;
    y -= yPos;

    windowPositions.push_back(FVector(x, y, 0));
}