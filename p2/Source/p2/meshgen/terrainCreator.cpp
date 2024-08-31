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
    for (int i = 0; i < map.size(); i++){
        map.at(i).clear();
    }
    map.clear();
}

terrainCreator::chunk::chunk(int xPos, int yPos)
{
    x = xPos;
    y = yPos;

    //creates the map for the chunk
    int limit = terrainCreator::CHUNKSIZE; //+1 fixes the gap but is wrong

    for (int i = 0; i < limit; i++){
        
        std::vector<FVector> vec;
        for (int j = 0; j < limit; j++){
            FVector pos(i * terrainCreator::ONEMETER, j * terrainCreator::ONEMETER, 0.0f);
            vec.push_back(pos);
        }
        innerMap.push_back(vec);
    }
}

terrainCreator::chunk::~chunk()
{
    for (int i = 0; i < innerMap.size(); i++){
        innerMap.at(i).clear();
    }
    innerMap.clear();
}

// ---- chunk methods ----

/// @brief will return the inner map as reference, is not deisnged to be modified
/// @return map by reference, do not modify
std::vector<std::vector<FVector>> &terrainCreator::chunk::readMap(){
    return innerMap;
}

std::vector<std::vector<FVector>> terrainCreator::chunk::readAndMerge(
    terrainCreator::chunk *top,
    terrainCreator::chunk *right,
    terrainCreator::chunk *topRight
){
    std::vector<std::vector<FVector>> copy = innerMap;
    float offset = terrainCreator::CHUNKSIZE * terrainCreator::ONEMETER;

    if(right != nullptr){
        std::vector<FVector> firstColFromNextRight = right->readFirstXColumn();
        //apply offset
        for (int i = 0; i < firstColFromNextRight.size(); i++){
            firstColFromNextRight.at(i).X += offset;
        }
        copy.push_back(firstColFromNextRight);
    }
    //return copy;

    //might be wrong!
    if(top != nullptr){
        std::vector<FVector> firstRowFromNextTop = top->readFirstYRow();
        //add row
        for (int j = 0; j < firstRowFromNextTop.size(); j++){
            if(j < copy.size()){
                FVector current = firstRowFromNextTop.at(j);
                current.Y += offset;
                copy.at(j).push_back(current);
            }
        }
    }

    //return copy;

    if(topRight != nullptr){
        FVector topRightCorner = topRight->readBottomLeftCorner();
        //proper offset
        topRightCorner.X += offset;
        topRightCorner.Y += offset;

        //add to last col
        copy.at(copy.size() - 1).push_back(topRightCorner);
    }
    
    return copy;
}

//testing with reading first row and column for weird osset fix

/// @brief return the column where x is 0 and y is iterating
/// @return 
std::vector<FVector> terrainCreator::chunk::readFirstXColumn(){
    std::vector<FVector> output;
    for (int i = 0; i < innerMap.size(); i++)
    {
        output.push_back(innerMap.at(0).at(i));
    }
    return output;
}

/// @brief return the row where y is 0 and x is iterating
/// @return 
std::vector<FVector> terrainCreator::chunk::readFirstYRow(){
    std::vector<FVector> output;
    for (int i = 0; i < innerMap.size(); i++)
    {
        output.push_back(innerMap.at(i).at(0));
    }
    return output;
}

FVector terrainCreator::chunk::readBottomLeftCorner(){
    return innerMap.at(0).at(0);
}



/// @brief gets the position in cm based on the complete chunk layout
/// @return position in cm
FVector terrainCreator::chunk::position(){
    FVector v(
        xPositionInCm(),
        yPositionInCm(),
        0);
    //weird consitent one meter offset hotfix between chunks
    //v.X -= x * terrainCreator::ONEMETER;
    //v.Y -= y * terrainCreator::ONEMETER;

    return v;
}

