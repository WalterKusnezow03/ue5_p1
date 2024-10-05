// Fill out your copyright notice in the Description page of Project Settings.


#include "bezierCurve.h"

bezierCurve::bezierCurve()
{
    EinheitsValue = 100;
    stepsToMakePerEinheitsValue = 1;
}

bezierCurve::~bezierCurve()
{
}

/// @brief will calculate the spline for you
/// @param ref reference anchor points (p0s and p3s)
/// @param output output vector to save in, MUST BE CLEAR
/// @param _einheitsValue einheits value between vectors, for example 100cm unreal engine scale
/// @param _stepsPerEinheitsValue steps to have per einheits value: for example 2, each 50cm
void bezierCurve::calculatecurve(
    std::vector<FVector2D> &ref, 
    TVector<FVector2D> &output,
    //std::vector<FVector2D> &output,
    float _einheitsValue,
    float _stepsPerEinheitsValue
){
    /**
     * error handeling
     */
    if(ref.size() < 3){ //if points are for example 1: the code wont be createable, weird issues occur
        return;
    }

    /**
     * curve code here
     */
    EinheitsValue = std::abs(_einheitsValue); //positive values only
    stepsToMakePerEinheitsValue = std::abs(_stepsPerEinheitsValue);
    if(stepsToMakePerEinheitsValue == 0 || EinheitsValue == 0){
        //ISSUE NOT ALLOWED DIVISION BY ZERO
        return;
    }

    
    //create and add curve anchors
    createContinuityCurve(ref);


    //interpolate
    processAllPoints(ref, output); //what if you do it more than once
   
}

/// @brief predicts the element count for the finally interpolated bezier curve 
/// @param anchors 
/// @return 
int bezierCurve::predictFinalCurveElementCount(std::vector<FVector2D> &anchors){
    int sum = 0;
    for (int i = 1; i < anchors.size(); i++){
        sum += ((int)FVectorUtil::Dist(anchors.at(i - 1), anchors.at(i)) / EinheitsValue);
    }

    return (int)(sum * 1.2f); //120% for safety
}

/// @brief iterates over the anchors and moves them closer on y axis together based on their distance on x
/// @param ref to smooth out
void bezierCurve::smoothAnchors(std::vector<FVector2D> &ref){
    float decreaseFrac = 0.1f; //percent
    for (int i = 1; i < ref.size(); i++)
    {
        FVector2D &a = ref[i-1];
        FVector2D &b = ref[i];
        float distX = std::abs(a.X - b.X);
        float distY = std::abs(a.Y - b.Y);
        if(distX / 2 < distY){
            if(a.Y < b.Y){
                b.Y = std::max(b.Y - distY * decreaseFrac, a.Y); //max to not go too small
            }
            if(a.Y > b.Y){
                b.Y = std::min(b.Y + distY * decreaseFrac, a.Y); //min to not go too big
            }
        }
    }
}

