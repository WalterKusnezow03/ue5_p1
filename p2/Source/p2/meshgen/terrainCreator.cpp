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
    float step = 1; //custom step which is not 1 adds unesecarry complexity
    int limit = terrainCreator::CHUNKSIZE;

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
    return (innerMap.size() > 0) && (a >= 0) && (a < innerMap.at(0).size());
}

int terrainCreator::chunk::jumpHeight(){
    return terrainCreator::ONEMETER;
}
bool terrainCreator::chunk::jumpOfInterest(FVector &a, FVector &b){
    int height = jumpHeight();
    return std::abs(a.Z - b.Z) >= height;
}
std::vector<FVector2D> terrainCreator::chunk::getXColumAnchors(int xColumn){
    int realScale = terrainCreator::CHUNKSIZE * terrainCreator::ONEMETER;
    std::vector<FVector2D> anchors;
    if (xIsValid(xColumn))
    {
        for (int i = 0; i < innerMap.at(xColumn).size(); i++){
            
            if(i == 0 || i == innerMap.at(xColumn).size() - 1){
                // convert to 2D
                FVector current = innerMap.at(xColumn).at(i);
                //xpos + i, height offset
                FVector2D converted(x * realScale + i * terrainCreator::ONEMETER, current.Z);
                anchors.push_back(converted);

            }else{
                FVector prev = innerMap.at(xColumn).at(i - 1);
                FVector current = innerMap.at(xColumn).at(i);
                if(jumpOfInterest(prev, current)){
                    //xpos + i, height offset
                    FVector2D converted(x * realScale + i * terrainCreator::ONEMETER, current.Z);
                    anchors.push_back(converted);
                }
            }
            
        }
    }
    return anchors;
}
std::vector<FVector2D> terrainCreator::chunk::getYRowAnchors(int yRow){
    int realScale = terrainCreator::CHUNKSIZE * terrainCreator::ONEMETER;

    std::vector<FVector2D> anchors;
    if (yIsValid(yRow))
    {
        for (int i = 0; i < innerMap.size(); i++){
            if(i == 0 || i == innerMap.size() - 1){
                //convert to 2D
                FVector current = innerMap.at(i).at(yRow);

                //ypos + i, height offset
                FVector2D converted(y * realScale + i * terrainCreator::ONEMETER, current.Z);
                anchors.push_back(converted);

            }else{
                FVector prev = innerMap.at(i - 1).at(yRow);
                FVector current = innerMap.at(i).at(yRow);
                if(jumpOfInterest(prev, current)){
                    //ypos + i, height offset
                    FVector2D converted(y * realScale + i * terrainCreator::ONEMETER, current.Z);
                    anchors.push_back(converted);
                }
            }

            /*
            //convert to 2D
            FVector current = innerMap.at(i).at(yRow);

            //ypos + i, height offset
            FVector2D converted(y * realScale + i * terrainCreator::ONEMETER, current.Z);
            anchors.push_back(converted);*/
        }
    }
    return anchors;
}




