// Fill out your copyright notice in the Description page of Project Settings.


#include <algorithm>
#include "p2/rooms/testing/roomBoundData.h"

roomBoundData::roomBoundData(int xin, int yin, int xscalein, int yscalein, int numberIn)
{
    x = xin;
    y = yin;
    xscale = xscalein;
    yscale = yscalein;
    number = numberIn;
}

roomBoundData::~roomBoundData()
{
}

int roomBoundData::xpos(){
    return x;
}
int roomBoundData::ypos(){
    return y;
}

FVector roomBoundData::positionInMeterSpace(int onemeter){
    return FVector(
        xpos() * onemeter,
        ypos() * onemeter,
        0
    );
}

int roomBoundData::xScale(){
    return xscale;
}
int roomBoundData::yScale(){
    return yscale;
}

int roomBoundData::xMax(){
    return x + xscale;
}
int roomBoundData::yMax(){
    return y + yscale;
}

void roomBoundData::connect(roomBoundData &other){

    //überlappende kante finden

    //brauche bool für doOverlap
    //brauche methode für finden der overlap zahlen
    //finde mitte
    //brauche für den porzess noch achsen unterscheidung

    if(hasNeighbor(&other)){
        return;
    }

        //für x, für y xy tauschen
    //entweder bei xmax und ymax ein abziehen 
    defineOverlap(
        other,
        other.xpos(),
        other.ypos(),
        other.xMax() -1,
        other.yMax() -1,
        xpos(),
        ypos(),
        xMax() -1,
        yMax() -1,
        false
    );

    defineOverlap(
        other,
        other.ypos(),
        other.xpos(),
        other.yMax() -1,
        other.xMax() -1,
        ypos(),
        xpos(),
        yMax() -1,
        xMax() -1,
        true
    );
}

void roomBoundData::defineOverlap(
    roomBoundData &other,
    int otherX,
    int otherY,
    int otherXmax,
    int otherYmax,
    int thisX,
    int thisY,
    int thisXmax,
    int thisYmax,
    bool xyFlipped
)
{

    bool flip = otherX < thisXmax;

    if (oneApart(thisXmax, otherX, flip) || oneApart(otherXmax, thisX, flip))
    {
        //vertical overlap
        int a = -1;
        int b = -1;
        // should be correct, one overlap of 2 possible
        a = std::max(thisY, otherY);
        b = std::min(otherYmax, thisYmax);
        if(b < a){ //no overlap
            return;
        }

        if(a != -1 && b != -1){
            int middle = (a + b) / 2.0f;

            
            if(flip){
                if(!xyFlipped){
                    //using only the larger coordinate works better with clamping
                    addDoorPosition(thisX, middle);
                    other.addDoorPosition(thisX, middle);
                }
                else
                {
                    addDoorPosition(middle, thisX);
                    other.addDoorPosition(middle, thisX);
                }
            }else{
                if(!xyFlipped){
                    addDoorPosition(otherX, middle);
                    other.addDoorPosition(otherX, middle);
                }else{
                    addDoorPosition(middle, otherX);
                    other.addDoorPosition(middle, otherX);
                }
            }

            //add neighbors each
            connectedNeighbors.push_back(&other);
            other.connectedNeighbors.push_back(this);

        }

        //oneApart(thisXmax, otherX, flip) || oneApart(otherXmax, thisX, flip)
        FString printing = FString::Printf(
            TEXT("neighbor found a %d, a1 %d OR b %d b1 %d"),
            thisXmax,
            otherX,
            otherXmax,
            thisX
        );
        DebugHelper::logMessage(printing);

        return;
    }else{
        //oneApart(thisXmax, otherX, flip) || oneApart(otherXmax, thisX, flip)
        FString printing = FString::Printf(
            TEXT("neighbor not found a %d, a1 %d OR b %d b1 %d"),
            thisXmax,
            otherX,
            otherXmax,
            thisX
        );
        //DebugHelper::logMessage(printing);
    }
}

bool roomBoundData::oneApart(int a, int b, bool &flip){
    /*if(order.size() < 2){
        return false;
    }*/
    if(a == b - 1){
        return true;
    }
    if(b == a - 1){ //b ist also grösser, x
        //flip = true;
        return true;
    }
    return false;

    //return a == b - 1 || b == a - 1;
}




/// @brief adds a window poisiton at x and y in global space
/// @param xIn x global
/// @param yIn y global
void roomBoundData::addWindowPosition(int xIn, int yIn){
    //make local
    int xLocal = xIn - x;
    int yLocal = yIn - y;

    //clamp?
    FVector pos(xLocal, yLocal, 0);
    clamp(pos);

    if(!contains(windowPositions, pos)){    
        windowPositions.push_back(pos);
    }
}

/// @brief adds a door poisiton at x and y in global space
/// @param xIn x global
/// @param yIn y global
void roomBoundData::addDoorPosition(int xIn, int yIn){
    //make local
    int xLocal = xIn - x;
    int yLocal = yIn - y;

    //clamp?
    FVector doorpos(xLocal, yLocal, 0);
    //clamp(doorpos);

    if(!contains(doorPositions, doorpos)){    
        doorPositions.push_back(doorpos);
    }
}

bool roomBoundData::contains(std::vector<FVector> &other, int xin, int yin){
    return contains(other, FVector(xin, yin, 0));
}

bool roomBoundData::contains(std::vector<FVector> &other, FVector pos){
    pos.X = (int)pos.X;
    pos.Y = (int)pos.Y;

    for (int i = 0; i < other.size(); i++){
        FVector &current = other.at(i);
        if((int)current.X == pos.X && (int)current.Y == pos.Y){
            return true;
        }
    }
    return false;
}

void roomBoundData::clamp(FVector &other){
    
    int xLocal = other.X;
    int yLocal = other.Y;

    xLocal = std::max(0, xLocal);
    yLocal = std::max(0, yLocal);
    xLocal = std::min(xscale, xLocal);
    yLocal = std::min(yscale, yLocal);

    other.X = xLocal;
    other.Y = yLocal;
}

std::vector<FVector> roomBoundData::relativeDoorPositionsCm(){
    std::vector<FVector> output;
    for (int i = 0; i < doorPositions.size(); i++){
        FVector copy = doorPositions.at(i);
        output.push_back(copy * 100);
    }
    return output;
}

std::vector<FVector> roomBoundData::relativeWindowPositionsCm(){
    std::vector<FVector> output;
    for (int i = 0; i < windowPositions.size(); i++){
        FVector copy = windowPositions.at(i);
        output.push_back(copy * 100);
    }
    return output;
}







bool roomBoundData::hasNeighbor(roomBoundData *other){
    if(other == nullptr){
        return false;
    }
    for (int i = 0; i < connectedNeighbors.size(); i++){
        if(other == connectedNeighbors[i]){
            return true;
        }
    }
    return false;
}