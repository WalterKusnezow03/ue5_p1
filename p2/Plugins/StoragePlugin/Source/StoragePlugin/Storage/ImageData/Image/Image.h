#pragma once

#include "CoreMinimal.h"
#include "StoragePlugin/Storage/ImageData/ImageLoader/ImageLoader.h"


class STORAGEPLUGIN_API Image {

public:
    bool LoadFromPath(FString localPath);
    bool IsValid();

    int widthX();
    int heightY();
    bool SizeValid();

    const TArray<TArray<FColor>> &GetData();

protected:
    bool wasLoaded = false;

    TArray<TArray<FColor>> data;
};