// Fill out your copyright notice in the Description page of Project Settings.

#include <cmath>
#include "p2/util/FVectorUtil.h"
#include "Algo/Sort.h"  // Include the necessary header
#include "bezierCurve.h"
#include "p2/util/TVector.h"
#include "HAL/PlatformTime.h"
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

/***
 * 
 * ---- CHUNK METHODS -----
 * 
 */

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

void terrainCreator::chunk::setTreesBlocked(bool b){
    blockTrees = b;
}
bool terrainCreator::chunk::createTrees(){
    return !blockTrees;
}

/// @brief will return the inner map as reference, is not deisnged to be modified
/// @return map by reference, do not modify
std::vector<std::vector<FVector>> &terrainCreator::chunk::readMap(){
    return innerMap;
}

/// @brief returns a deep copy of this chunks map connecting too 
/// @param top top chunk 
/// @param right right chunk
/// @param topRight top right chunk
/// @return slightly extended map - fixes a bug where each chunk had one meter gap
std::vector<std::vector<FVector>>& terrainCreator::chunk::readAndMerge(
    terrainCreator::chunk *top,
    terrainCreator::chunk *right,
    terrainCreator::chunk *topRight
){
    //std::vector<std::vector<FVector>> copy = innerMap;
    float offset = terrainCreator::CHUNKSIZE * terrainCreator::ONEMETER;

    if(right != nullptr){
        std::vector<FVector> firstColFromNextRight = right->readFirstXColumn();
        //apply offset
        for (int i = 0; i < firstColFromNextRight.size(); i++){
            firstColFromNextRight.at(i).X += offset;
        }
        innerMap.push_back(firstColFromNextRight);
    }
    
    //might be wrong!
    if(top != nullptr){
        std::vector<FVector> firstRowFromNextTop = top->readFirstYRow();
        //add row
        for (int j = 0; j < firstRowFromNextTop.size(); j++){
            if(j < innerMap.size()){
                FVector current = firstRowFromNextTop.at(j);
                current.Y += offset;
                innerMap.at(j).push_back(current);
            }
        }
    }


    if(topRight != nullptr){
        FVector topRightCorner = topRight->readBottomLeftCorner();
        //proper offset
        topRightCorner.X += offset;
        topRightCorner.Y += offset;

        //add to last col
        innerMap.at(innerMap.size() - 1).push_back(topRightCorner);
    }
    
    return innerMap;
}


/**
 * RAYCAST
 */
/// @brief get the height for an specific vertex
/// @param a vertex
/// @return 
int terrainCreator::chunk::getHeightFor(FVector &a){
    if(isInBounds(a)){
        //apply offset because mesh center is at center 
        //int offsetCenter = terrainCreator::ONEMETER * (innerMap.size() / 2); //moved to chunk instantiation
        int xa = convertToInnerIndex(a.X);
        int ya = convertToInnerIndex(a.Y);
        return innerMap.at(xa).at(ya).Z;
    }
    return a.Z;
}




/// @brief adds a value to all positions of the chunk
/// @param value adds a value to the z part of each vertex in this chunk
void terrainCreator::chunk::addheightForAll(int value){
    for (int i = 0; i < innerMap.size(); i++){
        for (int j = 0; j < innerMap.at(i).size(); j++){
            FVector &adjust = innerMap.at(i).at(j);
            adjust.Z += value;

            if(adjust.Z > terrainCreator::MAXHEIGHT){
                adjust.Z = terrainCreator::MAXHEIGHT;
            }
        }
    }
}

/// @brief multiplies the value to all positions of the chunk in z height
/// @param value mulitplicator
void terrainCreator::chunk::scaleheightForAll(float value){
    for (int i = 0; i < innerMap.size(); i++){
        for (int j = 0; j < innerMap.at(i).size(); j++){
            FVector &adjust = innerMap.at(i).at(j);
            adjust.Z *= value;

            if(adjust.Z > terrainCreator::MAXHEIGHT){
                adjust.Z = terrainCreator::MAXHEIGHT;
            }
        }
    }
}

