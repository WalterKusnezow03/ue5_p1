#include "ImageWriter.h"

#include "ImageUtils.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"

#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Modules/ModuleManager.h"
#include "Misc/FileHelper.h"

#include "StoragePlugin/Storage/ImageData/Image/Image.h"

#include "DebugPlugin/DebugHelper.h"

void ImageWriter::SaveImagesAsPngFromName(
    const TArray<Image> &images,
    FString pluginName,
    FString imageNameIn
){
    for (int i = 0; i < images.Num(); i++){
        FString genaratedName = FString::Printf(TEXT("%s_%d"), *imageNameIn, i);
        SaveImageAsPngFromName(images[i], pluginName, genaratedName);
    }
}
   

void ImageWriter::SaveImageAsPngFromName(
    const Image &image,
    FString pluginName,
    FString imageNameIn
){
    TArray<FColor> colors;
    if(image.GetColorBuffer(colors)){
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("ImageWriter Save Buffer %s %s %d"),
                *pluginName,
                *imageNameIn,
                colors.Num()
            )
        );
        uint8 *ptr = (uint8 *)colors.GetData();
        int32 width = image.widthX();
        int32 height = image.heightY();
        SaveColorBufferAsPngFromName(ptr, width, height, pluginName, imageNameIn);
    }
}

void ImageWriter::SaveColorBufferAsPngFromName(
    uint8 *ColorData, 
    int32 Width, 
    int32 Height, 
    FString pluginName,
    FString imageNameIn
){
    FString path = FString::Printf(
        TEXT("%s%s/%s.png"),
        *FPaths::ProjectSavedDir(),
        *pluginName,
        *imageNameIn
    );
    

    //FString path = FPaths::ProjectSavedDir() // has "/" at end
    //               + TEXT("ComputerVisionPlugin/") + imageNameIn + TEXT(".png");
    SaveColorBufferAsPngFromPath(ColorData, Width, Height, path);
}


void ImageWriter::SaveColorBufferAsPngFromPath(uint8* ColorData, int32 Width, int32 Height, const FString& FilePath)
{


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
