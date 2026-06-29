#pragma once

#include "CoreMinimal.h"
#include "ConvolutionOperator.h"

class POLYGONPLUGIN_API ConvolutionOperatorGauss : public ConvolutionOperator {

protected:
    float twoSigma2 = 1.0f;

public:
    
    ConvolutionOperatorGauss(float sigma, int sizeMask);

protected:
    

    float Gaussian(
        int i,
        int j,
        int xCenter,
        int yCenter
    ){
        float dx = float(i - xCenter); //AB = B - A
        float dy = float(j - yCenter);
        float dist2 = dx * dx + dy * dy; //size(dx,dy)^2
        float value = expf(-dist2 / twoSigma2);
        return value;
    }

private:


};

