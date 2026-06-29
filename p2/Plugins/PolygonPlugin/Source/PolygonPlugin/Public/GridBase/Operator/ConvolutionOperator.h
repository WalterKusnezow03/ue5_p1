#pragma once

#include "CoreMinimal.h"


class POLYGONPLUGIN_API ConvolutionOperator {

public:
    void ApplyOperator(TArray<TArray<float>> &grid) const;

    void SetMask(TArray<TArray<float>> &maskIn);

    //convolution operator around a single position, but applying the full mask once around the pos!
    void ApplyOperatorAt(TArray<TArray<float>> &grid, int x, int y);

protected:
    float ApplyOperatorOnPosition(
        const TArray<TArray<float>> &gridBase,
        int xCenter,
        int yCenter
    ) const;

    TArray<TArray<float>> mask;
    bool normalize = true;
    float normValue = 1.0f;

    float ApplyMask(float value, int i, int j) const;

    

private:
    int halfX = 0;
    int halfY = 0;
    

    void ComputeValuesFromMask();
    void ComputeNormValue();
    void ComputeSizes();

    bool InBound(int i, int j, const TArray<TArray<float>> &grid) const;



    void CopyGridAtCenter(
        const TArray<TArray<float>> &gridBase,
        int x,
        int y,
        TArray<TArray<float>> &outGrid
    );
    void PasteGridAtCenter(
        TArray<TArray<float>> &gridBase,
        int x,
        int y,
        const TArray<TArray<float>> &pasteGrid
    );
};