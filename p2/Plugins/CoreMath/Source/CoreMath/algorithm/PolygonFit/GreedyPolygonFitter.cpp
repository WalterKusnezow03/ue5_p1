#include "GreedyPolygonFitter.h"


GreedyPolygonFitter::GreedyPolygonFitter(){

}

GreedyPolygonFitter::~GreedyPolygonFitter(){

}


void GreedyPolygonFitter::SetOutlineShape(const TArray<FVector2D> &shapeIn){
    outlineShape = shapeIn;
}