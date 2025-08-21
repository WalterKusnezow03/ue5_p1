#pragma once
#include "CoreMinimal.h"

class GAMECORE_API LodConstants {

public:
    static const int metersLodNearUntil = 200;
    static const int metersLodMiddleUntil = 400;
    static const int metersLodFarUntil = 600;

private:
    LodConstants(){}
    ~LodConstants(){}
};