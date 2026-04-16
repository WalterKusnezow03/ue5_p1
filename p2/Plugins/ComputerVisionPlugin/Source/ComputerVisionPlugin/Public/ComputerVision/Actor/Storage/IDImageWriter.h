#pragma once 

#include "CoreMinimal.h"

#include "StoragePlugin/Storage/ImageData/ImageWriter/ImageWriter.h"

//saves images for debugging purposes
class COMPUTERVISIONPLUGIN_API IDImageWriter : public ImageWriter {

public:
    static void SaveColorBufferAsPng(uint8 *ColorData, int32 Width, int32 Height, int32 id);
    

private:
    static FString imageName(int32 id);
    static int MaxImages(){
        return 100;
    }
};