int terrainCreator::chunk::xPositionInCm(){
    int meter = terrainCreator::ONEMETER;
    int chunksize = terrainCreator::CHUNKSIZE;
    int chunkIncm = meter * chunksize;
    int finalvalue = chunkIncm * x;
    return finalvalue;
}
int terrainCreator::chunk::yPositionInCm(){
    int meter = terrainCreator::ONEMETER;
    int chunksize = terrainCreator::CHUNKSIZE;
    int chunkIncm = meter * chunksize;
    int finalvalue = chunkIncm * y;
    return finalvalue;
}

int terrainCreator::chunk::clampInnerIndex(int a){
    if(a >= innerMap.size()){
        a = innerMap.size() - 1;
    }
    if(a < 0){
        a = 0;
    }
    return a;
}
bool terrainCreator::chunk::xIsValid(int a){
    return (a >= 0) && (a < innerMap.size());
}
bool terrainCreator::chunk::yIsValid(int a){
    //return (innerMap.size() > 0) && (a >= 0) && (a < innerMap.at(0).size());
    return xIsValid(a);
}

int terrainCreator::chunk::jumpHeight(){
    return terrainCreator::ONEMETER;
}
bool terrainCreator::chunk::jumpOfInterest(FVector &a, FVector &b){
    int height = jumpHeight();
    return std::abs(a.Z - b.Z) >= height;
}


/// @brief 
/// @param xColumn inner index for columns {0...terrainCreator::CHUNKSIZE}
/// @return 
std::vector<FVector2D> terrainCreator::chunk::getXColumAnchors(int xColumn){
    
    int chunkInCmY = yPositionInCm();

    //saves the position in chunk world of this chunk

    //y ist x achse, z ist höhe
    FVector2D baseAnchorAlongAxis(chunkInCmY, 0);

    std::vector<FVector2D> anchors;
    if(xIsValid(xColumn)){

        //alle sprünge einsammeln und die erste und letzte vertecy

        //erste
        float x_yInCmFirst = innerMap.at(xColumn).at(0).Y; //is in cm by default, without offset
        float y_heightFirst = innerMap.at(xColumn).at(0).Z;
        // build
        FVector2D first(
            x_yInCmFirst, //nach oben richtung x, achse
            y_heightFirst
        );
        first += baseAnchorAlongAxis;



        //skalieren auf chunk welt
        first += baseAnchorAlongAxis;
        anchors.push_back(first);

        FVector2D prev = first;

        //alle
        for (int _y = 1; _y < innerMap.size(); _y++){

            //alle vektoren bauen aus 
            //xcolumn locked
            //y hier vom vector + base offset
            float x_yInCm = innerMap.at(xColumn).at(_y).Y;
            float y_height = innerMap.at(xColumn).at(_y).Z;

            // build
            FVector2D next(
                x_yInCm, //nach oben richtung x, achse
                y_height
            );
            next += baseAnchorAlongAxis;

            if(
                jumpOfInterest(innerMap.at(xColumn).at(_y), innerMap.at(xColumn).at(_y-1)) ||
                _y == innerMap.size() - 2 //prev last
            ){
                anchors.push_back(next);
            }

            prev = next;
            // einfügen
        }
    }

    return anchors;
}




