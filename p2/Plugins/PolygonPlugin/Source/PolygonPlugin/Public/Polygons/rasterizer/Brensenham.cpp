#include "Brensenham.h"




void Brensenham::BrensenhamLineFill(
    TArray<FIntPoint> &outPoints
){
    TArray<FIntPoint> tempBuffer;
    for (int i = 0; i < outPoints.Num() - 1; i++){
        int j = i + 1;
        BrensenhamLineAppend(
            outPoints[i],
            outPoints[j],
            tempBuffer
        );
    }
    outPoints = tempBuffer;
}

void Brensenham::BrensenhamLineAppend(
    const FIntPoint &Start, 
    const FIntPoint &End,
    TArray<FIntPoint> &outPoints
){
    //outPoints.Empty(); //dont Empty

    int x0 = Start.X;
    int y0 = Start.Y;
    int x1 = End.X;
    int y1 = End.Y;

    int dx = FMath::Abs(x1 - x0);
    int dy = FMath::Abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    while (true){
        outPoints.Add(FIntPoint(x0, y0));
        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;
        if (e2 > -dy){
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx){
            err += dx;
            y0 += sy;
        }
    }
}