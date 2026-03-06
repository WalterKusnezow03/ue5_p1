#pragma once

#include "StoragePlugin/Storage/BaseInterfacePaths/StorageInterfacePaths.h"

class STORAGEPLUGIN_API ImageLoader : public StorageInterfacePaths{

public:


    bool LoadPNGLocalPath(
        FString localFilePath,
        TArray<TArray<FColor>> &OutPixels
    );

protected:
    
    bool LoadPNGLocalPath(
        FString localFilePath, //no starting slash!
        TArray<FColor> &OutPixels, 
        int32 &Width, 
        int32 &Height
    );


    void Make2D(TArray<FColor> &raw, TArray<TArray<FColor>> &out, int32 with, int32 height);
    void Init2DArray(TArray<TArray<FColor>> &out, int32 with, int32 height);
    void Make2DIndex(int32 index, int32 width, int32 height, int32 &xOut, int32 &yOut);

    bool LoadPNG(
        const FString &FilePath, 
        TArray<FColor> &OutPixels, 
        int32 &Width, 
        int32 &Height
    );


};