/// @brief just liek the x cloumn method but all variables swapped obviously
/// @param yRow 
/// @return 
std::vector<FVector2D> terrainCreator::chunk::getYRowAnchors(int xColumn){
    
    int meter = terrainCreator::ONEMETER;
    int chunksize = terrainCreator::CHUNKSIZE;
    int chunkIncm = meter * chunksize;
    int chunkInCmY = chunkIncm * x;//y;

    //saves the position in chunk world of this chunk

    //y ist x achse, z ist höhe
    FVector2D baseAnchorAlongAxis(chunkInCmY, 0);

    std::vector<FVector2D> anchors;
    if(xIsValid(xColumn)){

        //alle sprünge einsammeln und die erste und letzte vertecy

        //erste
        float x_yInCmFirst = innerMap.at(0).at(xColumn).Y; //is in cm by default, without offset
        float y_heightFirst = innerMap.at(0).at(xColumn).Z;
        // build
        FVector2D first(
            x_yInCmFirst, //nach oben richtung x, achse
            y_heightFirst
        );
        first += baseAnchorAlongAxis;



        //skalieren auf chunk welt
        first += baseAnchorAlongAxis;
        anchors.push_back(first);

        FVector2D prev = first;

        //alle
        for (int _y = 1; _y < innerMap.size(); _y++){

            //alle vektoren bauen aus 
            //xcolumn locked
            //y hier vom vector + base offset
            float x_yInCm = innerMap.at(_y).at(xColumn).Y;
            float y_height = innerMap.at(_y).at(xColumn).Z;

            // build
            FVector2D next(
                x_yInCm, //nach oben richtung x, achse
                y_height
            );
            next += baseAnchorAlongAxis;

            if(jumpOfInterest(innerMap.at(_y).at(xColumn), innerMap.at(_y-1).at(xColumn))){
                anchors.push_back(next);
            }

            prev = next;
            // einfügen
        }
    }

    return anchors;


}




void terrainCreator::chunk::applyHeightBeetwennVerticalPositions(
    FVector2D aIn,
    FVector2D bIn
){
    int xa = aIn.X;
    int xIndex = convertToInnerIndex(xa);
    if(!xIsValid(xIndex)){
        DebugHelper::showScreenMessage("x invalid", xIndex, FColor::Red);
        return;
    }
    

    int yFrom = 0;
    int yTo = innerMap.size() - 1;

    int aClamped = clampOuterYIndex(aIn);
    int bClamped = clampOuterYIndex(bIn);
    if(aClamped < bClamped){
        yFrom = aClamped;
        yTo = bClamped;
    }
    else
    {
        yFrom = bClamped;
        yTo = aClamped;
    }

    int add = jumpHeight();
    for (int i = yFrom; i <= yTo; i++)
    {
        innerMap.at(xIndex).at(i).Z += add;
    }
}

/// @brief converts a value which can be used inside the map as index
/// @param value in cm!
/// @return 
int terrainCreator::chunk::convertToInnerIndex(int value){
    int total = terrainCreator::CHUNKSIZE * terrainCreator::ONEMETER;
    //scale down
    int mod = value % total;
    int toIndex = mod / terrainCreator::ONEMETER;
    return toIndex;
}

/// @brief converts the Y value the calmped value for this chunk
/// @param a 
/// @return 
int terrainCreator::chunk::clampOuterYIndex(FVector2D &a){
    int total = terrainCreator::CHUNKSIZE * terrainCreator::ONEMETER;
    int yToCheck = a.Y;
    int lowerRange = y * total;
    int higherRange = lowerRange + total;
    //inside bounds
    if(yToCheck >= lowerRange && yToCheck <= higherRange){
        return convertToInnerIndex(yToCheck);
    }

    //clamp out of bounds
    if(yToCheck <= lowerRange){
        return 0;
    }
    if(yToCheck >= higherRange){
        return innerMap.size() - 1;
    }
    return 0;
}


/// @brief applies a new height for an individual vertext
/// @param xIn xpos local index
/// @param yIn ypos local index
/// @param newHeight new height to apply
/// @param override apply with override or average
void terrainCreator::chunk::applyIndivualVertexIndexBased(
    int xIn,
    int yIn,
    int newHeight,
    bool override
){
    xIn = clampInnerIndex(xIn);
    yIn = clampInnerIndex(yIn);
    if(override){
        innerMap.at(xIn).at(yIn).Z = newHeight;
    }else{
        float newAvg = innerMap.at(xIn).at(yIn).Z;
        newAvg += newHeight;
        newAvg /= 2;
        innerMap.at(xIn).at(yIn).Z = newAvg;
    }
}

