#pragma once 

#include "CoreMinimal.h"

//saves images for debugging purposes
class STORAGEPLUGIN_API ImageWriter {

public:
    static void SaveColorBufferAsPngFromName(
        uint8 *ColorData,
        int32 Width,
        int32 Height,
        FString pluginName,
        FString imageNameIn
    );

    static void SaveColorBufferAsPngFromPath(
        uint8 *ColorData,
        int32 Width,
        int32 Height,
        const FString& FilePath
    );
private:
   
};