/// @brief sets the height for all positions of the chunk to a given value, overrides
/// @param value value to set
void terrainCreator::chunk::setheightForAll(float value){
    for (int i = 0; i < innerMap.size(); i++)
    {
        for (int j = 0; j < innerMap.at(i).size(); j++){
            //du musst hier eine referenz erzeugen weil er sonst nicht reinschreibt?
            FVector &ref = innerMap.at(i).at(j);
            ref.Z = value;
        }
    }
}




//testing with reading first row and column for weird osset fix

/// @brief return the column where x is 0 and y is iterating
/// @return first x column (upward along y)
std::vector<FVector> terrainCreator::chunk::readFirstXColumn(){
    std::vector<FVector> output;
    for (int i = 0; i < innerMap.size(); i++)
    {
        output.push_back(innerMap.at(0).at(i));
    }
    return output;
}

/// @brief return the row where y is 0 and x is iterating
/// @return first y row (right along x)
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
        0
    );

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
/// @brief will return if the inner index is in map bounds
/// @param a 
/// @return 
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
int terrainCreator::chunk::heightAdd(){
    return terrainCreator::ONEMETER / terrainCreator::fractureHeightStep; //test
}

bool terrainCreator::chunk::jumpOfInterest(FVector &a, FVector &b){
    int height = std::abs(jumpHeight());
    return std::abs(a.Z - b.Z) >= height / 2; //halfed at least
}

FVector2D terrainCreator::chunk::getFirstXColumnAnchor(int xColumn){
    int chunkInCmY = yPositionInCm();
    FVector2D baseAnchorAlongAxis(chunkInCmY, 0);

    int atIndex = innerMap.size() / 2;

    if(xIsValid(xColumn)){
        float x_yInCmFirst = innerMap.at(xColumn).at(atIndex).Y; //is in cm by default, without offset
        float y_heightFirst = innerMap.at(xColumn).at(atIndex).Z;
        FVector2D first(
            x_yInCmFirst, //nach oben richtung x, achse
            y_heightFirst
        );
        first += baseAnchorAlongAxis;
        return first;
    }
    return baseAnchorAlongAxis;
}

FVector2D terrainCreator::chunk::getFirstYRowAnchor(int yRow){
    int chunkInCmY = xPositionInCm(); //flipped for x

    //y ist x achse, z ist höhe
    FVector2D baseAnchorAlongAxis(chunkInCmY, 0);

    int atIndex = innerMap.size() / 2;

    std::vector<FVector2D> anchors;
    if(xIsValid(yRow)){
        float x_yInCmFirst = innerMap.at(atIndex).at(yRow).Y; //is in cm by default, without offset
        float y_heightFirst = innerMap.at(atIndex).at(yRow).Z;
        // build
        FVector2D first(
            x_yInCmFirst, //nach oben richtung x, achse
            y_heightFirst
        );
        first += baseAnchorAlongAxis;
        return first;
    }
    return baseAnchorAlongAxis;
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
    float newHeight,
    bool override
){
    if(!xIsValid(xIn) || !yIsValid(yIn)){
        return;
    }

    xIn = clampInnerIndex(xIn);
    yIn = clampInnerIndex(yIn);
    if(override){
        innerMap.at(xIn).at(yIn).Z = newHeight;
    }else{
        float newAvg = innerMap.at(xIn).at(yIn).Z;
        newAvg += newHeight;
        newAvg /= 2;
        
        if(newAvg > terrainCreator::MAXHEIGHT){
            newAvg = terrainCreator::MAXHEIGHT;
        }
        innerMap.at(xIn).at(yIn).Z = newAvg;
    }

}

/// @brief will check if a position is within the chunks bounds
/// @param a position to check
/// @return return true if within the chunks index bounds on x and y axis
bool terrainCreator::chunk::isInBounds(FVector &a){
    int total = terrainCreator::CHUNKSIZE * terrainCreator::ONEMETER;
    int selfX = x * total;
    int selfY = y * total;
    int selfXUpper = selfX + total;
    int selfYUpper = selfY + total;

    return selfX <= a.X && a.X <= selfXUpper &&
           selfY <= a.Y && a.Y <= selfYUpper;
}

