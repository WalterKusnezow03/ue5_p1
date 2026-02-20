#include "CurveRasterizer.h"


CurveRasterizer::CurveRasterizer(){

}

CurveRasterizer::~CurveRasterizer(){

}


void CurveRasterizer::RasterizeVerteciesTo(
    TArray<FVector> &polygon,
    float widthOfInsideStep
){
    for (int i = 0; i < polygon.Num(); i++){
        FVector &current = polygon[i];
        FVector &next = polygon[(i + 1) % polygon.Num()];
        FVector connect = next - current; //AB = B - A
        FVector normal(connect.Y * -1.0f, connect.X, 0.0f);

        FVector modLower = ToModCoordinate(current, widthOfInsideStep, -1);
        FVector modHigher = ToModCoordinate(current, widthOfInsideStep, 1);

        //stepping coordinate must be always right off, inside the polygon
        polygon[i] = ChooseRightOffCoordinateRelativeTo(modLower, modHigher, normal, current);
    }
}


//refernce is only temporary by given coordinates
FVector& CurveRasterizer::ChooseRightOffCoordinateRelativeTo(
    FVector &modLower,
    FVector &modHigher,
    FVector &normal,
    FVector &relative
){
    FVector modLowerRelative = modLower - relative; // AB = B - A
    //FVector modHigherRelative = modHigher - relative;

    //stepping coordinate must be always right off, inside the polygon
    if(FVector::DotProduct(normal, modLowerRelative) <= 0.0f){
        //return modHigher;
        return modLower;
    }
    //return modLower;
    return modHigher;
}

FVector CurveRasterizer::ToModCoordinate(FVector &pos, int mod, int dir){
    int x = pos.X;
    int y = pos.Y;
    x += dir * (x % mod);
    y += dir * (y % mod);
    return FVector(x, y, pos.Z);
}