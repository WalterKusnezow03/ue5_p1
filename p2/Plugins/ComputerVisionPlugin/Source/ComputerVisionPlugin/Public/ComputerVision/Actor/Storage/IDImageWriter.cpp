#include "IDImageWriter.h"




void IDImageWriter::SaveColorBufferAsPng(uint8* ColorData, int32 Width, int32 Height, int32 id){
    if(id > MaxImages()){
        return;
    }
    SaveColorBufferAsPngFromName(
        ColorData,
        Width,
        Height,
        "ComputerVisionPlugin",
        imageName(id)
    );
}

FString IDImageWriter::imageName(int32 id){
    return FString::Printf(TEXT("CV_%d.png"), id);
}
