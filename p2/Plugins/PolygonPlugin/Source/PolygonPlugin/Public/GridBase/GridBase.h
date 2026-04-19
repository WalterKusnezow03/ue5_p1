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

private:


};
