#include "CurveRasterizer.h"
#include "Brensenham.h"

CurveRasterizer::CurveRasterizer(){

}

CurveRasterizer::~CurveRasterizer(){

}


void CurveRasterizer::RasterizeVerteciesIntBrensenham(
    TArray<FIntPoint> &polygon
){
    Brensenham::BrensenhamLineFill(polygon);
}

void CurveRasterizer::RasterizeVerteciesInt(
    TArray<FIntPoint> &polygon
){
    RasterizeVerteciesToInt(polygon, 1);
}

void CurveRasterizer::RasterizeVerteciesToInt(
    TArray<FIntPoint> &polygon,
    int widthOfStep
){
    float asFloat = widthOfStep;
    TArray<FVector> converted;
    Convert(polygon, converted);
    RasterizeVerteciesTo(converted, asFloat); //since int 1 step.
    Convert(converted, polygon);
}

void CurveRasterizer::Convert(
    const TArray<FIntPoint> &raw, 
    TArray<FVector> &outData
){
    if(raw.Num() > 0){
        outData.SetNum(raw.Num());
        for (int i = 0; i < raw.Num(); i++){
            const FIntPoint &current = raw[i];
            FVector &currentOut = outData[i];
            currentOut.X = current.X;
            currentOut.Y = current.Y;
            currentOut.Z = 0.0f;
        }
    }
}

void CurveRasterizer::Convert(
    const TArray<FVector> &raw, 
    TArray<FIntPoint> &outData
){
    if(raw.Num() > 0){
        outData.SetNum(raw.Num());
        for (int i = 0; i < raw.Num(); i++){
            const FVector &current = raw[i];
            FIntPoint &currentOut = outData[i];

            currentOut.X = FMath::FloorToInt(current.X);
            currentOut.Y = FMath::FloorToInt(current.Y);
           
        }
    }
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
    modLowerRelative.Z = 0.0f;
    // FVector modHigherRelative = modHigher - relative;

    //stepping coordinate must be always right off, inside the polygon
    if(FVector::DotProduct(normal, modLowerRelative) <= 0.0f){
        //return modHigher;
        return modLower;
    }
    //return modLower;
    return modHigher;
}

FVector CurveRasterizer::ToModCoordinate(FVector &pos, int mod, int dir){
    /*int x = pos.X;
    int y = pos.Y;
    x += dir * (x % mod);
    y += dir * (y % mod);
    return FVector(x, y, pos.Z);*/

    float asFloatMod = mod;
    float x = pos.X;
    float y = pos.Y;

    if (dir < 0) {
        // Abrunden auf das vorherige Grid-Vielfache
        x = FMath::FloorToFloat(x / asFloatMod) * mod;
        y = FMath::FloorToFloat(y / asFloatMod) * mod;
    } else {
        // Aufrunden auf das nächste Grid-Vielfache
        x = FMath::CeilToFloat(x / asFloatMod) * mod;
        y = FMath::CeilToFloat(y / asFloatMod) * mod;
    }

    return FVector(x, y, pos.Z);
}





