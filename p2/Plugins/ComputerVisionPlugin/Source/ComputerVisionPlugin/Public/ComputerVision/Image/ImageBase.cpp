#include "ImageBase.h"


ImageBase::ImageBase(){

}

ImageBase::~ImageBase(){

}

ImageBase::ImageBase(const ImageBase &other){
    if(this != &other){
        *this = other;
    }
}

ImageBase &ImageBase::operator=(const ImageBase &other){
    //copy constructor
    sizeXSaved = other.sizeXSaved;
    sizeYSaved = other.sizeYSaved;
    buffer = other.buffer;
    return *this;
}

void ImageBase::Setup(const TArray<FColor> &color, int scaleX, int scaleY){
    sizeXSaved = scaleX;
    sizeYSaved = scaleY;
    buffer = color;
}

void ImageBase::Setup(int scaleX, int scaleY){
    sizeXSaved = scaleX;
    sizeYSaved = scaleY;
    buffer.SetNum(scaleX * scaleY);
}

bool ImageBase::IsValid(int sizeX, int sizeY){
    return sizeX * sizeY == buffer.Num() && buffer.Num() > 0;
}

TArray<FColor> &ImageBase::GetBufferReference(){
    return buffer;
}

uint8 *ImageBase::RawColorArrayPtr(){
    uint8 *ptr = (uint8*) buffer.GetData(); // bufferPackage.DataPtrAsUint8()
    return ptr;
}



void ImageBase::IndexTo2D(
    int index, 
    int sizeX,
    int &xOut, 
    int &yOut
){
    int ySteps = index / sizeX;
    int xStep = index % sizeX;
    xOut = xStep;
    yOut = ySteps;
}

int ImageBase::ToIndexClamped(int x, int y){
    return ToIndexClamped(x, y, sizeXSaved, buffer.Num());
}

int ImageBase::ToIndexClamped(int x, int y, int sizeXOnBuffer){
    return ToIndexClamped(x, y, sizeXOnBuffer, buffer.Num());
}

int ImageBase::ToIndexClamped(int x, int y, int sizeX, int sizeBuffer){
    int index = sizeX * y + x;
    index = std::max(index, 0);
    index = std::min(index, sizeBuffer - 1);

    return index;
}


int ImageBase::ToIndex(int x, int y, int sizeX){
    int index = sizeX * y + x;
    index = std::max(index, 0);
    return index;
}


FColor &ImageBase::GetPixel(int x, int y){
    //DebugHelper::logMessage("ImageBase::GetPixel on buffer size", buffer.Num());
    int asIndex = ToIndexClamped(x, y);
    if(asIndex >= 0 && asIndex < buffer.Num()){
        return buffer[asIndex];
    }
    return fallback;
}


int ImageBase::SizeBuffer() const {
    return buffer.Num();
}


float ImageBase::luminance(int x, int y){
    return luminance(GetPixel(x, y));
}

float ImageBase::luminance(const FColor &color){
    float L = 0.299f*color.R + 0.587f*color.G + 0.114f*color.B;
    return L;
}


TArray<FColor> ImageBase::CopyPatch(int xStart, int yStart, int xEnd, int yEnd){
    TArray<FColor> outColor;
    for (int i = xStart; i < xEnd; i++)
    {
        for (int j = yStart; j < yEnd; j++){
            int asIndex = ToIndexClamped(i,j, sizeXSaved, buffer.Num());
            if(asIndex >= 0 && asIndex < buffer.Num()){
                FColor &colorAt = buffer[asIndex];
                outColor.Add(colorAt);
            }
        }
    }
    return outColor;
}