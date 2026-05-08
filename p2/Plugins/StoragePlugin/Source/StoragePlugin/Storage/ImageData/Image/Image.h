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
    FColor GetPixel(int x, int y) const;
    void AddPixel(int x, int y, FColor color);

    bool GetColorBuffer(TArray<FColor> &buffer) const;

    void Transpose();
    void FlipX();

    void SetAlpha(int alpha);

protected:
    bool wasLoaded = false;
    bool InBound(int x, int y);

    

    FColor ClampedSum(const FColor &a, const FColor &b);
    uint8 CampedSumInt(uint8 a, uint8 b);

    TArray<TArray<FColor>> data;
};