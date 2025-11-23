#include "ImageFeatureFinder.h"

ImageFeatureFinder::ImageFeatureFinder(){

}

ImageFeatureFinder::~ImageFeatureFinder(){

}


void ImageFeatureFinder::ExtractFeatures(
    TArray<FColor> &colors,
    int sizeX, 
    int sizeY
){
    int k = 3;
    float blurN = 1;
    int count = 5;
    TArray<BlurredImage> blurred;
    blurred.SetNum(count);
    for (int i = 0; i < count; i++){
        blurN += 0.2f; //irgendwas erstmal.
        BlurredImage &made = blurred[i];
        made.BlurImageAndApplyGrayScale(
            colors,
            sizeX,
            sizeY,
            k,
            blurN // float sigma
        );
    }
    ExtractFeatures(blurred, sizeX, sizeY);
}

void ImageFeatureFinder::ExtractFeatures(
    TArray<BlurredImage> &imageStack,
    int sizeX, 
    int sizeY
){
    ComputeDifferenceOfGaussians(imageStack);
    FindExtremumsInDifferenceOfGaussians(imageStack, sizeX, sizeY);
}

//all images from buffer are overriden with the difference.
void ImageFeatureFinder::ComputeDifferenceOfGaussians(
    TArray<BlurredImage> &imageStack
){

    //TArray<TArray<FColor>> differenceStack;
    //differenceStack.SetNum(imageStack.Num());
    for (int i = 1; i < imageStack.Num(); i++)
    {
        BlurredImage &prev = imageStack[i - 1];
        BlurredImage &current = imageStack[i];

        //TArray<FColor> &differnce = differenceStack[i];
        prev.ComputeDifferenceOverride(current); //prev buffer is overriden with DOG
    }
}

void ImageFeatureFinder::FindExtremumsInDifferenceOfGaussians(
    TArray<BlurredImage> &imageStack,
    int sizeX, 
    int sizeY
){
    if(imageStack.Num() > 0){
        BlurredImage &first = imageStack[0];
        extremaFlags.SetNum(first.SizeBuffer()); //init some size on flag buffer.
    }

    for (int i = 2; i < imageStack.Num(); i++){
        CheckAndMarkExtrema(
            imageStack[i - 2],
            imageStack[i - 1],
            imageStack[i],
            sizeX,
            sizeY
        );
    }
}

void ImageFeatureFinder::CheckAndMarkExtrema(
    BlurredImage &prev,
    BlurredImage &current,
    BlurredImage &next,
    int sizeX,
    int sizeY
){
    for (int i = 1; i < sizeX - 1; i++){
        for (int j = 1; j < sizeY - 1; j++){
            int index = prev.ToIndexClamped(i, j); //einfach irgend ein index, bilder eh gleich gross.
            if(!extremaFlags[index]){
                if (current.IsExtremumPixel(
                    i,
                    j,
                    prev,
                    next
                )
                )
                {
                    extremaFlags[index] = true;
                }
            }
        }
    }
}



//as colorized buffer
TArray<FColor> ImageFeatureFinder::extremaAsColorBuffer(){
    TArray<FColor> outBuffer;
    outBuffer.SetNumUninitialized(extremaFlags.Num());
    for (int i = 0; i < extremaFlags.Num(); i++){
        int colorI = extremaFlags[i] ? 255 : 0;
        outBuffer[i] = FColor(colorI, colorI, colorI, 255);
    }
    return outBuffer;
}
