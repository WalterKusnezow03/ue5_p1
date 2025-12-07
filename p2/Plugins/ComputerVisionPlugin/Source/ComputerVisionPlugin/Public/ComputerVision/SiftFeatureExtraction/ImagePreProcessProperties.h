#pragma once

#include "CoreMinimal.h"


class COMPUTERVISIONPLUGIN_API ImagePreProcessProperties {

public:
    static constexpr bool detectEdges = true;
    static constexpr bool reduceColor = true;


    static void ClampColorTo(TArray<FColor> &colors, int step){
        for(FColor &color : colors){
            ClampColorTo(color, step);
        }
    }


    static void ClampColorTo(FColor &color, int step){
        int r = color.R;
        int g = color.G;
        int b = color.B;

        r -= r % step;
        g -= g % step;
        b -= b % step;

        color.R = r;
        color.G = g;
        color.B = b;
        
    }
};