bool terrainCreator::chunk::isInBounds(FVector &a){
    int total = terrainCreator::CHUNKSIZE * terrainCreator::ONEMETER;
    int selfX = x * total;
    int selfY = y * total;
    int selfXUpper = selfX + total;
    int selfYUpper = selfY + total;

    return selfX <= a.X && a.X <= selfXUpper &&
           selfY <= a.Y && a.Y <= selfYUpper;
}

// --- chunk plotting functions ---


void terrainCreator::chunk::plot(UWorld *world, FColor color){
    int a = round(terrainCreator::ONEMETER * 0.25f);
    a = 10; //10cm
    this->plot(world, color, a);
}

void terrainCreator::chunk::plot(UWorld *world, FColor color, int zOffset){

    FVector offset(
        x * terrainCreator::ONEMETER * terrainCreator::CHUNKSIZE, 
        y * terrainCreator::ONEMETER * terrainCreator::CHUNKSIZE, 
        zOffset
    );
    if(world != nullptr){
        for (int i = 1; i < innerMap.size(); i++){
            for (int j = 1; j < innerMap.at(i).size(); j++){

               
                FVector prevLeft = innerMap.at(i - 1).at(j) + offset;
                FVector prevDown = innerMap.at(i).at(j-1) + offset;
                FVector current = innerMap.at(i).at(j) + offset;

                DebugHelper::showLineBetween(world, prevLeft, current, color);
                DebugHelper::showLineBetween(world, prevDown, current, color);
            }
        }
    }
}

void terrainCreator::chunk::plotCorners(UWorld * world){
    if(world != nullptr){
        FVector basePos = position();

        int count = innerMap.size() - 1;
        FVector top = innerMap.at(count).at(count) + basePos;
        FVector offset(0, 0, 1000);
        offset += top;
        DebugHelper::showLineBetween(world, offset, top, FColor::Blue);

        top = innerMap.at(0).at(0) + basePos;
        offset = FVector(0, 0, 1000);
        offset += top;
        DebugHelper::showLineBetween(world, offset, top, FColor::Red);
    }
}






// ---- terrain methods ---- 

/// @brief will always create a qudratic terrain
/// @param world world to spawn in
/// @param meters size in meters
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


    //detail 10 %, fraction of one
    float detailStep = 0.05f; 

    //get anchors...
    std::vector<FVector2D> anchors = createSamplePoints(); //= getAnchors() to be implemented

    std::vector<FVector2D> outputData;
    bezierCurve b;
    b.calculatecurve(anchors, outputData, detailStep);
    processTopViewBezierCurve(outputData);


    //another
    std::vector<FVector2D> anchors1 = createSamplePoints(); //= getAnchors() to be implemented
    upScalePoints(anchors1, 3);
    std::vector<FVector2D> outputData1;
    b.calculatecurve(anchors1, outputData1, detailStep);
    processTopViewBezierCurve(outputData1);

    //works
    smooth3dMap();
    //works
    plotAllChunks(world, FColor::Cyan);
}

/// @brief process a created bezier
/// @param bezier bezier to apply to map 2d
void terrainCreator::processTopViewBezierCurve(std::vector<FVector2D> &bezier){
    //works as expected
    cleanValues(bezier);

    //sort by x coordinates, later by y
    std::sort(bezier.begin(), bezier.end(), [](const FVector2D& A, const FVector2D& B) {
        return A.X < B.X;
    });
    debugDrawCurve(worldPointer, bezier, FColor::Green);

    applyTopViewCurveToMap(bezier);
}



/// @brief creates a set of sample points
/// @return as vector of FVector2D
std::vector<FVector2D> terrainCreator::createSamplePoints(){
    std::vector<FVector2D> output;

    //One meter. Dont Change.
    int scale = terrainCreator::ONEMETER; //upscale to meters 

    //testing simple line shape
    int xstep = 2;
    int x = 0;
    int nums[] = {2, 1, 4, 5, 2, 3, 1, 1, 2};
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
       
        a *= scale; //upscale
        output.push_back(a);
    }

    
    return output;
}


