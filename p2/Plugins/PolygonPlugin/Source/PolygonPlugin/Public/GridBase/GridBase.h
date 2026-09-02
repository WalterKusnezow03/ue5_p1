#pragma once

#include "CoreMinimal.h"


class POLYGONPLUGIN_API GridBase {

public:

    template<typename T>
    bool TGridIsSize(int x, int y, TArray<TArray<T>> &grid){
        if(x > 0 && y > 0){
            if(grid.Num() == x){
                if(grid[0].Num() == y){
                    return true;
                }
            }
        }
        
        return false;
    }

    template<typename T>
    void TGenerateGrid(int x, int y, TArray<TArray<T>> &grid){
        //expects grid to be a quad!
        if(TGridIsSize<T>(x,y, grid)){
            return;
        }

        if(x > 0 && y > 0){
            grid.SetNum(x);
            for (int i = 0; i < x; i++){
                TArray<T> &currentGridBuffer = grid[i];
                currentGridBuffer.SetNum(y);
            }
        }
    }




    //similarity if same size
    //MAE loss based (Mean Absolute Error)
    template<typename T>
    float TSimilarity(
        const TArray<TArray<T>> &gridA,
        const TArray<TArray<T>> &gridB
    ){
        float lossTotal = TMAELoss<T>(gridA, gridB);
        float similarity = FMath::Clamp(1.0f - lossTotal, 0.0f, 1.0f);
        return similarity;
    }

    template<typename T>
    float TMAELoss(
        const TArray<TArray<T>> &gridA,
        const TArray<TArray<T>> &gridB
    ){
        if(TGridIsSameSize(gridA, gridB)){
            float range = Range<T>(gridA, gridB);
            range = std::max(range, 0.00001f); //prevent div by 0

            float countSamples = 0.0f;
            float lossTotal = 0.0f;

            for (int i = 0; i < gridA.Num(); i++){
                const TArray<T> &current = gridA[i];
                const TArray<T> &other = gridB[i];
                for (int j = 0; j < current.Num(); j++){
                    if(j >= 0 && j < other.Num()){
                        countSamples += 1.0f;

                        float bAsFloat = other[j];
                        float aAsFloat = current[j];

                        float loss = (bAsFloat - aAsFloat) / range; // AB = B - A
                        lossTotal += std::abs(loss);
                    }
                }
            }
            if(countSamples > 0.0f){
                lossTotal /= countSamples;
            }else{
                lossTotal = 1.0f;
            }
            return lossTotal;
        }
        return 1.0f;
    }

private:



    template<typename T>
    bool TGridIsSameSize(
        const TArray<TArray<T>> &gridA,
        const TArray<TArray<T>> &gridB
    ){
        int sizeX = gridA.Num();
        int sizeXOther = gridB.Num();
        if(sizeX == sizeXOther){
            if(sizeX > 0){
                int sizeY = gridA[0].Num();
                int sizeYOther = gridB[0].Num();
                return sizeY == sizeYOther;
            }
        }
        return false;
    }

    template<typename T>
    float Range(
        const TArray<TArray<T>> &gridA,
        const TArray<TArray<T>> &gridB
    ){
        return std::max(Range<T>(gridA), Range<T>(gridB));
    }

    template<typename T>
    float Range(const TArray<TArray<T>> &gridA){
        float min = std::numeric_limits<float>::max();
        float max = -min;

        for (int i = 0; i < gridA.Num(); i++){
            const TArray<T> &current = gridA[i];
            for (int j = 0; j < current.Num(); j++)
            {
                float asFloat = current[j];
                min = std::min(asFloat, min);
                max = std::max(asFloat, max);
            }
        }
        return std::abs(max - min); //AB = B - A
    }
};
