#pragma once

#include "CoreMinimal.h"

class COMPUTERVISIONPLUGIN_API ImageBase {

public:
    ImageBase();
    virtual ~ImageBase();

    ImageBase(const ImageBase &other);
    ImageBase &operator=(const ImageBase &other);

    virtual void Setup(const TArray<FColor> &color, int scaleX, int scaleY);
    virtual void Setup(int scaleX, int scaleY);

    bool IsValid(int sizeX, int sizeY);
    TArray<FColor> &GetBufferReference();
    uint8 *RawColorArrayPtr();

    static int ToIndexClamped(int x, int y, int sizeX, int sizeBuffer);
    int ToIndexClamped(int x, int y);
    int ToIndexClamped(int i, int j, int sizeXOnBuffer);

    int ToIndex(int x, int y, int sizeX);

    TArray<FColor> CopyPatch(int xStart, int yStart, int xEnd, int yEnd);

    int SizeBuffer() const;

    virtual float luminance(int x, int y);
    float luminance(const FColor &color);

protected:
    void IndexTo2D(
        int index,
        int sizeX,
        int &xOut,
        int &yOut
    );

    
    
    FColor &GetPixel(int x, int y);

    
    

    int sizeXSaved = 0;
    int sizeYSaved = 0;
    TArray<FColor> buffer;

    FColor fallback;

private:


};