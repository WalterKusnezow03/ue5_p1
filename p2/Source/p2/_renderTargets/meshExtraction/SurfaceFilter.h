#pragma once


#include "CoreMinimal.h"

class P2_API SurfaceFilter{

public:
    SurfaceFilter();
    ~SurfaceFilter();

    void filter(
        TArray<FVector> &vertecies,
        TArray<FVector> &normals,
        TArray<int32> &trianlges
    );

private:
    

    void findVerteciesInSimilarNormalDirection(
        int index,
        std::vector<bool> &indexCovered,
        TArray<int32> &foundVertecies,
        TArray<FVector> &normals
    );

    void findConnectedSurfaces(
        TArray<int32> &inbuffer,
        TArray<TArray<int32>> &outbuffers
    );

    int canFindInBuffer(int index, TArray<TArray<int32>> &buffers);
    bool canFind(int index, TArray<int32> &buffer);
};