/// @brief draws the vector in the world for debugging pruposes, scales up coords slightly.
/// y coords get displayed on z axis!
/// @param world world to show in
/// @param vec Fvectors to show
/// @param color color to draw
void terrainCreator::debugDrawCurve(UWorld* world, std::vector<FVector2D> &vec, FColor color){
    if(world != nullptr){

        int upscale = 1;
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



// --- top perpective section ---

/// @brief applies the curve to the map
/// @param vec vector of points, must be sorted by x to identify touples
void terrainCreator::applyTopViewCurveToMap(std::vector<FVector2D> &vec){

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
    if(a.X == b.X){
        return true;
    }
    if(std::abs(a.X - b.X) <= terrainCreator::ONEMETER){
        return true;
    }

    return false;
}

/// @brief validates the index on either x or y axis
/// @return clamped value
int terrainCreator::validateIndex(int a){
    if(a < 0){
        a = 0;
    }
    if(a >= map.size()){
        a = map.size() - 1;
    }
    return a;
}

/// @brief process a ybottom to ytop oriented vector touple
/// @param a 
/// @param b 
void terrainCreator::processTouple(FVector2D &a, FVector2D &b){

    //apply
    //create mod chunk size to get index
    int chunkMeters = terrainCreator::CHUNKSIZE * terrainCreator::ONEMETER;

    int ax = (int)a.X;
    int xChunk = (int)(ax / chunkMeters);
    int xIndex = (int)(ax % chunkMeters);

    int yBottomIndex = (int)(a.Y / chunkMeters);
    int yTopIndex = (int)(b.Y / chunkMeters);


    if(yBottomIndex > yTopIndex){
        int copy = yTopIndex;
        yTopIndex = yBottomIndex;
        yBottomIndex = copy;
    }

    xChunk = validateIndex(xChunk);
    yBottomIndex = validateIndex(yBottomIndex);
    yTopIndex = validateIndex(yTopIndex);

    //iterate over y axis
    for(int i = yBottomIndex; i < yTopIndex + 1; i++){
        terrainCreator::chunk *c = &map.at(xChunk).at(i);
        if(c != nullptr){

            int xCopy = (int)floor(a.X);
            c->applyHeightBeetwennVerticalPositions(a, b);
        }
    }
    
}


/// @brief clamps x and y to map size!
/// will clamp x and y values and clean the map to One meter gaps
/// @param vec vector to clean
void terrainCreator::cleanValues(std::vector<FVector2D> &vec){
    //hot fix weird jump issue, make first like second
    if(vec.size() > 0){
        vec.at(0).Y = vec.at(1).Y;
    }

    //clean
    int i = 1;
    int size = vec.size();
    while(i < size){
        
        if(i > 0){
            int xClampedPrev = (int)(vec.at(i - 1).X / terrainCreator::ONEMETER);
            

            int xClamped = (int)(vec.at(i).X / terrainCreator::ONEMETER);
            
            if(xClampedPrev == xClamped){

                //override x value to clamp
                vec.at(i - 1).X = xClamped * terrainCreator::ONEMETER; //wieder hoch skalieren

                //erase
                vec.erase(vec.begin() + i);
                size = vec.size();
                i--;
            }
        }
        i++;
    }
}









// --- 3d persepective section ---

void terrainCreator::smooth3dMap(){

    float interpolateFrac = 0.05f;

    //get all x and y axis and smooth them. 

    //first smooth all x rows, then add y and devide by two id say
    
    // ---> wenn sich der std vector in grösse nicht ändert darf ich auch pointer darauf haben
    /// weil dann keine daten kopiert werden oder weg genommen

    bezierCurve curve;

    //all x columns
    int xcount = 0;
    for (int i = 0; i < map.size(); i++)
    {
        for (int innerX = 0; innerX < terrainCreator::CHUNKSIZE; innerX++)
        {
            std::vector<FVector2D> column;
            for (int j = 0; j < map.at(i).size(); j++)
            {
                //get data and copy inside
                std::vector<FVector2D> copy = map.at(i).at(j).getXColumAnchors(innerX);
                column.insert(column.end(), copy.begin(), copy.end());
            }

            //proces data in bezier, to output
            std::vector<FVector2D> output;
            curve.calculatecurve(column, output, interpolateFrac);                
            cleanValues(output);

            //trying writing immidately
            applyXColumnToMap(xcount, output);
            xcount++;
           
        }
    }




    /*
    int ycount = 0;
    for (int cY = 0; cY < map.size(); cY++){
        for (int innerY = 0; innerY < terrainCreator::CHUNKSIZE; innerY++)
        {
            std::vector<FVector2D> row;
            //über ganz x laufen und einsammeln
            for (int cX = 0; cX < map.size(); cX++){
                std::vector<FVector2D> copy = map.at(cX).at(cY).getYRowAnchors(innerY);
                row.insert(row.end(), copy.begin(), copy.end());
            }

            std::vector<FVector2D> output;
            curve.calculatecurve(row, output, interpolateFrac);
            cleanValues(output);

            applyYRowToMap(ycount, output);

            ycount++;
        }
    }
    */

}




/// @brief 
/// @param index 
/// @param column 
void terrainCreator::applyXColumnToMap(int index, std::vector<FVector2D> &column){
    
    //copy new height
    float newHeight = 0;

    //convert x as m to chunk
    int xInMeter = index;
    int xInCm = xInMeter * terrainCreator::ONEMETER;
    int xInChunk = cmToChunkIndex(xInCm); //x chunk index in map

    int yInChunk = 0;

    FVector prev;

    // convert y as cm to chunk
    for (int i = 0; i < column.size(); i++){

        FVector2D now = column.at(i);
        newHeight = now.Y;
        float yInCm = now.X;

        //find from x value which is y for axis 
        //the appropaite index
        //from the cm value which the curve is made in
        yInChunk = cmToChunkIndex(yInCm);
        


        if(veriyIndex(yInChunk)){

            terrainCreator::chunk *c = &map.at(xInChunk).at(yInChunk);

            FVector k(
                xInCm,
                yInCm,
                newHeight
            );
            //not last point debug
            if(i < column.size() -1){
                //DebugHelper::showLineBetween(worldPointer, k, prev, FColor::Yellow);
            }
            
            prev = k;

            int xInnerIndex = cmToInnerChunkIndex(xInCm);
            int yInnerIndex = cmToInnerChunkIndex(yInCm);

            c->applyIndivualVertexIndexBased(
                xInnerIndex,
                yInnerIndex,
                newHeight,
                true
            );
        }

    }


}







/// @brief sam emethod as x but vars switched
/// @param index 
/// @param column 
void terrainCreator::applyYRowToMap(int index, std::vector<FVector2D> &row){
    
    //copy new height
    float newHeight = 0;

    //convert x as m to chunk
    int yInMeter = index;
    int yInCm = yInMeter * terrainCreator::ONEMETER;
    int yInChunk = cmToChunkIndex(yInCm); //x chunk index in map

    int xInChunk = 0;

    FVector prev;

    // convert y as cm to chunk
    for (int i = 0; i < row.size(); i++){

        FVector2D now = row.at(i);
        newHeight = now.Y;
        float xInCm = now.X;

        //find from x value which is y for axis 
        //the appropaite index
        //from the cm value which the curve is made in
        xInChunk = cmToChunkIndex(xInCm);
        


        if(veriyIndex(xInChunk)){

            terrainCreator::chunk *c = &map.at(xInChunk).at(yInChunk);

            FVector k(
                xInCm,
                yInCm,
                newHeight
            );

            //not! last point debug.
            if(i < row.size() - 1){
                //DebugHelper::showLineBetween(worldPointer, k, prev, FColor::Green);
            }
            
            prev = k;

            int xInnerIndex = cmToInnerChunkIndex(xInCm);
            int yInnerIndex = cmToInnerChunkIndex(yInCm);

            c->applyIndivualVertexIndexBased(
                xInnerIndex,
                yInnerIndex,
                newHeight,
                false //dont override, average instead!
            );
        }

    }


}







int terrainCreator::cmToMeter(int a){
    return a / terrainCreator::ONEMETER;
}
int terrainCreator::meterToInnerChunkIndex(int a){
    return a % terrainCreator::CHUNKSIZE;
}
/// @brief creates the inner chunk index from cm
/// @param a cm
/// @return index
int terrainCreator::cmToInnerChunkIndex(int a){
    a = cmToMeter(a);
    return meterToInnerChunkIndex(a);
}

/// @brief creates the outer chunk index inside the map for a cm value
/// @param a cm in
/// @return chunk index outer
int terrainCreator::cmToChunkIndex(int a){
    //int cmForOneChunk = terrainCreator::CHUNKSIZE * terrainCreator::ONEMETER;
    int aToMeter = cmToMeter(a);
    int aToChunk = aToMeter / terrainCreator::CHUNKSIZE;
    return aToChunk;
}

/// @brief checks if the index is within the map bounds
/// @param a index
/// @return true false map bounds kept
bool terrainCreator::veriyIndex(int a){
    return a >= 0 && a < map.size();
}







void terrainCreator::plotAllChunks(UWorld * world, FColor color){
    if(world != nullptr && terrainCreator::PLOTTING_ENABLED){

        for (int i = 0; i < map.size(); i++){
            for (int j = 0; j < map.at(i).size(); j++){
                map.at(i).at(j).plot(world, color);
            }
        }
    }
}





/// @brief scales the list with a constant factor
/// @param points points to scale
/// @param factor factor to apply
void terrainCreator::upScalePoints(std::vector<FVector2D> &points, int factor){
    for (int i = 0; i < points.size(); i++){
        FVector2D &referenced = points.at(i);
        referenced *= factor;
    }
}




/// @brief returns the count of the inner chunks
/// @return count
int terrainCreator::chunkNum(){
    if(map.size() > 0){
        return map.size() * map.at(0).size();
    }
    return 0;
}

/// @brief will create all meshes and populate the data
/// will not hold on to the reference when goes out of scope, only applies the data
/// where it can and doesnt go out of bounds
void terrainCreator::applyTerrainDataToMeshActors(std::vector<AcustomMeshActor*> &actors){

    int a = 0;
    int x = 0;
    int y = 0;
    //inclusive index limit
    int xLimit = map.size();
    int yLimit = map.size();


    while(a < actors.size()){
        //do actor things
        AcustomMeshActor *currentActor = actors.at(a);
        a++;
        if (currentActor != nullptr)
        {   
            terrainCreator::chunk *currentChunk = &map.at(x).at(y);
            

            //debug
            currentChunk->plotCorners(worldPointer);

            // get position
            // apply position
            FVector newPos = currentChunk->position();
            currentActor->SetActorLocation(newPos); //some offset might be applied later addionally if wanted

            //apply data new testing
            //readAndMerge (connect to prev map)
            terrainCreator::chunk *top = nullptr;
            terrainCreator::chunk *right = nullptr;
            terrainCreator::chunk *topright = nullptr;
            if(y + 1 < yLimit){
                top = &map.at(x).at(y+1);
            }
            if(x + 1 < xLimit){
                right = &map.at(x+1).at(y);
            }
            if(x + 1 < xLimit && y + 1 < yLimit){
                topright = &map.at(x+1).at(y+1);
            }
            std::vector<std::vector<FVector>> mapCopy = currentChunk->readAndMerge(top, right, topright);
            currentActor->process2DMap(mapCopy);

            // apply data
            //currentActor->process2DMap(currentChunk->readMap());

            x++;
            //top corner reached, return
            if(y >= yLimit && x >= xLimit){
                return;
            }
            //next row
            if(x >= xLimit){
                x = 0;
                y++;
            }
        }
    }

}
