#pragma once 

#include "CoreMinimal.h"
#include "CoreMath/Matrix/2D/MMatrix2D.h"

//image functions and blurred image must be merged together.
class COMPUTERVISIONPLUGIN_API ImagePatch {

public:
    ImagePatch();
    ~ImagePatch();

    ImagePatch(const ImagePatch &other);
    ImagePatch &operator=(const ImagePatch &other);

    void SavePatch(
        TArray<FColor> &colorIn, 
        int sizePatchX, 
        int sizePatchY, 
        int32 idIn
    );

    void SavePatch(
        TArray<FColor> &colorIn, 
        int sizePatchX, 
        int sizePatchY, 
        int32 idIn,
        FVector2D &screenPositonIn //screen position of original patch on screen.
    );



    // ------ TODO SIFT ZU ENDE MACHEN ! ------
    //TODO: Compute Orientation before Copy Patch (To in blurred image DOG?)
    //TODO: Remove Orientation to match the player images (e.x. every image to compare!)

    void ScaleDown(float scalePixels);

    //debug
    void SaveToStorage();

    void OverrideId(int32 idIn){
        id = idIn;
    }

    void ComputeGradients();

    void ComputeLuminance();

    float luminance(int x, int y);
    float luminance(const FColor &color);
    FColor &GetPixel(int x, int y);

    TArray<float> &FeatureVectorRef(){
        return FeatureVector;
    }

    /// @brief position of patch from extracted image
    /// @return 
    FVector2D &GetOriginalImagePosition();

    float Distance(ImagePatch &other);
    float DistanceSquared(ImagePatch &other);

private:
    int ToIndex(int i, int j);
    int ToIndex(int i, int j, int sizeXIn);
    FColor Sample(int iStart, int jStart, int iEnd, int jEnd);

    void FindDominantGradientAngle(
        TArray<float> &magnitude,
        TArray<float> &theta,
        int numBins,
        float &outDominantAngle,
        float &outDominantGradientMagnitude
    );

    FVector2D ComputeGradient(int x, int y);

    //visualize rotation invariance
    void RotateImage(float angleRad);
    void RotatePixelCoordintaes(
        FVector2D &pixel,
        MMatrix2D &rotator
    );

    void Copy4x4FromAndBuildDescriptor(
        const TArray<float> &magnitude,
        const TArray<float> &theta,
        int sizeXOnOriginalBuffer,
        int iStart,
        int jStart,
        int iEnd,
        int jEnd,
        TArray<float> &appendToFeatureVector
    );

    TArray<float> MakeHistogramOfMagnitudePerAngleBin(
        TArray<float> &magnitude,
        TArray<float> &theta,
        int numBins
    );

    FVector2D screenPosition;

    TArray<float> luminanceBuffer;
    TArray<FColor> colorSaved;

    TArray<float> FeatureVector;

    //TArray<float> magnitude;
    //TArray<float> theta;


    int sizeXSaved = 1;
    int sizeYSaved = 1;
    int32 id;

    FColor fallback;
};