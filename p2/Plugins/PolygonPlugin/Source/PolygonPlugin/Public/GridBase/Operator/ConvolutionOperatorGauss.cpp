#include "ConvolutionOperatorGauss.h"



ConvolutionOperatorGauss::ConvolutionOperatorGauss(float sigma, int sizeMask){
    float sigma2 = sigma * sigma;
    twoSigma2 = 2.0f * sigma2;

    TArray<TArray<float>> maskIn;
    for (int i = 0; i < sizeMask; i++){
        TArray<float> inner;
        inner.SetNum(sizeMask);
        maskIn.Add(inner);
    }

    int center = sizeMask / 2;

    for (int i = 0; i < maskIn.Num(); i++){
        TArray<float> &current = maskIn[i];
        for (int j = 0; j < current.Num(); j++){
            current[j] = Gaussian(i, j, center, center);
        }
    }
    SetMask(maskIn);
}