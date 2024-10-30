// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/meshgen/vornoi/VPolygon.h"

VPolygon::VPolygon()
{
}

VPolygon::VPolygon(std::vector<FVector> &verteciesIn){
    vertecies = verteciesIn; //hier müsste der copy constructor gecallt werden
    sort();
}

VPolygon::~VPolygon()
{
}



void VPolygon::sort(){
    ComputeConvexHull(vertecies);
}

//will compute the convex hull and write into the passed array
void VPolygon::ComputeConvexHull(std::vector<FVector> &points) { //passed by reference also kann ich reinkritzeln
    std::vector<FVector> convexHull; //value, will be removed from stack

    if (points.size() < 3) {
        return;
    }

    // Sort points to calculate the lower hull
    //Z is up and ignored here.
    std::sort(points.begin(), points.end(), [](const FVector& a, const FVector& b) {
        if (a.X == b.X) {
            return a.Y < b.Y;
        }
        return a.X < b.X;
    });

    // Calculate the lower hull
    for (const FVector& point : points) {
        while (convexHull.size() >= 2 && !IsClockwise(convexHull[convexHull.size() - 2], convexHull.back(), point)) {
            convexHull.pop_back();
        }
        convexHull.push_back(point);
    }

    // Calculate the upper hull
    int lowerHullCount = convexHull.size();
    for (int i = points.size() - 2; i >= 0; --i) {
        FVector point = points[i];
        while (
            convexHull.size() > lowerHullCount && 
            !IsClockwise(convexHull[convexHull.size() - 2], convexHull.back(), point)
        ) {
            convexHull.pop_back();//den geprüften punkt poppen
        }
        convexHull.push_back(point); //neuen punkt pushen
    }

    // Remove the last point which might be duplicated
    if (!convexHull.empty()) {
        convexHull.pop_back();
    }


    //copy data to passed array
    int cSize = convexHull.size();
    points = convexHull; //zuweisung / überschreibung
    
}

/// @brief checks if the point is clockwise rotated for the graham scan
/// @param a 
/// @param b 
/// @param c 
/// @return 
bool VPolygon::IsClockwise(const FVector& a, const FVector& b, const FVector& c) {
    return (b.X - a.X) * (c.Y - a.Y) - (b.Y - a.Y) * (c.Y - a.Y) < 0;
    // < 0 means, kolliniear vectors are kept and ignored.
}


/*
/// @brief wird in beide richtungen geprüft auch schnittpunkt
/// @param a 
/// @param b 
/// @param start 
/// @param direction 
/// @return 
bool VPolygon::doesIntersect(FVector &a, FVector &b, FVector&start, FVector &direction, FVector &output){
    //direkt schnittpunkt mit geraden gleichung ausrechnen
    //dann prüfen ob der schnittpunkt in bounds of a und b liegt (skalarprodukt entgegengesetzt)

    //gx: g + r * gdir
    //fx: f + s * fdir

    float g1 = a.X;

    return false;
}*/