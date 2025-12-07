#include "ImageWriter.h"

#include "ImageUtils.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"

#include "ImageWriter.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Modules/ModuleManager.h"
#include "Misc/FileHelper.h"



void ImageWriter::SaveColorBufferAsPngFromName(
    uint8 *ColorData, 
    int32 Width, 
    int32 Height, 
    FString imageNameIn
){
    FString path = FPaths::ProjectSavedDir() // has "/" at end
                   + TEXT("ComputerVisionPlugin/") + imageNameIn + TEXT(".png");
    SaveColorBufferAsPng(ColorData, Width, Height, path);
}

void ImageWriter::SaveColorBufferAsPng(uint8* ColorData, int32 Width, int32 Height, int32 id){
    if(id > MaxImages()){
        return;
    }
    SaveColorBufferAsPngFromName(
        ColorData,
        Width,
        Height,
        imageName(id)
    );
}

FString ImageWriter::imageName(int32 id){
    return FString::Printf(TEXT("CV_%d.png"), id);
}

void ImageWriter::SaveColorBufferAsPng(uint8* ColorData, int32 Width, int32 Height, const FString& FilePath)
{
    //DEBUG
    return;


    /*TArray<FColor> ColorArray;
    ColorArray.SetNumUninitialized(Width * Height);

    // Metal liefert BGRA oder RGBA? Falls RGBA:
    FMemory::Memcpy(ColorArray.GetData(), ColorData, Width * Height * 4);

    TArray<uint8> PngData;
    FImageUtils::CompressImageArrayToPNG(Width, Height, ColorArray, PngData);

    FFileHelper::SaveArrayToFile(PngData, *FilePath);*/

    DebugHelper::logMessage(FString::Printf(TEXT("ImageWriter::SaveImageAs %s"), *FilePath));

    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

    // Achtung: UE-RenderTargets = meist BGRA8
    ImageWrapper->SetRaw(ColorData, Width * Height * 4, Width, Height, ERGBFormat::BGRA, 8);

    // 1) Get the compressed data (TArray64<uint8>)
    TArray64<uint8> CompressedData64 = ImageWrapper->GetCompressed();

    // 2) Copy into a normal TArray<uint8>
    TArray<uint8> PNGData;
    PNGData.Append(CompressedData64.GetData(), CompressedData64.Num());

    // 3) Save to file
    FFileHelper::SaveArrayToFile(PNGData, *FilePath);

    
}
