#include "BlurredImage.h"


BlurredImage::BlurredImage(){

}

BlurredImage::~BlurredImage(){

}

BlurredImage::BlurredImage(const BlurredImage &other){
    if(this != &other){
        *this = other;
    }
}

BlurredImage &BlurredImage::operator=(const BlurredImage &other){
    if(this != &other){
        blurredBuffer = other.blurredBuffer;
    }
    return *this;
}

bool BlurredImage::IsValid(int sizeX, int sizeY){
    return sizeX * sizeY == blurredBuffer.Num() && blurredBuffer.Num() > 0;
}

TArray<FColor> &BlurredImage::GetBufferReference(){
    return blurredBuffer;
}

uint8 *BlurredImage::RawColorArrayPtr(){
    uint8 *ptr = (uint8*) blurredBuffer.GetData(); // bufferPackage.DataPtrAsUint8()
    return ptr;
}

void BlurredImage::BlurImage(
    const TArray<FColor> &colorIn, 
    int sizeX, 
    int sizeY, 
    int k,
    float sigma
){
    if(colorIn.Num() <= 0){
        return;
    }
    blurredBuffer.SetNumUninitialized(colorIn.Num());
    sizeXSaved = sizeX;
    sizeYSaved = sizeY;

    for (int i = 0; i < colorIn.Num(); i++){
        blurredBuffer[i] = ProcessBatch(
            colorIn,
            sizeX,
            i,
            k,
            sigma
        );
    }
}


void BlurredImage::BlurImageAndApplyGrayScale(
    const TArray<FColor> &colorIn,
    int sizeX,
    int sizeY,
    int k,
    float sigma
){
    BlurImage(
        colorIn,
        sizeX,
        sizeY,
        k,
        sigma
    );
    ApplyGrayScale();
}

    


void BlurredImage::ApplyGrayScale(){
    for (int i = 0; i < blurredBuffer.Num(); i++){
        FColor &c = blurredBuffer[i];
        uint8 gray = uint8(0.299f * c.R + 0.587f * c.G + 0.114f * c.B);
        c.R = gray;
        c.G = gray;
        c.B = gray;
    }
}

FColor BlurredImage::ProcessBatch(
    const TArray<FColor> &colorIn, 
    int sizeX, 
    int index, 
    int k, 
    float sigma
){
    int sizeBuffer = colorIn.Num();
    int xCenter = 0;
    int yCenter = 0;
    IndexTo2D(index, sizeX, xCenter, yCenter);

    FColor outColor(0, 0, 0, 0);
    float sumWeights = 0.0f;
    for (int x = xCenter - k; x < xCenter + k; x++)
    {
        for (int y = yCenter - k; y < yCenter + k; y++){
            int i = ToIndexClamped(x, y, sizeX, sizeBuffer);
            if (i >= 0 && i < colorIn.Num()){
                const FColor& current = colorIn[i];

                float w = GaussianScalar(x - xCenter, y - yCenter, sigma);
                sumWeights += w;

                AddColorLin(outColor, current, w);
            }
        }
    }
    if(sumWeights > 0.0f){
        outColor.R /= sumWeights;
        outColor.G /= sumWeights;
        outColor.B /= sumWeights;
        outColor.A /= sumWeights;
    }
    
    return outColor;
}

void BlurredImage::AddColorLin(FColor &color, const FColor &add, float scalar){
    color.R += add.R * scalar;
    color.G += add.G * scalar;
    color.B += add.B * scalar;
    color.A += add.A * scalar;
}

float BlurredImage::GaussianScalar(int dx, int dy, float sigma){
    float s2 = sigma * sigma;
    float norm = 1.0f / (2.0f * PI * s2);
    return norm * FMath::Exp(-(dx*dx + dy*dy) / (2.0f * s2));
}



