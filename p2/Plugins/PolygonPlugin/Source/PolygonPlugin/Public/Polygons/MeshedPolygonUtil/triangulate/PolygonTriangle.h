#pragma once

#include "CoreMinimal.h"


class POLYGONPLUGIN_API FPolygonTriangle {

public:
    
    void Setup(
        const std::pair<int,int> &v0In, 
        const std::pair<int,int> &v1In, 
        const std::pair<int,int> &v2In
    );
    void Setup(FIntPoint &v0In, FIntPoint &v1In, FIntPoint &v2In);

    bool InsideTriangle(int x, int y) const;
    bool InsideTriangle(FIntPoint &other) const;

    bool GetNormalSign();

    bool AnyInsideIgnoreSupportPoints(const TArray<std::pair<int, int>> &positions);

    bool IsDegenerate()const;

private:
    FIntPoint v0, v1, v2;
    FIntPoint e0, e1, e2;
    int normalSignTriangle = 0;

    bool NormalSign(const FIntPoint &vertex, const FIntPoint &edge, const FIntPoint &pTest) const;
    int NormalSignInt(const FIntPoint &vertex, const FIntPoint &edge, const FIntPoint &pTest) const;
    int NormalSignInt(const FIntPoint &edge0, const FIntPoint &edge1) const;


    bool IsSupportPoint(const std::pair<int, int> &pos);
    bool IsSame(FIntPoint &a, FIntPoint &b);
};