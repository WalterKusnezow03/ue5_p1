#pragma once

#include "CoreMinimal.h"
#include "StoragePlugin/Storage/ImageData/ImageLoader/ImageLoader.h"


class STORAGEPLUGIN_API Image {

public:
    bool LoadFromPath(FString localPath);
    bool IsValid();

    int widthX()const;
    int heightY()const;
    bool SizeValid()const;

    const TArray<TArray<FColor>> &GetData();

    //manual fill
    void Setup(int x, int y);
    void SetPixel(int x, int y, FColor color);

    bool GetColorBuffer(TArray<FColor> &buffer) const;

protected:
    bool wasLoaded = false;

    TArray<TArray<FColor>> data;
};