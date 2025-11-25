#include "ImagePatch.h"
#include "ComputerVisionPlugin/Public/ComputerVision/Actor/Storage/ImageWriter.h"

ImagePatch::ImagePatch(){

}

ImagePatch::~ImagePatch(){

}

ImagePatch::ImagePatch(const ImagePatch &other){
    if(this != &other){
        *this = other;
    }
}
ImagePatch &ImagePatch::operator=(const ImagePatch &other){
    if(this != &other){
        colorSaved = other.colorSaved;

        sizeXSaved = other.sizeXSaved;
        sizeYSaved = other.sizeYSaved;
        id = other.id;
    }
    return *this;
}

void ImagePatch::SavePatch(TArray<FColor> &colorIn, int sizePatchX, int sizePatchY, int32 idIn){
    colorSaved = colorIn;

    sizeXSaved = sizePatchX;
    sizeYSaved = sizePatchY;
    id = idIn;

    //for debugging --> looks ok.
    //SaveToStorage();
}

void ImagePatch::SaveToStorage(){
    if(id > 20){
        return; //MAX DEBUG
    }
    if(colorSaved.Num() <= 0){
        return;
    }

    uint8 *ptr = (uint8*) colorSaved.GetData();
    ImageWriter::SaveColorBufferAsPngFromName(
        ptr,
        sizeXSaved,
        sizeYSaved,
        FString::Printf(TEXT("Patches/patch_%d.png"), id)
    );
}




void ImagePatch::ScaleDown(float scalePixels){
    //skalar = distTarget / DistAll

    //frac = scalePixels / all
    TArray<FColor> newBuffer;
    int numPixels = scalePixels * scalePixels;
    //newBuffer.SetNum(numPixels);

    float sX = sizeXSaved;
    float sY = sizeYSaved;

    float step = sX / scalePixels;
    float stepY = sY / scalePixels;
    for (int i = 0; i < scalePixels; i++){
        for (int j = 0; j < scalePixels; j++){
            
            //in original skallierung
            float iStartF = step * i;
            float jStartF = stepY * j;
            float iEndF = step * (i+1);
            float jEndF = stepY * (j+1);

            int iStart = FMath::FloorToInt(iStartF);
            int jStart = FMath::FloorToInt(jStartF);
            int iEnd = FMath::CeilToInt(iEndF);
            int jEnd = FMath::CeilToInt(jEndF);

            //ToIndex(i, j);
            //int index1D = j * scalePixels + i;
            newBuffer.Add(Sample(iStart, jStart, iEnd, jEnd));
        }
    }

    //override patch with new data
    SavePatch(newBuffer, scalePixels, scalePixels, id);
}

FColor ImagePatch::Sample(int iStart, int jStart, int iEnd, int jEnd){
    
    float meanDenominator = (iEnd - iStart) * (jEnd - jStart);

    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    for (int i = iStart; i < iEnd; i++)
    {
        for (int j = jStart; j < jEnd; j++){
            int index = ToIndex(i, j);
            if(index >= 0 && index < colorSaved.Num()){
                FColor &current = colorSaved[index];
                r += current.R;
                g += current.G;
                b += current.B;
            }
        }
    }
    FColor sampled(0, 0, 0, 0);
    sampled.R = FMath::Clamp(FMath::RoundToInt(r / meanDenominator), 0, 255);
    sampled.G = FMath::Clamp(FMath::RoundToInt(g / meanDenominator), 0, 255);
    sampled.B = FMath::Clamp(FMath::RoundToInt(b / meanDenominator), 0, 255);
    sampled.A = 255;
        
    return sampled;
}

int ImagePatch::ToIndex(int i, int j){
    int index = j * sizeXSaved + i;
    if(index >= 0 && index < colorSaved.Num()){
        return index;
    }
    return -1;
}