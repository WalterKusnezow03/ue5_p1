#pragma once

#include "CoreMinimal.h"
#include "PolygonPlugin/Public/Polygons/MeshedPolygonUtil/MeshedPolygonHullSet.h"


/// @brief class to colorize a float grid by color flag or lerp
/// used for meshed polygon visualization of nn data
class PATHFINDERNNEXTENSION_API FGridColorizer {

public:
    FGridColorizer();
    ~FGridColorizer();

    void ColorizeFromEpsilonFlag(
        Image &image,
        TArray<TArray<float>> &grid,
        float epsilon,
        const FColor &color
    );
    void ColorizeFromEpsilonFlag(
        Image &image,
        TArray<TArray<float>> &grid,
        float epsilon,
        bool useAbs,
        const FColor &color
    );

    void ColorizeFromUintFlag(
        Image &image,
        TArray<TArray<uint8>> &flagGrid,
        const FColor &color,
        bool flagsInverted
    );

    void ColorizeFromLerp(
        Image &image,
        TArray<TArray<float>> &grid,
        const FColor &colorMin,
        const FColor &colorMax
    );


    void ColorizeEdgePoints(
        Image &image,
        FMeshedPolygonHullSet &set,
        FColor color
    );

private:
    
    FColor DirColor(
        const FColor &colorMin,
        const FColor &colorMax
    );
    FColor LerpColor(
        const FColor &colorMin,
        const FColor &colorDir,
        float scalar
    );
};