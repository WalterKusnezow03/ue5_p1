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

    void operator/=(float time){
        if(std::abs(time) > 0.0001f){
            timeStamp /= time;
        }
    }

    FVector GetPosition() const {
        return pos;
    }

    float GetTime() const {
        return timeStamp;
    }

    //chooses the absolute maximum
    float AbsMaxTime(float other) const {
        float t = GetTime();
        other = std::max(std::abs(t), other);
        return other;
    }

private:
    float timeStamp = 0.0f;
    FVector pos;
};