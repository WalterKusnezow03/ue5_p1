#pragma once

#include "CoreMinimal.h"

#include "Timer.h"


class COREMATH_API FlipTimer : public Timer{

public:
    FlipTimer();
    virtual ~FlipTimer();

    virtual bool timesUp() override;
    virtual float scalar() override;

private:
    bool bFlipped = false; //marks state
};