void terrainCreator::chunk::applyHeight(
    FVector2D aIn,
    FVector2D bIn
){
    int xa = aIn.X;
    int xIndex = convertToInnerIndex(xa);
    if (xIndex < 0 || xIndex >= innerMap.size()) {
        // Handle the error, or correct the index
        DebugHelper::showScreenMessage("x invalid", xIndex, FColor::Red);
        return; // or throw an error
    }else{
        DebugHelper::showScreenMessage("x valid", xIndex, FColor::Green);
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
/// @param value 
/// @return 
int terrainCreator::chunk::convertToInnerIndex(int value){
    int total = terrainCreator::CHUNKSIZE * terrainCreator::ONEMETER;
    //scale down
    int mod = value % total;
    int toIndex = mod / terrainCreator::ONEMETER;
    return toIndex;

    // erzeugt hunderter werte obwohl nicht gewollt

    value = (int)(value % total);
    return value;
}

int terrainCreator::chunk::clampOuterYIndex(FVector2D &a){
    int total = terrainCreator::CHUNKSIZE * terrainCreator::ONEMETER;
    int yToCheck = a.Y;
    int lowerRange = y * total;
    int higherRange = lowerRange + total;
    //inside bounds
    if(yToCheck >= lowerRange && yToCheck <= higherRange){
        yToCheck = (int)(yToCheck % total); //converts to centimeters
        //to meter
        yToCheck /= terrainCreator::ONEMETER;
        return yToCheck;
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

void terrainCreator::chunk::plot(UWorld *world, FColor color){
    int a = round(terrainCreator::ONEMETER / 2.1f);
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

    //works as expected
    cleanValues(outputData);

    //sort by x coordinates, later by y
    std::sort(outputData.begin(), outputData.end(), [](const FVector2D& A, const FVector2D& B) {
        return A.X < B.X;
    });
    debugDrawCurve(world, outputData, FColor::Green);

    applyTopViewCurveToMap(outputData);


    //testing
    plotAllChunks(world, FColor::Cyan);
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

void terrainCreator::processTouple(FVector2D &a, FVector2D &b){

    //debug
    FVector offset(0, 0, terrainCreator::ONEMETER / 2);
    FVector aC(a.X, a.Y, terrainCreator::ONEMETER);
    FVector bC(b.X, b.Y, terrainCreator::ONEMETER);
    aC += offset;
    bC += offset;
    DebugHelper::showLineBetween(worldPointer, aC, bC);

    //apply
    //create mod chunk size to get index
    int chunkMeters = terrainCreator::CHUNKSIZE * terrainCreator::ONEMETER;

    int ax = (int)a.X;
    int xChunk = (int)(ax / chunkMeters);
    int xIndex = (int)(ax % chunkMeters);

    int yBottomIndex = (int)(a.Y / chunkMeters);
    int yTopIndex = (int)(b.Y / chunkMeters);

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

    xChunk = validateIndex(xChunk);
    yBottomIndex = validateIndex(yBottomIndex);
    yTopIndex = validateIndex(yTopIndex);

    //iterate over y axis
    for(int i = yBottomIndex; i < yTopIndex + 1; i++){
        terrainCreator::chunk *c = &map.at(xChunk).at(i);
        if(c != nullptr){
            //c->plot(worldPointer, FColor::Yellow, 120); // testing

            int xCopy = (int)floor(a.X);
            c->applyHeight(a, b);
        }
    }
    
}


/// @brief clamps x and y to map size!
/// will clamp x and y values and clean the map to One meter gaps
/// @param vec vector to clean
void terrainCreator::cleanValues(std::vector<FVector2D> &vec){
    int i = 1;
    int size = vec.size();
    while(i < size){
        
        if(i > 0){
            int xClampedPrev = (int)(vec.at(i - 1).X / terrainCreator::ONEMETER);
            int yClampedPrev = round(vec.at(i - 1).Y / terrainCreator::ONEMETER);

            int xClamped = (int)(vec.at(i).X / terrainCreator::ONEMETER);
            
            if(xClampedPrev == xClamped){

                //override x value
                vec.at(i - 1).X = xClamped * terrainCreator::ONEMETER; //wieder hoch skalieren
                //clamp and override y value
                vec.at(i - 1).Y = yClampedPrev * terrainCreator::ONEMETER;

                //erase prev
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
    //get all x and y axis and smooth them. 

    //first smooth all x rows, then add y and devide by two id say
    
    // ---> wenn sich der std vector in grösse nicht ändert darf ich auch pointer darauf haben
    /// weil dann keine daten kopiert werden oder weg genommen

    bezierCurve curve;

    //all x columns
    std::vector<std::vector<FVector2D>> allColumns;
    for (int i = 0; i < map.size(); i++)
    {
        for (int innerX = 0; innerX < terrainCreator::CHUNKSIZE; innerX++)
        {
            std::vector<FVector2D> column;
            for (int j = 0; map.at(i).size(); j++)
            {
                terrainCreator::chunk &current = map.at(i).at(j);
                std::vector<FVector2D> copy = current.getXColumAnchors(innerX);
                column.insert(column.end(), copy.begin(), copy.end());
            }

            std::vector<FVector2D> output;
            curve.calculatecurve(column, output);
            cleanValues(output);

            allColumns.push_back(output);
        }
    }

    //all y columns
    std::vector<std::vector<FVector2D>> allRows;
    for (int i = 0; i < map.at(0).size(); i++) //i is up
    {
        for (int innerY = 0; innerY < terrainCreator::CHUNKSIZE; innerY++)
        {
            std::vector<FVector2D> row;
            for (int j = 0; map.size(); j++)
            {                                                     // j is sideways
                terrainCreator::chunk &current = map.at(j).at(i); //flipped on purpose to match x and y

                std::vector<FVector2D> copy = current.getYRowAnchors(innerY);
                row.insert(row.end(), copy.begin(), copy.end());
            }

            std::vector<FVector2D> output;
            curve.calculatecurve(row, output);
            cleanValues(output);

            allRows.push_back(output);
        }
    }




    //merge or apply individually xcolums and yrows to map 
}




/// @brief 
/// @param index 
/// @param column 
void terrainCreator::applyXColumnToMap(int index, std::vector<FVector> &column){

    int moddedForChunk = (int)(index / terrainCreator::CHUNKSIZE);

}



void terrainCreator::applyYRowToMap(int index, std::vector<FVector> &column){

}






void terrainCreator::plotAllChunks(UWorld * world, FColor color){
    if(world != nullptr){

        for (int i = 0; i < map.size(); i++){
            for (int j = 0; j < map.at(i).size(); j++){
                map.at(i).at(j).plot(world, color);
            }
        }
    }
}