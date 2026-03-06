#pragma once

#include "CoreMinimal.h"

#include "Timer.h"


class COREMATH_API FlipTimer : public Timer{

public:
    FlipTimer();
    virtual ~FlipTimer();

    virtual bool timesUp();
    virtual float scalar();

private:
    bool bFlipped = false; //marks state
};