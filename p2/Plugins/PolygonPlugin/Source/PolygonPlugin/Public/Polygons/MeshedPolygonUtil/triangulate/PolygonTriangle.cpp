#include "PolygonTriangle.h"


void FPolygonTriangle::Setup(
    const std::pair<int,int> &v0In, 
    const std::pair<int,int> &v1In, 
    const std::pair<int,int> &v2In
){
    FIntPoint v0Int(v0In.first, v0In.second);
    FIntPoint v1Int(v1In.first, v1In.second);
    FIntPoint v2Int(v2In.first, v2In.second);
    Setup(v0Int, v1Int, v2Int);
}

void FPolygonTriangle::Setup(FIntPoint &v0In, FIntPoint &v1In, FIntPoint &v2In){
    v0 = v0In;
    v1 = v1In;
    v2 = v2In;

    e0 = v1 - v0;
    e1 = v2 - v1;
    e2 = v0 - v2;

    FIntPoint e0e2 = v2 - v0; //AB = B - A
    normalSignTriangle = NormalSignInt(e0, e0e2);
}

bool FPolygonTriangle::InsideTriangle(int x, int y) const{
    FIntPoint asPoint(x, y);
    return InsideTriangle(asPoint);
}

bool FPolygonTriangle::InsideTriangle(FIntPoint &other) const{
    /*bool signA = NormalSign(v0, e0, other);
    bool signB = NormalSign(v1, e1, other);
    bool signC = NormalSign(v2, e2, other);
    //return signA == signB && signB == signC;*/
    if(IsDegenerate()){
        return false;
    }

    int a = NormalSignInt(v0,e0,other);
    int b = NormalSignInt(v1,e1,other);
    int c = NormalSignInt(v2,e2,other);

    if(normalSignTriangle > 0){
        return a >= 0 && b >= 0 && c >= 0;
    }
    else{
        return a <= 0 && b <= 0 && c <= 0;
    }
    return false;

    /*return 
    (signA >= 0 && signB >= 0 && signC >= 0) ||
    (signA <= 0 && signB <= 0 && signC <= 0);*/
}

bool FPolygonTriangle::NormalSign(const FIntPoint &vertex, const FIntPoint &edge, const FIntPoint &pTest) const {
    int z = NormalSignInt(vertex, edge, pTest);
    if (z == 0){
        return false;
    }

    return z > 0;
}

int FPolygonTriangle::NormalSignInt(const FIntPoint &vertex, const  FIntPoint &edge, const FIntPoint &pTest)const{
    FIntPoint relative = pTest - vertex; // AB = B - A
    return NormalSignInt(relative, edge);
}

int FPolygonTriangle::NormalSignInt(const FIntPoint &edge0, const  FIntPoint &edge1)const{
    //n.Z = ax by - ay bx
    int z = edge0.X * edge1.Y - edge0.Y * edge1.X;
    return z;
}




bool FPolygonTriangle::AnyInsideIgnoreSupportPoints(const TArray<std::pair<int, int>> &positions){
    for (int i = 0; i < positions.Num(); i++){
        const std::pair<int, int> &current = positions[i];
        if (!IsSupportPoint(current)){
            if(InsideTriangle(current.first, current.second)){
                return true;
            }
        }
    }
    return false;
}

bool FPolygonTriangle::IsSupportPoint(const std::pair<int, int> &pos){
    FIntPoint asPoint(pos.first, pos.second);
    return IsSame(asPoint, v0) || IsSame(asPoint, v1) || IsSame(asPoint, v2);
}

bool FPolygonTriangle::IsSame(FIntPoint &a, FIntPoint &b){
    return a.X == b.X && b.Y == a.Y;
}

bool FPolygonTriangle::GetNormalSign(){
    return normalSignTriangle > 0;
}

bool FPolygonTriangle::IsDegenerate()const{
    float area = std::abs(normalSignTriangle) / 2.0f; //A = sizeNormal / 2
    return area <= 0.0f;
}