/// @brief will remove all spikes and graps from the map, designed to be called with smooth map function
void terrainCreator::chunk::fixGaps(){
    for (int i = 1; i < innerMap.size() - 1; i++){
        for (int j = 1; j < innerMap.at(i).size() - 1; j++){
            FVector &center = innerMap.at(i).at(j);

            FVector &up = innerMap.at(i).at(j+1);
            FVector &down= innerMap.at(i).at(j-1);
            FVector &right = innerMap.at(i+1).at(j);
            FVector &left = innerMap.at(i-1).at(j);

            //spitzen
            float schnittGes = ((up.Z + down.Z) + (left.Z + right.Z)) / 4;
            if(std::abs(schnittGes - center.Z) > terrainCreator::ONEMETER / 2){
                center.Z = schnittGes;
                continue;
            }


            //schnitt oben unten
            float upDownSchnitt = (up.Z + down.Z) / 2;
            if(std::abs(upDownSchnitt - center.Z) > terrainCreator::ONEMETER / 2){
                center.Z = upDownSchnitt;
                continue;
            }

            //schnitt rechts links
            upDownSchnitt = (left.Z + right.Z) / 2;
            if(std::abs(upDownSchnitt - center.Z) > terrainCreator::ONEMETER / 2){
                center.Z = upDownSchnitt;
                continue;
            }
        }
    }
}





// --- chunk plotting functions ---



