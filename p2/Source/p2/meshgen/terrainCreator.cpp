// Fill out your copyright notice in the Description page of Project Settings.

#include <cmath>
#include "p2/util/FVectorUtil.h"
#include "Algo/Sort.h"  // Include the necessary header
#include "bezierCurve.h"
#include "terrainCreator.h"

terrainCreator::terrainCreator()
{
}

terrainCreator::~terrainCreator()
{
}

terrainCreator::chunk::chunk(int xPos, int yPos)
{
    x = xPos;
    y = yPos;

    //creates the map for the chunk
    float step = 1; //custom step which is not 1 adds unesecarry complexity
    int limit = terrainCreator::CHUNKSIZE;

    for (int i = 0; i < limit; i++){
        
        std::vector<FVector> vec;
        for (int j = 0; j < limit; j++){
            FVector pos(i, j, 0.0f);
            vec.push_back(pos);
        }
        innerMap.push_back(vec);
    }
}

terrainCreator::chunk::~chunk()
{
}

// ---- chunk methods ----

/// @brief applies the height to a chunk between 2 y coordinates. Enclosed area gets enheightend
/// @param xValue x position raw, not edited
/// @param yLowerIndex y lower position in map
/// @param yTopIndex y higher position in map
/// @param yBottomValue y lower position raw
/// @param yTopValue y higher position raw
void terrainCreator::chunk::applyHeight(
    int xValue, 
    int yLowerIndex, 
    int yTopIndex,
    int yBottomValue,
    int yTopValue
){

    int HEIGHT_ADD = 100;
    
    int xModded = (int)(xValue % terrainCreator::CHUNKSIZE);

    //not the same chunk
    if(yLowerIndex != yTopIndex){
        //if not top or bottom
        if(this->y != yLowerIndex && this->y != yTopIndex){
            yBottomValue = 0;
            yTopValue = innerMap.at(0).size();
        }
        if(y == yLowerIndex){
            yBottomValue %= terrainCreator::CHUNKSIZE;
            yTopValue = innerMap.at(0).size();
        }
        if(y == yTopIndex){
            yBottomValue = 0;
            yTopValue %= terrainCreator::CHUNKSIZE;
        }

    }else{
        //both same chunk
        yBottomValue %= terrainCreator::CHUNKSIZE;
        yTopValue %= terrainCreator::CHUNKSIZE;
    }

    for(int i = yBottomValue; i < yTopValue; i++){

        FVector &pos = innerMap.at(xModded).at(i);
        pos.Z += HEIGHT_ADD; //height add
    }
}







// ---- terrain methods ---- 

void terrainCreator::createterrain(UWorld *world, int meters){
    worldPointer = world;

    int chunks = meters / terrainCreator::CHUNKSIZE; //to chunks
    int detail = CHUNKSIZE; // 1 by 1 detail

    for (int i = 0; i < chunks; i++){
        std::vector<terrainCreator::chunk> vec;
        for (int j = 0; j < chunks; j++){
            chunk c(i,j);
            vec.push_back(c);
        }
        map.push_back(vec);
    }

    

    //get anchors...
    std::vector<FVector2D> anchors = createSamplePoints(); //= getAnchors() to be implemented

    std::vector<FVector2D> outputData;
    bezierCurve b;
    b.calculatecurve(anchors, outputData);

    //draw final curve
    debugDrawCurve(world, outputData, FColor::Red);



    //sort by x coordinates, later by y
    std::sort(outputData.begin(), outputData.end(), [](const FVector2D& A, const FVector2D& B) {
        return A.X < B.X;
    });

    applyCurveToMap(outputData);
}

std::vector<FVector2D> terrainCreator::createSamplePoints(){
    std::vector<FVector2D> output;

    int scale = 10; //upscale to meters 

    //testing simple line shape
    int xstep = 2;
    int x = 0;
    int nums[] = {2, 1, 4, 5, 2, 2, 1, -1, 2};
    int size =  sizeof(nums) / sizeof(nums[0]);
    bool flipped = false;

    for (int i : nums){
        FVector2D a(x, i);
        if(x / 2 < size / 2 && !flipped){
            x += xstep;
        }else{
            flipped = true;
            x -= xstep;
        }
       
        a *= scale;
        output.push_back(a);
    }

        
    /*for (int i = 0; i < output.size(); i++){
        output.at(i) *= 10;
    }*/
    return output;
}


/// @brief draws the vector in the world for debugging pruposes, scales up coords slightly.
/// y coords get displayed on z axis!
/// @param world world to show in
/// @param vec Fvectors to show
/// @param color color to draw
void terrainCreator::debugDrawCurve(UWorld* world, std::vector<FVector2D> &vec, FColor color){
    if(world != nullptr){

        int upscale = 5;
        FVector offset(0, 0, 50);

        std::vector<FVector> draw;
        for(FVector2D &v : vec){
            //FVector p(v.X * upscale, 0.0f, v.Y * upscale);
            FVector p(v.X * upscale, 0.0f, v.Y * upscale);
            p += offset;
            draw.push_back(p);
        }
        //draw.push_back(draw.front()); //dont ceonnect last

        DebugHelper::showLine(world, draw, color);
    }
    
}





/// @brief applies the curve to the map
/// @param vec vector of points, must be sorted by x to identify touples
void terrainCreator::applyCurveToMap(std::vector<FVector2D> &vec){

    for (int i = 1; i < vec.size(); i++){

        FVector2D a = vec.at(i - 1);
        FVector2D b = vec.at(i);
        if (isXTouple(a, b))
        {
            processTouple(a, b);
            i++; // go to next (making i += 2)
        }
    }
}

bool terrainCreator::isXTouple(FVector2D &a, FVector2D &b){
    return a.X == b.X;
}


/// @brief validates the index on the y axis, requires the map to at least 1 large and symetrical
/// @param b y index to check
/// @return clamped index
int terrainCreator::validateY(int b){
    if(b < 0){
        b = 0;
    }
    if(map.size() > 0){
        if(b > map.at(0).size()){
            b = map.at(0).size() - 1;
        }
    }
    return b;
}

/// @brief validates the index on the x axis;
/// @param a x index
/// @return clamped index
int terrainCreator::validateX(int a){
    if(a < 0){
        a = 0;
    }
    if(a > map.size()){
        a = map.size() - 1;
    }
    return a;
}

void terrainCreator::processTouple(FVector2D &a, FVector2D &b){

    //create mod chunk size to get index
    
    int xChunk = a.X / terrainCreator::CHUNKSIZE;
    int yBottomIndex = a.Y / terrainCreator::CHUNKSIZE;
    int yTopIndex = b.Y / terrainCreator::CHUNKSIZE;

    int yTop = (int)a.Y;
    int yBottom = (int)b.Y;

    if(yBottomIndex > yTopIndex){
        int copy = yTopIndex;
        yTopIndex = yBottomIndex;
        yBottomIndex = copy;

        int copy1 = yTop;
        yTop = yBottom;
        yBottom = copy1;
    }

    xChunk = validateX(xChunk);
    yBottomIndex = validateY(yBottomIndex);
    yTopIndex = validateY(yBottomIndex);

    //iterate over y axis
    for(int i = yBottomIndex; i < yTopIndex; i++){
        terrainCreator::chunk *c = &map.at(xChunk).at(i);

        int xCopy = (int)a.X;
        c->applyHeight(xCopy, yBottomIndex, yTopIndex, yBottom, yTop);
    }
}

