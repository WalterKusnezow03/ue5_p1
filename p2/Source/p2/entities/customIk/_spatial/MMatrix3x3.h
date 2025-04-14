#pragma once

#include "CoreMinimal.h"

class P2_API MMatrix3x3{

public:
    MMatrix3x3();
    ~MMatrix3x3();
    MMatrix3x3(const MMatrix3x3 &other);
    MMatrix3x3(FRotator &other);
    MMatrix3x3 &operator=(const MMatrix3x3 &other);

    void makeIdentity();

    static MMatrix3x3 skewMatrix(FVector &other);

    MMatrix3x3 operator*(const MMatrix3x3 &other);
    MMatrix3x3 operator+(const MMatrix3x3 &other);
    FVector operator*(FVector &other);

    void operator*=(MMatrix3x3 &other);
    void operator*=(float skalar);

    void operator+=(MMatrix3x3 &other); // cij = aij + bij
    

    static float degToRadian(float deg);
    static float radToDegree(float rad);

    void rollRadAdd(float a);
    void pitchRadAdd(float a);
    void yawRadAdd(float a);


    MMatrix3x3 createRotatorFrom(FVector &other);
    MMatrix3x3 createRotatorFrom(FVector &other, FVector2D XAxis, FVector2D ZAxis);

    float unsignedAngleRadBetween(FVector2D &a, FVector2D &b);
    float signedAngleRadBetween(FVector2D &a, FVector2D &b);
    float signForAngle(FVector2D &a, FVector2D &b);
    
    
    void operator/=(int skalar);
    MMatrix3x3 expMatrix(int detail);

    FRotator extractRotator();

private:
    void fakultaet(int value, std::vector<int> &output);

    static constexpr int size = 9;
    float array[9];

    void set(int column, int row, float value);
    float get(int columnX, int rowY);
};