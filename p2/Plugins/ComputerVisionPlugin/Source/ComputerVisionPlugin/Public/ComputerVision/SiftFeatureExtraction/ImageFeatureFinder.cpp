#include "ImageFeatureFinder.h"
#include "DebugPlugin/DebugHelper.h"
#include "ImagePreProcessProperties.h"
#include "ImagePatch.h"


ImageFeatureFinder::ImageFeatureFinder(){

}

ImageFeatureFinder::~ImageFeatureFinder(){

}

ImageFeatureFinder::ImageFeatureFinder(const ImageFeatureFinder &other){
    if(this != &other){
        *this = other;
    }
}

ImageFeatureFinder &ImageFeatureFinder::operator=(const ImageFeatureFinder &other){
    if(this != &other){
        createdDesicriptorPatches = other.createdDesicriptorPatches;
    }
    
    return *this;
}



/// log
void ImageFeatureFinder::StartTime(){
    TimeStart = FPlatformTime::Seconds();
}

void ImageFeatureFinder::EndTime(FString prefix){
    //hier output ca 70ms, schlecht!
    float deltatime = FPlatformTime::Seconds() - TimeStart; // AB = B - A
    deltatime = std::max(0.0f, deltatime);

    /*DebugHelper::logMessage(
        FString::Printf(TEXT("%s %.2f"), *prefix, deltatime)
    );*/
}


void ImageFeatureFinder::ExtractFeatures(
    TArray<FColor> &colors,
    int sizeX, 
    int sizeY
){
    //new
    if(ImagePreProcessProperties::reduceColor){
        ImagePreProcessProperties::ClampColorTo(colors, 64);
        //256 / 16 = 16 bit color.
        //256 / 32 = 8 bit color.
        //256 / 64 = 4 bit color.
    }

    //return;
    imageRawGray.PasteImageGrayScale(colors, sizeX, sizeY); // raw image as gray scale

    StartTime();
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
    EndTime("ImageFeatureFinder::Blur");


    StartTime();
    ExtractFeatures(blurred, sizeX, sizeY);
    //MakePatches(colors, sizeX, sizeY);
    //CreateDescriptorsFromKeyPoints(colors, sizeX);

    EndTime("ImageFeatureFinder::extractFeatures");
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
        prev.RemoveContrastFromDifference(1.3f); //testwert //10 zu hoch
    }
}

void ImageFeatureFinder::FindExtremumsInDifferenceOfGaussians(
    TArray<BlurredImage> &imageStack,
    int sizeX, 
    int sizeY
){
    if(imageStack.Num() > 0){
        BlurredImage &first = imageStack[0];
        //extremaFlags.SetNum(first.SizeBuffer()); //init some size on flag buffer.
        extremaFlags.Init(false, first.SizeBuffer());
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

    //10 ok 
    //2 too low, 
    //3.5 too low
    //5 like 10, 
    //4 better
    //3.9 to low
    //3.95 to low
    //3.99 to low
    //3.999999f 
    float edgeThresHold = 4.0f;
    for (int i = 1; i < sizeX - 1; i++){
        for (int j = 1; j < sizeY - 1; j++){
            int index = prev.ToIndexClamped(i, j); //einfach irgend ein index, bilder eh gleich gross.
            if(!extremaFlags[index]){

                
                if (current.IsValidKeypoint(
                        // IsExtremumPixel(
                        i,
                        j,
                        prev,
                        next,
                        edgeThresHold
                    )
                )
                {
                    extremaFlags[index] = true;


                    int windowSize = 16;
                    ImagePatch made = current.GeneratePatch(i, j, windowSize);
                    createdDesicriptorPatches.Add(made);
                }
                else
                {
                    extremaFlags[index] = false;
                }
            }
        }
    }
}


TArray<ImagePatch> &ImageFeatureFinder::ExtractedFeaturePatches(){
    return createdDesicriptorPatches;
}

//as colorized buffer
TArray<FColor> ImageFeatureFinder::extremaAsColorBuffer(){
    TArray<FColor> outBuffer;
    outBuffer.SetNumUninitialized(extremaFlags.Num());
    int features = 0;
    for (int i = 0; i < extremaFlags.Num(); i++)
    {
        bool flag = extremaFlags[i];

        //invertieren der logik weil es teilweise einfach falsch ist.
        bool invert = true;
        if(invert){
            flag = !flag;
        }

        int colorI = flag ? 255 : 0;
        outBuffer[i] = FColor(colorI, colorI, colorI, 255);

        if(flag){
            features++;
        }
    }
    //DebugHelper::logMessage("ImageFeatureFinder::FoundFeatures ", features); //around 2000, 3percent of image

    return outBuffer;
}




///erstmal 16x16 durch iterieren, wenn keypoint, copy patch
//nicht ums zentrum oderso, sondern einfach primitv das patch kopieren
void ImageFeatureFinder::MakePatches(
    TArray<FColor> &colorBuffer,
    int sizeX, 
    int sizeY
){
    int patchSize = 16;
    int patchesX = sizeX / patchSize;
    int patchesY = sizeY / patchSize;


    for (int i = 0; i < patchesX; i++){
        for (int j = 0; j < patchesY; j++){
            TArray<FColor> colorPatch;
            int startX = i * patchSize;
            int startY = j * patchSize;
            if (CopyPatch(sizeX, sizeY, patchSize, startX, startY, colorBuffer, colorPatch)){
                ImagePatch patch;
                int32 id = i * j;
                patch.SavePatch(colorPatch, patchSize, patchSize, id); //einfach so.
            }
        }
    }
}

bool ImageFeatureFinder::CopyPatch(
    int sizeX, 
    int sizeY,
    int patchSize,
    int startX,
    int startY,
    TArray<FColor> &colorBuffer,
    TArray<FColor> &outColor
){
    if(!HasFeature(patchSize, startX, startY, sizeX)){
        return false;
    }

    for (int i = startX; i < startX + patchSize; i++){
        for (int j = startY; j < startY + patchSize; j++){
            int asIndex = BlurredImage::ToIndexClamped(i,j, sizeX, colorBuffer.Num());
            if(asIndex >= 0 && asIndex < colorBuffer.Num()){
                FColor &colorAt = colorBuffer[asIndex];
                outColor.Add(colorAt);
            }
        }
    }
    return true;
}

bool ImageFeatureFinder::HasFeature(
    int patchSize,
    int startX,
    int startY,
    int sizeXImage
){
    //On2 primitv.
    for (int i = startX; i < startX + patchSize; i++){
        for (int j = startY; j < startY + patchSize; j++){
            int asIndex = BlurredImage::ToIndexClamped(i,j, sizeXImage, extremaFlags.Num());
            if(asIndex >= 0 && asIndex < extremaFlags.Num()){
                if(extremaFlags[asIndex]){
                    return true;
                }
            }
        }
    }
    return false;
}










