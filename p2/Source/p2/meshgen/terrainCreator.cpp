// Fill out your copyright notice in the Description page of Project Settings.

#include <cmath>
#include "p2/util/FVectorUtil.h"
#include "Algo/Sort.h"  // Include the necessary header
#include "terrainCreator.h"

terrainCreator::terrainCreator()
{
}

terrainCreator::~terrainCreator()
{
}

terrainCreator::chunk::chunk()
{

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

//chunk methods









//terrain methods

void terrainCreator::createterrain(UWorld *world, int meters){
    worldPointer = world;

    int chunks = meters / terrainCreator::CHUNKSIZE; //to chunks
    int detail = CHUNKSIZE; // 1 by 1 detail

    for (int i = 0; i < chunks; i++){
        std::vector<terrainCreator::chunk> vec;
        for (int j = 0; j < chunks; j++){
            chunk c;
            vec.push_back(c);
        }
        map.push_back(vec);
    }

    

    //get anchors...
    std::vector<FVector2D> anchors = createSamplePoints(); //= getAnchors() to be implemented

    //achutung, die resevierten elemente sollten die distanz zwischen allen punkten sein
    //wenn ich 1 / 1 die bezier kurve erstelle habe ich am ende auch genauso viele elemente darin MAXIMAL
    //Sollte die kurve mehr vertikal sein natürlich nicht.
    int reserveSize = predictFinalCurveElementCount(anchors);
    anchors.reserve(reserveSize);

    

    //data will be overriden
    //reserve more space for the vector
    createContinuityCurve(anchors);


    //create the whole curve
    std::vector<FVector2D> output;
    output.reserve(reserveSize); //reserve size before processing and adding for safety
    processAllPoints(anchors, output); //what if you do it more than once

    fillGaps(output);

    //draw final curve
    debugDrawCurve(world, output, FColor::Red);

    //sort by x coordinates, later by y
    std::sort(output.begin(), output.end(), [](const FVector2D& A, const FVector2D& B) {
        return A.X < B.X;
    });
}


std::vector<FVector2D> terrainCreator::createSamplePoints(){
    std::vector<FVector2D> output;

    int scale = 10; //upscale to meters 

    //testing simple line shape
    int xstep = 2;
    int x = 0;
    int nums[] = {2, 1, 4, 5, 2, 3, 4, 1, 2};
    for (int i : nums){
        FVector2D a(x, i);
        x += xstep;
        a *= scale;
        output.push_back(a);
    }

        
    /*for (int i = 0; i < output.size(); i++){
        output.at(i) *= 10;
    }*/
    return output;
}

/// @brief predicts the element count for the finally interpolated bezier curve 
/// @param anchors 
/// @return 
int terrainCreator::predictFinalCurveElementCount(std::vector<FVector2D> &anchors){
    int sum = 0;
    for (int i = 1; i < anchors.size(); i++){
        sum += (int)FVectorUtil::Dist(anchors.at(i - 1), anchors.at(i));
    }
    return (int)(sum * 1.2f); //120% for safety
}

/// @brief lets say the anchors array only has anchors on no continuity helpers
/// @param anchors anchors to process
/// @param curve curve to save procesced anchors and continouity points in
/// @param g2Continuity enable g2 continoutiy or use g1
void terrainCreator::createContinuityCurve(std::vector<FVector2D> &anchors){
    if(anchors.size() < 3){
        return;
    }

    //data will be copied later
    std::vector<FVector2D> curve;

    float beta = 0.5; //0.5 //to large positiive: schleife hickup issue
    float close = 0.5f; // 0.1 , to large values, more than 1/2 lead to abrupt changes

    beta = 1.5f;

    //this code creates the curve and should generally create the curve as wanted

    int i = 1; 
    while(i < anchors.size()){

        if(i == 1){
            //first part
            FVector2D p0 = anchors.at(i-1); //start
            FVector2D p3 = anchors.at(i);  // at(i); //end

            FVector2D direction = (p3 - p0);

            FVector2D p1 = p0 + direction * close;
            FVector2D p2 = p3 - direction * close; // symetrical to first control point

            FVector2D p4 = p3 + (p3 - p2) * beta; //next anchor

            curve.push_back(p0);
            curve.push_back(p1);
            curve.push_back(p2);
            curve.push_back(p3);
            curve.push_back(p4);


            i++;
        }else{

            int index = curve.size();
            FVector2D p0 = curve.at(index - 2); //the one before p4 (alias p1)
            FVector2D p1 = curve.at(index - 1);

            FVector2D p3 = anchors.at(i); //next point from this paralell list
            FVector2D dir = (p3 - p0);

            FVector2D p2 = p3 - dir * close; //align to connection


            //next p1 point
            FVector2D p4 = p3 + (p3 - p2) * beta;

            //p0 and p1 already in list
            curve.push_back(p2);
            curve.push_back(p3);
            curve.push_back(p4);

            i++;
        }

        //create p4 to insert
    }


    //copy to anchors, i defined before
    i = 0;
    for (i = 0; i < curve.size(); i++)
    {
        if(i < anchors.size()){
            anchors.at(i) = curve.at(i); //copy
        }
        else
        {
            anchors.push_back(curve.at(i)); //add, critical section. Memory must be allocated before for safety
        }
    }

    //first point end start connect again
    //anchors.push_back(curve.front());
}

/// @brief 
/// @param points anchors to create the curve from, points will be overriden 
/// @param reserve space to be resevered (which was also reserved for points vector)
void terrainCreator::processAllPoints(
    std::vector<FVector2D> &points,
    std::vector<FVector2D> &output
){

    //the curves overlap like this
    //index:      [0] [1] [2] [3] [4] //which makes i += 3, should be correct
    //first part: p0, p1, p2, p3, p4next
    //second part:            p0, p1, p2, p3
    //                               [5], [6]

    int next = 3;
    for (int i = 0; i < points.size() - 3; i += next)
    {
        process4Points(points, i, output);
    }
}

void terrainCreator::process4Points(std::vector<FVector2D> &points, int offset, std::vector<FVector2D> &output){
    if(offset + 3 >= points.size()){
        return;
    }

    //std::vector<FVector2D> tmpListCurve;

    FVector2D p0 = points.at(offset);
    FVector2D p1 = points.at(offset + 1);
    FVector2D p2 = points.at(offset + 2);
    FVector2D p3 = points.at(offset + 3);

    

    //temporary output for clean up
    std::vector<FVector2D> tmp;
    float stepSize = 0.01f; //0.001
    float limit = 0.6f;  //fixing weird overlap on curves by cutting them off 

    //stepSize = 0.0005f; // Define a suitable step size //0.001
    for (float i = 0; i <= limit; i += stepSize) {
        FVector2D newPos = FVector2DFourAnchorBezier(p0, p1, p2, p3, i);
        tmp.push_back(newPos);


        //debugging
        //output.push_back(newPos);
    }

   

    //clean up output for consistent X values and copy ---> consider y clean up too
    
    for (int i = 0; i < tmp.size(); i++){
        FVector2D current = tmp.at(i);
        current.X = round(current.X);

        if(i > 0){
            FVector2D prev = output.back();
            if(prev.X != current.X){
                output.push_back(current);
            }
        }else{
            output.push_back(current);
        }
    }
}


void terrainCreator::fillGaps(std::vector<FVector2D> &vec){

    int i = 1;
    int size = vec.size();
    while (i < size){

        FVector2D prev = vec.at(i - 1);
        FVector2D current = vec.at(i);

        if(std::abs(prev.X - current.X) > 1){
            std::vector<FVector2D> fill;
            linearInterpolate(prev, current, fill);

            // insert
            // v.insert(startpoint, other.begin(), other.end());
            vec.insert(vec.begin() + i, fill.begin(), fill.end());
            i += fill.size(); //+ 1; //to next

            /*
            FString s = FString::Printf(TEXT("from %d to "), i);
            i += fill.size(); //+ 1; //to next
            DebugHelper::showScreenMessage(s, i, FColor::Red);
            debugDrawCurve(worldPointer, fill, FColor::Yellow);
            */
        }
        i++; //to next default

        size = vec.size();
    }


}

void terrainCreator::linearInterpolate(FVector2D &a, FVector2D &b, std::vector<FVector2D> &container){
    if(std::abs(a.X - b.X) > 1){ //one meter by default

        float dist = FVectorUtil::Dist(a, b);
        if(dist > 1){
            FVector2D dir = (b - a) / dist; //normalized dir
            float step = 1.0f / dist;


            for (float i = 0; i < 1.0f; i += step)
            {
                FVector2D newPos = a + i * dir;
                container.push_back(newPos);
            }
        }
        
    }
}




FVector2D terrainCreator::FVector2DFourAnchorBezier(
    FVector2D &a,
    FVector2D &b,
    FVector2D &c,
    FVector2D &d,
    float skalar
){
    FVector2D ab = a + skalar * (b - a);
    FVector2D bc = b + skalar * (c - b) ;
    FVector2D cd = c + skalar * (d - c) ;
    FVector2D abbc = ab + skalar * (bc - ab); //das ding dazwischen
    FVector2D bccd = cd + skalar * (cd - bc); //das ding dazwischen
    FVector2D abbcbccd = abbc + skalar * (bccd - abbc); //das ding dazwischen
    return abbcbccd;
}

float terrainCreator::yOffsetFourAnchorBezier(
    FVector2D &a,
    FVector2D &b,
    FVector2D &c,
    FVector2D &d,
    float skalar
){
    FVector2D ab = a + skalar * (b - a);
    FVector2D bc = b + skalar * (c - b) ;
    FVector2D cd = c + skalar * (d - c) ;
    FVector2D abbc = ab + skalar * (bc - ab); //das ding dazwischen
    FVector2D bccd = cd + skalar * (cd - bc); //das ding dazwischen
    FVector2D abbcbccd = abbc + skalar * (bccd - abbc); //das ding dazwischen
    return abbcbccd.Y;
}





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