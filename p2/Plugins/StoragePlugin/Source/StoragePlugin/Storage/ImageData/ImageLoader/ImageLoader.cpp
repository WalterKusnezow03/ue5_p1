#include "ImageLoader.h"

#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "Modules/ModuleManager.h"





bool ImageLoader::LoadPNGLocalPath(
    FString localFilePath,
    TArray<TArray<FColor>> &OutPixels
){
    int32 width = 0;
    int32 height = 0;
    TArray<FColor> rawArray;
    if(LoadPNGLocalPath(localFilePath, rawArray, width, height)){
        Make2D(rawArray, OutPixels, width, height);
        return true;
    }
    return false;
}

void ImageLoader::Make2D(TArray<FColor> &raw, TArray<TArray<FColor>> &out, int32 width, int32 height){
    Init2DArray(out, width, height);

    
    for (int i = 0; i < raw.Num(); i++){
        int32 x = 0;
        int32 y = 0;
        Make2DIndex(i, width, height, x, y);
        out[x][y] = raw[i];
    }
}


void ImageLoader::Init2DArray(TArray<TArray<FColor>> &out, int32 width, int32 height){
    out.Empty();
    out.SetNum(width);
    for (int i = 0; i < out.Num(); i++){
        TArray<FColor> &current = out[i];
        current.SetNum(height);
    }
}

void ImageLoader::Make2DIndex(int32 index, int32 width, int32 height, int32 &xOut, int32 &yOut){
    xOut = index % width;
    yOut = index / width;

    xOut = std::max(0, xOut);
    yOut = std::max(0, yOut);

    xOut = std::min(xOut, width - 1);
    yOut = std::min(yOut, height - 1);
}

bool ImageLoader::LoadPNGLocalPath(
    FString localFilePath, 
    TArray<FColor> &OutPixels, 
    int32 &Width, 
    int32 &Height
){
    FString path = BaseDir(localFilePath);
    return LoadPNG(path, OutPixels, Width, Height);
}

// somePath/ToFile.png
bool ImageLoader::LoadPNG(const FString& FilePath, TArray<FColor>& OutPixels, int32& Width, int32& Height)
{
    TArray<uint8> FileData;
    if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load file: %s"), *FilePath);
        return false;
    }

    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>("ImageWrapper");
    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

    if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(FileData.GetData(), FileData.Num()))
    {
        Width = ImageWrapper->GetWidth();
        Height = ImageWrapper->GetHeight();

        TArray<uint8> RawData;
        if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData))
        {
            // Kopiere die Bytes in FColor-Array
            OutPixels.SetNum(Width * Height);
            for (int32 i = 0; i < Width * Height; i++)
            {
                int32 idx = i * 4;
                OutPixels[i] = FColor(RawData[idx + 2], RawData[idx + 1], RawData[idx + 0], RawData[idx + 3]);
            }
            return true;
        }
    }
    return false;
}