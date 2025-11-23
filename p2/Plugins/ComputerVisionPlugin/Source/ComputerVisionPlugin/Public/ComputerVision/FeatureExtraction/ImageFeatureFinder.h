#pragma once 
#include "CoreMinimal.h"


class COMPUTERVISIONPLUGIN_API ImageFeatureFinder {

public:
    ImageFeatureFinder();
    ~ImageFeatureFinder();

    void ExtractFeatures(
        TArray<FColor> &colors,
        int sizeX, 
        int sizeY
    );

    

    TArray<FColor> extremaAsColorBuffer();

private:

    void ExtractFeatures(
        TArray<BlurredImage> &imageStack,
        int sizeX, 
        int sizeY
    );


    void ComputeDifferenceOfGaussians(
        TArray<BlurredImage> &imageStack
    );

    void FindExtremumsInDifferenceOfGaussians(
        TArray<BlurredImage> &imageStack,
        int sizeX,
        int sizeY
    );

    void CheckAndMarkExtrema(
        BlurredImage &prev,
        BlurredImage &current,
        BlurredImage &next,
        int sizeX,
        int sizeY
    );


    TArray<bool> extremaFlags;
};