void BlurredImage::IndexTo2D(
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

int BlurredImage::ToIndexClamped(int x, int y){
    return ToIndexClamped(x, y, sizeXSaved, blurredBuffer.Num());
}

int BlurredImage::ToIndexClamped(int x, int y, int sizeX, int sizeBuffer){
    int index = sizeX * y + x;
    index = std::max(index, 0);
    index = std::min(index, sizeBuffer - 1);

    return index;
}



void BlurredImage::ComputeDifference(
    const BlurredImage &other, 
    TArray<float> &outArray
){  
    if(blurredBuffer.Num() <= 0){
        return;
    }
    const TArray<FColor> &otherBuffer = other.blurredBuffer;

    outArray.SetNumUninitialized(blurredBuffer.Num());
    for (int i = 0; i < blurredBuffer.Num(); i++){
        if (i < otherBuffer.Num()){
            FColor &current = blurredBuffer[i];
            const FColor &otherCurrent = otherBuffer[i];

            float luminanceA = luminance(current);
            float luminanceB = luminance(otherCurrent);
            float difference = luminanceB - luminanceA; //AB = B - A //so rum?
            outArray[i] = difference;
        }
    }
}


void BlurredImage::ComputeDifferenceOverride(const BlurredImage &other){
    ComputeDifference(other, luminanceDifferenceOfGaussians);
}

FColor &BlurredImage::GetPixel(int x, int y){
    int asIndex = ToIndexClamped(x, y);
    if(asIndex >= 0 && asIndex < blurredBuffer.Num()){
        return blurredBuffer[asIndex];
    }
    return fallback;
}


int BlurredImage::SizeBuffer() const {
    return blurredBuffer.Num();
}

//erstmal so, sollte helligkeit sein?
int BlurredImage::ExtremaCheckValue(FColor &color){
    return luminance(color);
}

float BlurredImage::luminance(int x, int y){
    return luminance(GetPixel(x, y));
}

float BlurredImage::luminance(const FColor &color){
    float L = 0.299f*color.R + 0.587f*color.G + 0.114f*color.B;
    return L;
}

float BlurredImage::DifferenceOfGaussiansSaved(int x, int y){
    int asIndex = ToIndexClamped(x, y);
    if(asIndex >= 0 && asIndex < luminanceDifferenceOfGaussians.Num()){
        return luminanceDifferenceOfGaussians[asIndex];
    }
    return 0.0f;
}

bool BlurredImage::MoreExtremeMin(int oldExtrema, FColor &check){
    int compare = ExtremaCheckValue(check);
    if (compare < oldExtrema){
        return true;
    }
    return false;
}

bool BlurredImage::MoreExtremeMax(int oldExtrema, FColor &check){
    int compare = ExtremaCheckValue(check);
    if (compare > oldExtrema){
        return true;
    }
    return false;
}

bool BlurredImage::IsExtremumPixel(
    int x, 
    int y, 
    BlurredImage &prev, 
    BlurredImage &next
){
    int thisSize = SizeBuffer();
    if (thisSize == prev.SizeBuffer() && thisSize == next.SizeBuffer()){
        x = std::max(x, 1);
        y = std::max(y, 1);
        //offset
        x = std::min(x, sizeXSaved - 2);
        y = std::min(y, sizeYSaved - 2);

        
        float extrema = DifferenceOfGaussiansSaved(x,y);

        //FColor &currentPixel = GetPixel(x, y);
        //int extrema = //ExtremaCheckValue(currentPixel);

        bool isExtremaMin = true;
        bool isExtremaMax = true;
        for (int i = x - 1; i <= x + 1; i++)
        {
            for (int j = y - 1; j <= y + 1; j++){
                if(!isExtremaMin && !isExtremaMax){
                    return false;
                }
                if(i == x && j == y)
                    continue;

                float prevDog = prev.DifferenceOfGaussiansSaved(i, j);
                float currentNeighborDog = DifferenceOfGaussiansSaved(i, j);
                float nextDog = next.DifferenceOfGaussiansSaved(i, j);


                //check if is more more extreme 
                if(prevDog < extrema){
                    isExtremaMin = false;
                }
                if(currentNeighborDog < extrema && i != j){
                    isExtremaMin = false;
                }
                if(nextDog < extrema){
                    isExtremaMin = false;
                }

                if(prevDog > extrema){
                    isExtremaMax = false;
                }
                if(currentNeighborDog > extrema && i != j){
                    isExtremaMax = false;
                }
                if(nextDog > extrema){
                    isExtremaMax = false;
                }

                /*
                //mit darüber und darunter üben prüfen ob max, und seiten, 
                //3x3x3 block checkup
                FColor &prevPixel = prev.GetPixel(i, j);
                FColor &nextPixel = next.GetPixel(i, j);
                FColor &currentNeighbor = GetPixel(i, j);

                if(
                    MoreExtremeMin(extrema, prevPixel) ||
                    MoreExtremeMin(extrema, nextPixel)
                ){
                    if(x != i && y != i){

                        if(MoreExtremeMin(extrema, currentNeighbor)){
                            isExtremaMin = false;
                        }
                    }
                }
                if(
                    MoreExtremeMax(extrema, prevPixel) ||
                    MoreExtremeMax(extrema, nextPixel)
                ){
                    if(x != i && y != i){

                        if(MoreExtremeMax(extrema, currentNeighbor)){
                            isExtremaMax = false;
                        }
                    }
                }*/
                
            }
        }
        return isExtremaMin || isExtremaMax;
    }
    return false;
}