#pragma once 
#include "CoreMinimal.h"
#include "KeyPoint.h"
#include "ImagePatch.h"

class COMPUTERVISIONPLUGIN_API ImageFeatureFinder {

public:
    ImageFeatureFinder();
    ~ImageFeatureFinder();

    ImageFeatureFinder(const ImageFeatureFinder &other);
    ImageFeatureFinder &operator=(const ImageFeatureFinder &other);

    void ExtractFeatures(
        TArray<FColor> &colors,
        int sizeX, 
        int sizeY
    );

    

    TArray<FColor> extremaAsColorBuffer();

    TArray<ImagePatch> &ExtractedFeaturePatches();

private:
    BlurredImage imageRawGray;
    //TArray<KeyPoint> keypointsMade;

    void ExtractFeatures(
        TArray<BlurredImage> &imageStack,
        int sizeX, 
        int sizeY
    );

    float TimeStart;
    void StartTime();
    void EndTime(FString prefix);

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

    void MakePatches(
        TArray<FColor> &colorBuffer,
        int sizeX,
        int sizeY
    );

    bool CopyPatch(
        int sizeX,
        int sizeY,
        int patchSize,
        int startX,
        int startY,
        TArray<FColor> &colorBuffer,
        TArray<FColor> &outColor
    );

    bool HasFeature(
        int patchSize,
        int startX,
        int startY,
        int sizeXOriginalBuffer
    );


    //new
    TArray<ImagePatch> createdDesicriptorPatches;
    void CreateDescriptorsFromKeyPoints(
        TArray<FColor> &colorBuffer,
        int sizeX);
    ImagePatch CopyPatchFromKeyPoint(
        KeyPoint &keypoint,
        TArray<FColor> &colorBuffer,
        int sizeX // of original image
    );
};