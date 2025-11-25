#pragma once

#include "CoreMinimal.h"

class COMPUTERVISIONPLUGIN_API KeyPoint
{
public:
    KeyPoint();
    ~KeyPoint();

    KeyPoint(const KeyPoint &other);

    KeyPoint &operator=(const KeyPoint &other);

    KeyPoint(int xin, int yin, float sigmaIn, float angleIn);

    void Dimensions(int windowSize, int &outX, int &outY, int &outEndX, int &outEndY);

private:
    int x;
    int y;
    float sigma;   // scale
    float angle;   // orientation (next step)
};