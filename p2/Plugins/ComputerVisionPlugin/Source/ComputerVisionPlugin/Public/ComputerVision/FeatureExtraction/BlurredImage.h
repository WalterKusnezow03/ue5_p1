#pragma once 

#include "CoreMinimal.h"
#include "KeyPoint.h"

class COMPUTERVISIONPLUGIN_API BlurredImage {

public:
    BlurredImage();
    ~BlurredImage();

    BlurredImage(const BlurredImage &other);
    BlurredImage &operator=(const BlurredImage &other);

    //computes difference to other, but overrides buffer of this to be DOG buffer!
    void ComputeDifferenceOverride(const BlurredImage &other);
    void ComputeDifference(const BlurredImage &other, TArray<float> &outArray);

    void RemoveContrastFromDifference(float threshold);

    void PasteImageGrayScale(
        const TArray<FColor> &colorIn,
        int sizeX,
        int sizeY
    );

    void BlurImage(
        const TArray<FColor> &colorIn,
        int sizeX,
        int sizeY,
        int k,
        float sigma
    );

    void BlurImageAndApplyGrayScale(
        const TArray<FColor> &colorIn,
        int sizeX,
        int sizeY,
        int k,
        float sigma
    );

    //is tested.
    void ApplyGrayScale();

    TArray<FColor> &GetBufferReference();

    uint8 *RawColorArrayPtr();

    bool IsValid(int sizeX, int sizeY);

    //do not resize buffer when using this method!
    FColor &GetPixel(int x, int y);

    //requires all buffer sizes to be the same.
    bool IsValidKeypoint(int x, int y, BlurredImage &prev, BlurredImage &next, float threshold);

    //requires all buffer sizes to be the same.
    bool IsExtremumPixel(
        int x, 
        int y, 
        BlurredImage &prev, 
        BlurredImage &next
    );

    KeyPoint MakeKeyPoint(int x, int y);

    int SizeBuffer() const;

    int ToIndexClamped(int x, int y);
    static int ToIndexClamped(int x, int y, int sizeX, int sizeBuffer);


    //auch für die skalen invarianz (?)
    float DifferenceOfGaussiansSaved(int x, int y);





private:
    float sigmaSaved = 1.0f;

    TArray<FColor> blurredBuffer;
    TArray<float> luminanceDifferenceOfGaussians; // convert all to linear color????

    int sizeXSaved = 1;
    int sizeYSaved = 1;
    FColor fallback;

    void AddColorLin(FColor &color, const FColor &add, float scalar);

    FColor ProcessBatch(
        const TArray<FColor> &colorIn,
        int sizeX,
        int index,
        int k,
        float sigma
    );

    float GaussianScalar(int x, int y, float sigma);

    
    

    void IndexTo2D(
        int index, 
        int sizeX,
        int &xOut, 
        int &yOut
    );




    //hilfs funktionen erstmal so.

    float luminance(int x, int y);
    float luminance(const FColor &color);
   

    
    
};