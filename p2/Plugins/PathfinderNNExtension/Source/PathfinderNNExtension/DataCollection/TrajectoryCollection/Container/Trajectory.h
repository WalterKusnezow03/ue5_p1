#pragma once

#include "CoreMinimal.h"

class PATHFINDERNNEXTENSION_API Trajectory {

public:

    Trajectory(FVector &posIn){
        pos = posIn;
        timeStamp = FPlatformTime::Seconds();
    }

    void operator-=(const FVector &posIn){
        //AB = B - A
        pos -= posIn;
    }

    void operator-=(float time){
        timeStamp -= time;
    }

    FVector GetPosition() const {
        return pos;
    }

    float GetTime() const {
        return timeStamp;
    }

private:
    float timeStamp = 0.0f;
    FVector pos;
};