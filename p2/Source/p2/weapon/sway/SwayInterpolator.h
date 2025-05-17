#pragma once

#include "CoreMinimal.h"

class P2_API SwayInterpolator{

public:
    SwayInterpolator();
    ~SwayInterpolator();


    void setMotionTime(float time);
    void setHeightOfWave(float y, float z);

    FVector Tick(float deltatime);
    bool reachedLoop();

    void enableSwayFlag(bool flag);

private:
    bool reachedPeakFlag = false;

    float deltaTimeSum = 0.0f;

    float fullMotionTime = 0.0f;
    float fullMotionStretchSinWaveDenominator = 0.0f;

    float heightY = 1.0f;
    float heightZ = 1.0f;

    float timeSumSign = 1.0f;
    bool swayEnabled = true;
    bool stopSway = false;

    bool DEBUG_BLOCKSWAY = true;
};