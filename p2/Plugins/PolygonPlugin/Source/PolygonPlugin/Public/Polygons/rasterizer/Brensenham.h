#pragma once

#include "CoreMinimal.h"


class POLYGONPLUGIN_API Brensenham {

public:
    static void BrensenhamLineFill(
        TArray<FIntPoint> &outPoints
    );


    static void BrensenhamLineAppend(
        const FIntPoint &Start, 
        const FIntPoint &End,
        TArray<FIntPoint> &outPoints
    );

    



};
