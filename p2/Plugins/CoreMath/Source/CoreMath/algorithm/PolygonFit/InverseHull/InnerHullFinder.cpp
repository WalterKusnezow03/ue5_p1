#include "InnerHullFinder.h"
#include "CoreMath/algorithm/GrahamScan/GrahamScan2D.h"

InnerHullFinder::InnerHullFinder(){

}

InnerHullFinder::~InnerHullFinder(){

}




void InnerHullFinder::MakeMostInnerHull(TArray<FVector2D> &shape){

    //solange der vertex count der hülle, eliminated points >= 3 ist,
    //ist es eine valide most inner hull.

    //solange die convexe hülle abtragen bis das kleine polygon was valide ist
    //convex überig bleibt.

    GrahamScan2D scanner;
    int maxIterations = 100;
    for (int i = 0; i < maxIterations; i++){
        TArray<FVector2D> potentialNewInner;
        scanner.ComputeConvexHull(shape, potentialNewInner);
        if(potentialNewInner.Num() >= 3){
            shape = potentialNewInner;
        }else{
            return; //finished, no smaller shape found.
        }
    }
}