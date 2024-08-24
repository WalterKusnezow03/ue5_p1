// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/rooms/room.h"
#include "p2/rooms/layoutCreator.h"
#include <list>

layoutCreator::layoutCreator()
{
    map = nullptr;
    number = 0;
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
                    fill(i, j, i + xSize, j + ySize, p);
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

// ---- layout creator methods ----



void layoutCreator::createRooms(UWorld *worldIn, FVector location){

    //clean up
    //clean();

    map = new layoutCreator::grid(10, 10);
    fillLayout();
}

void layoutCreator::clean(){
    created.clear();
    if(map != nullptr){
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

    int rooms = 10;

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

void layoutCreator::createRoomStartingFromSize(int x, int y){

    
    layoutCreator::roomBounds *room = testRoom(x, y);

    int a = 0;
    int maxAttempts = 100;
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
            room = testRoom(x,y);
        }

        //attempts limit
        a++;
        if(a > maxAttempts){
            return;
        }
    }
}

layoutCreator::roomBounds* layoutCreator::testRoom(int x, int y){
    layoutCreator::roomBounds *s = new layoutCreator::roomBounds(x, y, number++);
    return s;
}