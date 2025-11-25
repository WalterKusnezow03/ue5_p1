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
        sigmaSaved = other.sigmaSaved;
        luminanceDifferenceOfGaussians = other.luminanceDifferenceOfGaussians;
        sizeXSaved = other.sizeXSaved;
        sizeYSaved = other.sizeYSaved;
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


void BlurredImage::PasteImageGrayScale(
    const TArray<FColor> &colorIn,
    int sizeX,
    int sizeY
){
    if(colorIn.Num() <= 0){
        return;
    }
    blurredBuffer = colorIn;
    sizeXSaved = sizeX;
    sizeYSaved = sizeY;
    ApplyGrayScale();
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
    sigmaSaved = sigma;
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

void BlurredImage::RemoveContrastFromDifference(float threshold){
    for (int i = 0; i < luminanceDifferenceOfGaussians.Num(); i++){
        if(std::abs(luminanceDifferenceOfGaussians[i]) < threshold){
            luminanceDifferenceOfGaussians[i] = 0.0f;
        }
    }
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

                
            }
        }
        return isExtremaMin || isExtremaMax;
    }
    return false;
}


//not tested.
bool BlurredImage::IsValidKeypoint(
    int x, 
    int y, 
    BlurredImage &prev, 
    BlurredImage &next, 
    float thresHold
)
{
    if(IsExtremumPixel(x, y, prev, next)){
        
        // --- Kantenfilter ---

        //hessian: Die ableitung ist ide steigung der funktion
        //die steigung der steigung ist die beschleunigung
        //wenn die beschleunigung sehr hoch ist, ist es eine harte kante.


        //taylor = sum_i ((f'^i (x) / i!) * (x-p)^i
        //aus taylor beidseitig: f’’(x) =ca= f(x+1) − 2f(x) + f(x−1)
        /*
        Taylor rechts:
        t(x+1) = f(x) + f’(x) + 1/2 f’’(x)(x+1)^2
        Taylor links:
        t(x−1) = f(x) − f’(x) + 1/2 f’’(x)(x-1)^2


        x^2 + 2x + 1 + x^2 - 2x + 1 = 2x^2 + 2

        TAll = f(x) + f’(x) + 1/2 f’’(x) + f(x) − f’(x) + 1/2 f’’(x)
        TAll = 1/2 f’’(x) + 1/2 f’’(x) + f(x) + f’(x) + f(x) − f’(x)
        TAll = f’’(x) + 2f(x)
        f’’(x) = fx - 2fx 
        f’’(x) = (fx - 2fx)
        */
        
        float Dxx = DifferenceOfGaussiansSaved(x+1,y) + DifferenceOfGaussiansSaved(x-1,y) - 2*DifferenceOfGaussiansSaved(x,y);
        
        float Dyy = DifferenceOfGaussiansSaved(x,y+1) + DifferenceOfGaussiansSaved(x,y-1) - 2*DifferenceOfGaussiansSaved(x,y);
        float Dxy = (
            DifferenceOfGaussiansSaved(x+1,y+1) - 
            DifferenceOfGaussiansSaved(x+1,y-1) - 
            DifferenceOfGaussiansSaved(x-1,y+1) + 
            DifferenceOfGaussiansSaved(x-1,y-1)
        ) / 4.0f;

        float Trace = Dxx + Dyy;
        float Trace2 = Trace * Trace;
        float Det = Dxx * Dyy - Dxy * Dxy; //ad - bc

        if(Det <= 0.0f || (Trace2 / Det) > thresHold){
            return false; // Pixel verwerfen, da instabil
        }
        return true;
    }

    return false;
}



KeyPoint BlurredImage::MakeKeyPoint(int x, int y){
    float angle = 0.0f;
    KeyPoint point(x, y, sigmaSaved, angle);
    return point;
}