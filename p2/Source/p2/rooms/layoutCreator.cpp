// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/rooms/layoutCreator.h"

layoutCreator::layoutCreator()
{
}

layoutCreator::~layoutCreator()
{
}

layoutCreator::roomBounds::roomBounds(int xIn, int yIn){
    x = xIn;
    y = yIn;
}
layoutCreator::roomBounds::~roomBounds(){
    
}

int layoutCreator::roomBounds::xValue(){
    return x;
}
int layoutCreator::roomBounds::yValue(){
    return y;
}


std::vector<std::vector<int>> layoutCreator::generateLayout(int sizeX, int sizeY){
    sizeX = (sizeX > 1 ? sizeX : 1);
    sizeY = (sizeY > 1 ? sizeY : 1);

    std::vector<std::vector<int>> map;
    //create
    for (int x = 0; x < sizeX; x++){
        std::vector<int> inner;
        for (int y = 0; y < sizeY; y++){
            inner.push_back(0);
        }
        map.push_back(inner);
    }

    

    // fill
    int roomCount = 10;
    int xstart = 0;
    int ystart = 0;
    for (int i = 0; i < roomCount; i++)
    {
        addRoom(map, xstart, ystart, i);
    }

    //debug show

    FString s = TEXT("map \n");
    for (int i = 0; i < map.size(); i++){
        FString a = TEXT("");
        for (int j = 0; j < map.at(i).size(); j++)
        {
            a.Append(FString::Printf(TEXT(". %d"), map.at(i).at(j)));
        }
        s.Append(a);
        s.Append("\n");
    }
    DebugHelper::showScreenMessage(s);

    return map;
}


void layoutCreator::addRoom(std::vector<std::vector<int>> &map, int &nextX, int &nextY, int roomNum){ 
    
    //find next free place
    while (nextX < map.size() && nextY < map.at(0).size() && !isFree(map.at(nextX).at(nextY))) {
        

        nextX += 1;
        if (nextX >= map.size() - 1) {
            nextX = 0;
            nextY += 1;
        }
    }

    if (nextX >= map.size() && nextY >= map.at(0).size()) {
        return; // No free space left
    }

    
    //x and y must be found previously where a room is not blocked / added
    //or goes out of bounds targeted

    
    int upperX = 4;
    int upperY = 4;
    upperX = (nextX + upperX < map.size() ? upperX : map.size() - nextX);
    upperY = (nextY + upperX < map.at(0).size() ? upperY : map.size() - nextY);

    int maxAttempts = 100;
    int attempts = 0;

    while(true){
        attempts++;
        if(attempts > maxAttempts){
            return;
        }

        // invalid size, shouldnt be the case at any time
        if(upperX <= 0 || upperY <= 0){
            return;
        }

        layoutCreator::roomBounds s = pickRoom(upperX,upperY); //mus be made by choosing random from rooms which exist somewhere else, DO LATER!

        int outerX = nextX + s.xValue();
        int outerY = nextY + s.yValue();

        if (
            outerX < map.size() && outerY < map.at(0).size()
        ){
            bool xBlocked = !isFree(map.at(outerX).at(nextY)); //beide verlängern, ja.
            bool yBlocked = !isFree(map.at(nextX).at(outerY));

            if(xBlocked){
                upperX--;
            }
            if(yBlocked){
                upperY--;
            }

            //valid area
            if(!xBlocked && !yBlocked){
                //block area, valid area found
                blockArea(map, nextX, nextY, outerX, outerY, roomNum);

                //update next index
                /* //happens at the top automatically
                if(outerX == map.size()){
                    nextY = nextY + 1; // outerY;
                    nextX = 0;
                }else{
                    nextX = outerX; //x update only
                }*/

                return;
            }
        }
    }
}

bool layoutCreator::isFree(int a){
    return a == 0;
}


void layoutCreator::blockArea(
    std::vector<std::vector<int>> &map, 
    int x, 
    int y, 
    int xSize, 
    int ySize,
    int roomNum
){
    for(int i = x; i < xSize; i++){
        for(int j = y; j < ySize; j++){
            if(i < map.size() && j < map.at(i).size()){
                map.at(i).at(j) = roomNum;
            }
        }
    }
}



layoutCreator::roomBounds layoutCreator::pickRoom(int xMax, int yMax){
    //mus be made by choosing random from rooms which exist somewhere else, DO LATER!
    layoutCreator::roomBounds s(xMax, yMax);
    return s;
}