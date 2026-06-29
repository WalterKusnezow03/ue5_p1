#include "GridColorizer.h"


FGridColorizer::FGridColorizer(){

}

FGridColorizer::~FGridColorizer(){

}



void FGridColorizer::ColorizeFromEpsilonFlag(
    Image &image, 
    TArray<TArray<float>> &grid,
    float epsilon,
    const FColor &color
){
    ColorizeFromEpsilonFlag(image, grid, epsilon, false, color);
}

void FGridColorizer::ColorizeFromEpsilonFlag(
    Image &image, 
    TArray<TArray<float>> &grid,
    float epsilon,
    bool useAbs,
    const FColor &color
){
    for (int i = 0; i < grid.Num(); i++){
        const TArray<float> &currentBuffer = grid[i];
        for (int j = 0; j < currentBuffer.Num(); j++){
            float scalar = currentBuffer[j];
            if(useAbs){
                scalar = std::abs(scalar);
            }

            if(scalar > epsilon){
                image.SetPixel(i, j, color);
            }
        }
    }
}

void FGridColorizer::ColorizeFromEpsilonFlagMix(
    Image &image,
    TArray<TArray<float>> &grid,
    float epsilon,
    bool useAbs,
    const FColor &color
){
    ColorizeFromEpsilonFlagMix(image, grid, epsilon, useAbs, color, 1.0f);
}

void FGridColorizer::ColorizeFromEpsilonFlagMix(
    Image &image,
    TArray<TArray<float>> &grid,
    float epsilon,
    bool useAbs,
    const FColor &color,
    float externalWeight
){
    for (int i = 0; i < grid.Num(); i++){
        const TArray<float> &currentBuffer = grid[i];
        for (int j = 0; j < currentBuffer.Num(); j++){
            float scalar = currentBuffer[j];
            if(useAbs){
                scalar = std::abs(scalar);
            }

            FColor current = image.GetPixel(i, j);
            FColor setColor = MixColor(color, current, scalar * externalWeight);

            if(scalar > epsilon){
                image.SetPixel(i, j, setColor);
            }
        }
    }
}

FColor FGridColorizer::MixColor(
    const FColor &a,
    const FColor &b,
    float weightA
){
    weightA = FMath::Clamp(weightA, 0.0f, 1.0f);
    float weightB = 1.0f - weightA;
    return MixColor(a, b, weightA, weightB);
}

FColor FGridColorizer::MixColor(
    const FColor &a,
    const FColor &b,
    float weightA,
    float weightB
){
    FColor result(
        a.R * weightA + b.R * weightB,
        a.G * weightA + b.G * weightB,
        a.B * weightA + b.B * weightB,
        255
    );
    return result;
}

void FGridColorizer::ColorizeFromUintFlag(
    Image &image,
    TArray<TArray<uint8>> &flagGrid,
    const FColor &color,
    bool flagsInverted
){
    for (int i = 0; i < flagGrid.Num(); i++){
        const TArray<uint8> &currentBuffer = flagGrid[i];
        for (int j = 0; j < currentBuffer.Num(); j++){
            bool flag = currentBuffer[j];
            if(flagsInverted){
                flag = !flag;
            }

            if(flag){
                image.SetPixel(i, j, color);
            }
        }
    }
}





void FGridColorizer::ColorizeFromLerp(
    Image &image,
    TArray<TArray<float>> &grid,
    const FColor &colorMin, 
    const FColor &colorMax
){
    int sizeX = grid.Num();
    if(grid.Num() > 0){
        FColor colorDir = DirColor(colorMin, colorMax);
        int sizeY = grid[0].Num();
        image.Setup(sizeX, sizeY);

        int indexColorbuffer = 0;
        for (int i = 0; i < grid.Num(); i++)
        {
            const TArray<float> &column = grid[i];
            for (int j = 0; j < column.Num(); j++){
                float scalar = column[j];
                image.SetPixel(i, j, LerpColor(colorMin, colorDir, scalar));
            }
        }
    }
}

FColor FGridColorizer::DirColor(
    const FColor &colorMin, 
    const FColor &colorMax
){
    FColor result;
    result.R = colorMax.R - colorMin.R;
    result.G = colorMax.G - colorMin.G;
    result.B = colorMax.B - colorMin.B;
    return result;
}

FColor FGridColorizer::LerpColor(
    const FColor &colorMin, 
    const FColor &colorDir,
    float scalar
){
    FColor result;
    result.R = colorMin.R + scalar * colorDir.R;
    result.G = colorMin.G + scalar * colorDir.G;
    result.B = colorMin.B + scalar * colorDir.B;

    uint8 zero = 0;
    uint8 maxVal = 255;
    result.R = std::max(result.R, zero);
    result.G = std::max(result.G, zero);
    result.B = std::max(result.B, zero);

    result.R = std::min(result.R, maxVal);
    result.G = std::min(result.G, maxVal);
    result.B = std::min(result.B, maxVal);

    result.A = 255;
    return result;
}









void FGridColorizer::ColorizeEdgePoints(
    Image &image,
    FMeshedPolygonHullSet &set,
    FColor color
){

    for (int i = 0; i < set.Num(); i++){
        FMeshedPolygonHullIndices &indices = set[i];

        for (int j = 0; j < indices.Num(); j++){
            std::pair<int, int> &pair = indices[j];
            image.SetPixel(pair.first, pair.second, color);
        }

            
    }

        
    
}