/// @brief lets say the anchors array only has anchors on no continuity helpers
/// @param anchors anchors to process
/// @param curve curve to save procesced anchors and continouity points in
/// @param g2Continuity enable g2 continoutiy or use g1
void bezierCurve::createContinuityCurve(std::vector<FVector2D> &anchors){
    if(anchors.size() < 2){
        return;
    }


    //data will be copied later
    std::vector<FVector2D> curve;


    //percent distance / one fraction to place p4 and p2 respectivley as control points
    //tangentially
    float BETA = 0.6f; //from p0 to p1 scaling
    float BETA_END = BETA / 2; //p2 to p3: keep second control point more to the end, 
                               //allows less sharp edges in theory

    //this code creates the curve and should generally create the curve as wanted

    int i = 1; 
    while(i < anchors.size()){

        if(i == 1){
            //first part
            FVector2D p0 = anchors.at(i-1); //start
            FVector2D p3 = anchors.at(i);  // at(i); //end

            FVector2D direction = (p3 - p0);

            //is just linear at first
            FVector2D p1 = p0 + direction * BETA_END; //was BETA, was made beta end to make less agressivee in beginning
            FVector2D p2 = p3 - direction * BETA_END; // symetrical to first control point

            FVector2D p4 = p3 + (p3 - p2) * BETA; //next anchor, next p1, continuity

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
            

            //p1 kommt von vorher aber man sollte p4 so wählen es sich an den nächsten 
            //anker punkt anschmiegen wenn möglich
            if(i < anchors.size() - 1){
                FVector2D p6 = anchors.at(i + 1);
                FVector2D dirToNext = (p6 - p3);
                FVector2D p4 = p3 + dirToNext * BETA; // AB = B - A //p1 für next
                FVector2D p2 = p3 - dirToNext * BETA_END; //creating own anchor p2

                curve.push_back(p2);
                curve.push_back(p3);
                curve.push_back(p4); //push last but only if not at end of curve

            }else{

                //last interpolation
                FVector2D dir = (p3 - p0);


                FVector2D p2 = p3 - dir * BETA_END; //p2 is now very aligned to p1


                //next p1 point
                FVector2D p4 = p3 + (p3 - p2) * BETA; 

                //p0 and p1 already in list
                curve.push_back(p2);
                curve.push_back(p3);
            }

            
            i++;
        }

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

}






/// @brief 
/// @param points anchors to create the curve from, points will be overriden 
/// @param reserve space to be resevered (which was also reserved for points vector)
void bezierCurve::processAllPoints(
    std::vector<FVector2D> &points,
    TVector<FVector2D> &output
){

    //the curves overlap like this
    //index:      [0] [1] [2] [3] [4] //which makes i += 3, should be correct
    //first part: p0, p1, p2, p3, p4next
    //second part:            p0, p1, p2, p3
    //                               [5], [6]

    int next = 3;
    int size = points.size() - 3;
    for (int i = 0; i < size; i += next)
    {
        process4Points(points, i, output);
    }
}

/// @brief will process the cubic bezier part
/// @param points points to process from
/// @param offset offset, index must be p0
/// @param output output to save in
void bezierCurve::process4Points(
    std::vector<FVector2D> &points, 
    int offset, 
    TVector<FVector2D> &output
){
    if(offset + 3 >= points.size()){
        return;
    }

    //std::vector<FVector2D> tmpListCurve;

    FVector2D &p0 = points[offset];
    FVector2D &p1 = points[offset + 1];
    FVector2D &p2 = points[offset + 2];
    FVector2D &p3 = points[offset + 3];

    
    float distance = FVectorUtil::Dist(p0, p3);
    float step = (distance / EinheitsValue) / stepsToMakePerEinheitsValue;
    if(step == 0 || step <= 0.01f){ //100max z.b.
        return;
    }
    step = 1.0f / step; //to percentage frac of 1



    float limit = 1.0f; //fixing weird overlap on curves by cutting them off, 0.6f
    limit = 0.8f; //fix over interpolating
    FVector2D prev(-100, 0); //first not in curve vector.
    for (float i = 0; i <= limit; i += step)
    {
        FVector2D newPos = FVector2DFourAnchorBezier(p0, p1, p2, p3, i);
        newPos.X = (int)(newPos.X);
        
        // new code
        if(output.size() > 0){
            
            if(prev.X != newPos.X){
                output.push_back(newPos);
                
            }
        }else{
            output.push_back(newPos);
        }

        //copy for next
        prev = newPos;

    }

    //fillGaps(output);

}





FVector2D bezierCurve::FVector2DFourAnchorBezier(
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








/**
 * 
 * FILL GAPS
 * 
 */

/// @brief fills gaps larger than one
/// @param vec 
void bezierCurve::fillGaps(TVector<FVector2D> &vec){

    int i = 1;
    int size = vec.size();
    while (i < size){

        FVector2D prev = vec.at(i - 1);
        FVector2D current = vec.at(i);

        if(std::abs(prev.X - current.X) > (EinheitsValue / stepsToMakePerEinheitsValue)){ //must be converted to average distance!!
            //std::vector<FVector2D> fill;
            TVector<FVector2D> fill;
            linearInterpolate(prev, current, fill);

            // insert
            // v.insert(startpoint, other.begin(), other.end());
            //vec.insert(vec.begin() + i, fill.begin(), fill.end());
            vec.insert(i, fill);
            i += fill.size(); //to next
        }
        i++; //to next default

        size = vec.size();
    }


}

/// @brief linear interpolate from a to b
/// @param a start
/// @param b end
/// @param container container to save in  
void bezierCurve::linearInterpolate(FVector2D &a, FVector2D &b, TVector<FVector2D> &container){
    if(stepsToMakePerEinheitsValue != 0){
        float dist = std::abs(a.X - b.X);
        float step = (EinheitsValue / stepsToMakePerEinheitsValue);
        if (dist > step)
        { // one (centi)meter by default

            float euklidDist = FVectorUtil::Dist(a, b);
            FVector2D dir = (b - a) / euklidDist; // normalized dir
            for (float i = 0; i < dist; i += step){
                FVector2D newPos = a + dir * i;
                container.push_back(newPos);
            }
        }
    }
    
}
