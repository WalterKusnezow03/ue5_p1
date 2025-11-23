#pragma once 

#include "CoreMinimal.h"

//saves images for debugging purposes
class COMPUTERVISIONPLUGIN_API ImageWriter {

public:
    static void SaveColorBufferAsPng(uint8 *ColorData, int32 Width, int32 Height, int32 id);

    static int MaxImages(){
        return 10; 
    }

private:
    static FString imageName(int32 id);
    static void SaveColorBufferAsPng(uint8 *ColorData, int32 Width, int32 Height, const FString &FilePath);
};