void terrainCreator::chunk::plot(UWorld *world){

    FVector offset(
        x * terrainCreator::ONEMETER * terrainCreator::CHUNKSIZE, 
        y * terrainCreator::ONEMETER * terrainCreator::CHUNKSIZE, 
        120 //10cm
    );
    //defferentiate heights
    std::vector<FColor> colors;
    colors.push_back(FColor::Red);
    colors.push_back(FColor::Blue);
    colors.push_back(FColor::Green);

    float oneThird = terrainCreator::ONEMETER / 2;

    if(world != nullptr){
        for (int i = 1; i < innerMap.size(); i++){
            for (int j = 1; j < innerMap.at(i).size(); j++){

                FVector prevLeft = innerMap.at(i - 1).at(j) + offset;
                FVector prevDown = innerMap.at(i).at(j-1) + offset;
                FVector current = innerMap.at(i).at(j) + offset;

                int toIndex = (int) std::abs(current.Z / oneThird);
                FColor currentColor = colors.at(toIndex % colors.size());
                DebugHelper::showLineBetween(world, prevLeft, current, currentColor);
                DebugHelper::showLineBetween(world, prevDown, current, currentColor);
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



/***
 * 
 * ---- TERRAIN METHODS -----
 * 
 */


// ---- terrain methods ---- 

/// @brief will always create a qudratic terrain
/// @param world world to spawn in / debug draw in
/// @param meters size in meters
void terrainCreator::createTerrain(UWorld *world, int meters){
    worldPointer = world;

    int chunks = floor(meters / terrainCreator::CHUNKSIZE); //to chunks
    //int detail = CHUNKSIZE; // 1 by 1 detail

    for (int i = 0; i < chunks; i++){
        std::vector<terrainCreator::chunk> vec;
        for (int j = 0; j < chunks; j++){
            chunk c(i,j);
            vec.push_back(c);
        }
        map.push_back(vec);
    }

    

    /**
     * Enheigheting the whole chunk and smoothening at a later point works much better
     * and looks much better too.
     * Terrain will be kept like this
     * -> additional thoughts: hills could be prdefined by marking certain x and y chunks and
     * enheighten them more than others
     */

    


    int hillCount = 5; //means 40m height in worse case
    for (int j = 0; j < hillCount; j++)
    {
        createBezierChunkWide();
    }
    
    //whole map smoothening (works good but takes long)
    
    smooth3dMap();

    // must be set on terrain type, more hills, flatter terrain like deserts... etc.
    // works good and as intended, multiply could be set to 0 < val < 1 values for deserts for example
    /*
    float multiply = 1.0f;
    for (int i = 0; i < map.size(); i++)
    {
        for (int j = 0; j < map.at(i).size(); j++){
            map.at(i).at(j).scaleheightForAll(multiply);
        }
    }*/

}





/// @brief draws the vector in the world for debugging pruposes, scales up coords slightly.
/// y coords get displayed on z axis!
/// @param world world to show in
/// @param vec Fvectors to show
/// @param color color to draw
void terrainCreator::debugDrawCurve(UWorld* world, std::vector<FVector2D> &vec, FColor color){
    if(world != nullptr){

        int upscale = 100;
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


/// @brief clamps x to map size / einheitsvalue
/// will clamp x values and clean the map to One meter gaps and will remove duplicates if 
/// ordered by each other
/// @param vec vector to clean
void terrainCreator::cleanValues(TVector<FVector2D> &vec, int scalingCut){
    
    //cast all values to clear meters
    for (int i = 0; i < vec.size(); i++){
        // Round to nearest multiple of ONEMETER (scalingCut)
        vec.at(i).X = (int)std::round(vec.at(i).X / scalingCut) * scalingCut; //only x because this function 
        //is used by 3d too
    }

    // Remove duplicates
    int i = 1;
    int size = vec.size();
    while (i < size)
    {
        if (vec.at(i - 1).X == vec.at(i).X){ //&& vec.at(i - 1).Y == vec.at(i).Y
            //vec.erase(vec.begin() + i);  // Remove duplicate
            vec.erase(i);  // Remove duplicate
        } else {
            i++;
        }
        size = vec.size();
    }

    //fillGaps(vec);

}

/// @brief linerally interpolates the given vector for X axis, for One meter consitency / distance
/// between each vector
/// @param vec passed vector to fill gaps in 
void terrainCreator::fillGaps(std::vector<FVector2D> &vec){

    int size = vec.size();
    int i = 1;
    while(i < size){
        FVector2D A = vec.at(i - 1); //prev A
        FVector2D B = vec.at(i); //current B
        int distance = std::abs(A.X - B.X);

        if(distance > terrainCreator::ONEMETER){
            FVector2D connect = (B - A).GetSafeNormal();
            // Compute the number of new points to insert
            int numPoints = std::floor(distance / ONEMETER);
            for (int j = 1; j < numPoints; j++) {
                FVector2D newPos;
                newPos.X = A.X + j * ONEMETER;
                newPos.Y = A.Y + j * connect.Y; // Keep the same Y value
                vec.insert(vec.begin() + i, newPos);
                i++; // Move to next insertion point
            }
            
        }
        i++;
        size = vec.size();
    }
}

/// @brief scales the height for all chunks (designed to upscale before bezier and downscale later)
/// creates more detailed interpolation on Z axis (maybe)
/// @param scale sclae to set
void terrainCreator::scaleHeightForAll(float scale){
    for (int i = 0; i < map.size(); i++){
        for (int j = 0; j < map.at(i).size(); j++){
            map.at(i).at(j).scaleheightForAll(scale);
        }
    }
}





/**
 * ----- 3D SMOOTH MAP SECTION -----
 */

/// @brief will smooth out all chunks rows and columns and merge them together to the map
void terrainCreator::smooth3dMap(){
    FVector a(0, 0, 0);
    int max = map.size() * terrainCreator::ONEMETER * terrainCreator::CHUNKSIZE;
    FVector b(max, max, 0);

    int iterations = 3;
    smooth3dMap(a, b, iterations);
}

/// @brief will smooth out all chunks rows and columns and merge them together to the map
void terrainCreator::smooth3dMap(FVector &a, FVector &b, int iterations){

    double StartTime = FPlatformTime::Seconds();

    //upscale
    float scaleToSet = 10.0f;
    scaleHeightForAll(scaleToSet); //scaling up and scaling down later does smooth out the curve a bit (seemengly)

    //calculate enclosed bounds, works as expected
    int fromX = a.X < b.X ? a.X : b.X;
    int fromY = a.Y < b.Y ? a.Y : b.Y;
    int toX = a.X > b.X ? a.X : b.X;
    int toY = a.Y > b.Y ? a.Y : b.Y;
    fromX = clampIndex(cmToChunkIndex(fromX));
    fromY = clampIndex(cmToChunkIndex(fromY));
    toX = clampIndex(cmToChunkIndex(toX));
    toY = clampIndex(cmToChunkIndex(toY));


    // get all x and y axis and smooth them.
    bezierCurve curve;

    //define this vector out side loop for preventing unesecarry wiping from stack until complete
    //TVector data gets overriden anyway / internal garbage collector
    int scalePrediction = terrainCreator::CHUNKSIZE * map.size(); //one meter gaps obviously.
    TVector<FVector2D> output(scalePrediction); // use only one custom tvector for efficency

    for (int it = 0; it < iterations; it++){

        // all x columns
        int xcount = 0;
        //for (int i = 0; i < map.size(); i++)
        for (int i = fromX; i <= toX; i++)
        {   
            
            for (int innerX = 0; innerX < terrainCreator::CHUNKSIZE; innerX++)
            {
                std::vector<FVector2D> column;
                //for (int j = 0; j < map.at(i).size(); j++)
                for (int j = fromY; j <= toY; j++){
                    column.push_back(map.at(i).at(j).getFirstXColumnAnchor(innerX));
                }
                output.clear();
                curve.calculatecurve(column, output, terrainCreator::ONEMETER, 1);

                //if first smooth, and first row, create curve, other wise: reuse
                if(it == 0 && innerX == 0){
                    
                }
                
                
                //cleanValues(output, ONEMETER); //can! be the case

                //trying writing immidately
                applyXColumnToMap(xcount, output);
                xcount++;
            
            }
        }
        



        //then all y rows
        int ycount = 0;
        for (int cY = fromY; cY <= toY; cY++){
        //for (int cY = 0; cY < map.size(); cY++){
            for (int innerY = 0; innerY < terrainCreator::CHUNKSIZE; innerY++)
            {
                std::vector<FVector2D> row;
                //über ganz x laufen und einsammeln
                for (int cX = fromX; cX <= toX; cX++){
                    row.push_back(map.at(cX).at(cY).getFirstYRowAnchor(innerY));
                }

                output.clear();
                curve.calculatecurve(row, output, terrainCreator::ONEMETER, 1);
                

                applyYRowToMap(ycount, output);

                ycount++;
            }
        }


        //fill gaps and spikes, very important chunk
        for(int i = 0; i < map.size(); i++){
            for(int j = 0; j < map.at(i).size(); j++){
                map.at(i).at(j).fixGaps();
            }
        }
    }

    //downscale
    scaleHeightForAll(1 / scaleToSet);

    //log
    //double EndTime = FPlatformTime::Seconds();
    //double ElapsedTime = EndTime - StartTime;
    //UE_LOG(LogTemp, Log, TEXT("Elapsed time: %f seconds"), ElapsedTime);
}












/// @brief applies the height from the inner vertecies to the map
/// @param index index of column in whole chunk map
/// @param column column data to override (int all chunks) (2D bezier)
void terrainCreator::applyXColumnToMap(int index, 
TVector<FVector2D> &column
){
    
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
        


        if(verifyIndex(yInChunk)){

            terrainCreator::chunk *c = &map.at(xInChunk).at(yInChunk);

            //calculate the position to apply the height to within the chunk
            int xInnerIndex = cmToInnerChunkIndex(xInCm);
            int yInnerIndex = cmToInnerChunkIndex(yInCm);


            c->applyIndivualVertexIndexBased(
                xInnerIndex,
                yInnerIndex,
                newHeight,
                true
                //true //override true
            );

            
        }

    }


}




/// @brief same method as x but vars switched
/// @param index index of row of whole map
/// @param row row data to override (int all chunks) (2D bezier)
void terrainCreator::applyYRowToMap(int index, 
TVector<FVector2D> &row
//std::vector<FVector2D> &row
){
    
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
        


        if(verifyIndex(xInChunk)){

            terrainCreator::chunk *c = &map.at(xInChunk).at(yInChunk);

            /*
            FVector k(
                xInCm,
                yInCm,
                newHeight
            );
            prev = k;
            */

            //calculate the position to apply the height to within the chunk
            int xInnerIndex = cmToInnerChunkIndex(xInCm);
            int yInnerIndex = cmToInnerChunkIndex(yInCm);

            c->applyIndivualVertexIndexBased(
                xInnerIndex,
                yInnerIndex,
                newHeight,
                false //override, x false, merge values instead
            );
        }

    }


}

/**
 * 
 * --- EMBED ROOMS ---
 * 
 */

/// @brief will set the are to a certain height provided in the location vector, which should be 
/// the bottom left corner of the rooms created, will smooth the terrain data afterwards
/// @param location location and Z offset to set
/// @param sizeMetersX size in meters X
/// @param sizeMetersY size in meters Y
void terrainCreator::setFlatArea(FVector &location, int sizeMetersX, int sizeMetersY){
    //convert to meters ue5 world scale just as usual
    sizeMetersX *= 100;
    sizeMetersY *= 100;

    //get map bounds with little upscaling
    int fromX = clampIndex(cmToChunkIndex(location.X) - 1);
    int fromY = clampIndex(cmToChunkIndex(location.Y) - 1);
    int toX = clampIndex(fromX + cmToChunkIndex(sizeMetersX) + 1);
    int toY = clampIndex(fromY + cmToChunkIndex(sizeMetersY) + 1);

    int heightToSet = location.Z;

    //iterate over map and smooth
    for (int i = fromX; i <= toX; i++){
        for (int j = fromY; j <= toY; j++){
            terrainCreator::chunk *c = &map.at(i).at(j);
            if(c != nullptr){
                c->setheightForAll(heightToSet);
                c->setTreesBlocked(true);
            }
        }
    }

    //finally also smooth the map
    FVector buffer = FVector(terrainCreator::CHUNKSIZE, terrainCreator::CHUNKSIZE, 0);
    FVector a = location - buffer;
    FVector b(
        sizeMetersX, 
        sizeMetersY,
        0
    );
    b += (a + buffer * 2);

    int iterations = 2;
    smooth3dMap(a, b, iterations); // disabled for debugging
}

/**
 * 
 * --- SCALE CONVERSION SECTION ---
 * 
 */


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
bool terrainCreator::verifyIndex(int a){
    return a >= 0 && a < map.size();
}


/// @brief validates the index on either x or y axis
/// @return clamped value valid to acces the 2D vector map
int terrainCreator::clampIndex(int a){
    if(a < 0){
        a = 0;
    }
    if(a >= map.size()){
        a = map.size() - 1;
    }
    return a;
}







/**
 * 
 * plotting
 *
 */


/// @brief debug plotting
/// @param world 
void terrainCreator::plotAllChunks(UWorld * world){
    if(world != nullptr && terrainCreator::PLOTTING_ENABLED){

        for (int i = 0; i < map.size(); i++){
            for (int j = 0; j < map.at(i).size(); j++){
                map.at(i).at(j).plot(world);
            }
        }
    }
}

/**
 * --- VIRTUAL RAYCAST ---
 * DEBUG NEEDED, ENTETIES YEET IN THE AIR
 */

/// @brief instead of raycasting the z height can be got from the generated mesh data
/// @param position position to find (only x y important)
/// @return return z for the x y position
int terrainCreator::getHeightFor(FVector &position){

    int chunkX = cmToChunkIndex(position.X);
    int chunkY = cmToChunkIndex(position.Y);

    if(verifyIndex(chunkX) && verifyIndex(chunkY)){
        return map.at(chunkX).at(chunkY).getHeightFor(position);
    }
    return position.Z;
}



/// @brief returns the count of the inner chunks in total
/// @return count
int terrainCreator::chunkNum(){
    //is used from the entity manager to pass the mesh actors
    if(map.size() > 0){
        return map.size() * map.at(0).size();
    }
    return 0;
}

/**
 * ---- APPLY DATA TO MESH ACTORS ----
 */

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
            
            // get position
            // apply position
            FVector newPos = currentChunk->position();

            //fix offset to be anchor at bottom lefta dn not center of the mesh,
            //so the coordinate in mesh (0,0) is (0,0) and not (chunksize /2, chunksize/2)
            float offsetCenter = terrainCreator::ONEMETER * (terrainCreator::CHUNKSIZE / 2);
            newPos.X -= offsetCenter;
            newPos.Y -= offsetCenter;
            currentActor->SetActorLocation(newPos); 


            //apply data
            //readAndMerge (connect to next in map)
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
            std::vector<std::vector<FVector>> &mapReference = currentChunk->readAndMerge(top, right, topright);

            bool treesBlocked = currentChunk->createTrees();
            currentActor->createTerrainFrom2DMap(mapReference, treesBlocked);
            currentActor->setMaterialBehaiviour(materialEnum::grassMaterial, false); //no split

            

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

    DebugHelper::logTime("elapsed total raycast time"); //30 seconds.
    PathFinder *p = PathFinder::instance();
    if(p != nullptr){
        p->debugCountNodes();
    }
}

/**
 * 
 * ----- DEPRECATED / NOT IN USE -> do NOT remove, might be fixed. -----
 * 
 */


// --- top perpective section ---

/// @brief will create a bezier curve and add a 100cm height increase at each chunk enclosed.
/// @param offset offset chunk index to take along x and y
void terrainCreator::createBezierChunkWide(){

    //creates terrain better than a bezier curve imo.
    int scaleX = FVectorUtil::randomNumber(6, map.size());
    int scaleY = FVectorUtil::randomNumber(6, scaleX);
    int startX = clampIndex(FVectorUtil::randomNumber(1, map.size() / 2));
    int startY = clampIndex(FVectorUtil::randomNumber(1, map.size() / 2));

    int endX = clampIndex(startX + scaleX);
    int endY = clampIndex(startY + scaleY);

    int layerCount = 0;
    while (
        (endX - startX) >= 3 &&
        (endY - startY) >= 3 &&
        layerCount < 7
    ){
        for (int i = startX; i <= endX; i++)
        {
            for (int j = startY; j <= endY; j++)
            {
                map.at(i).at(j).addheightForAll(
                    FVectorUtil::randomNumber(terrainCreator::ONEMETER, terrainCreator::ONEMETER * 3)
                );
            }
        }
        layerCount++;
        startX = clampIndex(startX + 1);
        startY = clampIndex(startX + 1);
        endX = clampIndex(endX - 1);
        endY = clampIndex(endY - 1);
    }
    return;

}

/// Deprecated
/// @brief process a created bezier
/// @param bezier bezier to apply to map 2d
void terrainCreator::processTopViewBezierCurve(std::vector<FVector2D> &bezier){
    
    if(bezier.size() < 2){
        return;
    }

    //sort by x coordinates, later by y
    std::sort(bezier.begin(), bezier.end(), [](const FVector2D& A, const FVector2D& B) {
        return A.X < B.X;
    });

    applyTopViewCurveToMap(bezier);
}

/// Deprecated
/// @brief applies the curve to the map
/// @param vec vector of points, must be sorted by x to identify touples
void terrainCreator::applyTopViewCurveToMap(std::vector<FVector2D> &vec){

    for (int i = 1; i < vec.size(); i++){

        FVector2D a = vec.at(i - 1);
        FVector2D b = vec.at(i);

        if(a.X == b.X){
            processToupleChunk(a, b);
            i++;
        }

    }
}




/// @brief will always add one meter
/// @param a between a and
/// @param b b node
void terrainCreator::processToupleChunk(FVector2D &a, FVector2D &b){
    //by refernce natürlich weil effizient
    int xIndex = a.X;
    int y1 = std::min(a.Y, b.Y);
    int y2 = std::max(a.Y, b.Y);

    if(
        verifyIndex(xIndex) //only verify x, y will both be clamped when needed
    ){
        //clamp to make sure
        xIndex = clampIndex(xIndex);
        y1 = clampIndex(y1);
        y2 = clampIndex(y2) - 1; //keep like this, last chunk is error prone 

        for (int i = y1; i <= y2; i++){
            chunk *c = &map.at(xIndex).at(i);
            if(c != nullptr){
                c->addheightForAll(terrainCreator::ONEMETER); //new all chunk appl
            }
        }
    }

    
}


/// @brief scales the list with a constant factor
/// @param points points to scale
/// @param factor factor to apply
void terrainCreator::upScalePoints(std::vector<FVector2D> &points, float factor){
    if(factor > 0){
        for (int i = 0; i < points.size(); i++){
            FVector2D &referenced = points.at(i);
            referenced *= factor;
        }
    }
}

/// @brief apply an offset to every point of the vector
/// @param vec vector
/// @param offset offset to apply
void terrainCreator::offsetPoints(std::vector<FVector2D> &vec, FVector2D offset){
    for (int i = 0; i < vec.size(); i++){
        vec.at(i) += offset;
    }
}



