#include "ConvolutionOperator.h"




void ConvolutionOperator::SetMask(TArray<TArray<float>> &maskIn){
    mask = maskIn;
    ComputeValuesFromMask();
}

void ConvolutionOperator::ComputeValuesFromMask(){
    ComputeNormValue();
    ComputeSizes();
}

void ConvolutionOperator::ComputeNormValue(){
    normValue = 0.0f;
    for (int i = 0; i < mask.Num(); i++){
        TArray<float> &current = mask[i];
        for (int j = 0; j < current.Num(); j++){
            normValue += current[j];
        }
    }
}

void ConvolutionOperator::ComputeSizes(){
    halfX = (mask.Num() * 0.5f);
    halfY = 0;
    if (mask.Num() > 0){
        halfY = (mask[0].Num() * 0.5f);
    }
}




void ConvolutionOperator::ApplyOperator(TArray<TArray<float>> &grid) const {
    TArray<TArray<float>> copy = grid;
    for (int i = 0; i < grid.Num(); i++)
    {
        TArray<float> &column = grid[i];
        for (int j = 0; j < column.Num(); j++)
        {
            column[j] = ApplyOperatorOnPosition(copy, i, j);
        }
    }
}






//convlution operator for a single position x and y
float ConvolutionOperator::ApplyOperatorOnPosition(
    const TArray<TArray<float>> &gridBase,
    int xCenter,
    int yCenter
)const{
    int xBottom = xCenter - halfX;
    int yBottom = yCenter - halfY;
    
    float sum = 0.0f;
    for(int i = 0; i < mask.Num(); i++){
        const TArray<float> &column = mask[i];
        for(int j = 0; j < column.Num(); j++){
            int gridX = xBottom + i;
            int gridY = yBottom + j;
            if(InBound(gridX, gridY, gridBase)){
                float value = gridBase[gridX][gridY];
                sum += ApplyMask(value, i, j);
            }
        }
    }
    
    if(normalize && std::abs(normValue) > 0.0f){
        sum /= normValue;
    }
    return sum;
}




bool ConvolutionOperator::InBound(int i, int j, const TArray<TArray<float>> &grid) const {
    if(i >= 0 && i < grid.Num()){
        if(j >= 0 && j < grid[i].Num()){
            return true;
        }
    }
    return false;
}

float ConvolutionOperator::ApplyMask(float value, int i, int j) const {
    if(InBound(i, j, mask)){
        float maskValue = mask[i][j];
        return value * maskValue;
    }
    return 0.0f;
}









//convolution operator around a single position, but applying the full mask once around the pos!
void ConvolutionOperator::ApplyOperatorAt(TArray<TArray<float>> &grid, int x, int y){
    
    //copy partial grid
    TArray<TArray<float>> copied;
    CopyGridAtCenter(grid, x, y, copied);

    //apply gauss on partial grid
    ApplyOperator(copied);

    //copy partial grid back
    PasteGridAtCenter(grid, x, y, copied);
}

void ConvolutionOperator::CopyGridAtCenter(
    const TArray<TArray<float>> &gridBase, 
    int x, 
    int y, 
    TArray<TArray<float>> &outGrid
){
    if(mask.Num() <= 0){
        return;
    }

    int xStart = x - halfX;
    int yStart = y - halfY;

    outGrid.Empty();
    outGrid.SetNum(mask.Num());

    for(int i = 0; i < mask.Num(); i++){
        int end = mask[i].Num();
        outGrid[i].SetNum(end);
        for(int j = 0; j < end; j++){
            int xWorld = xStart + i;
            int yWorld = yStart + j;
            if(InBound(xWorld, yWorld, gridBase)){
                outGrid[i][j] = gridBase[xWorld][yWorld];
            }
        }
    }
}


void ConvolutionOperator::PasteGridAtCenter(
    TArray<TArray<float>> &gridBase, 
    int x, 
    int y, 
    const TArray<TArray<float>> &pasteGrid
){
    int xStart = x - halfX;
    int yStart = y - halfY;
    for (int i = 0; i < pasteGrid.Num(); i++){
        const TArray<float> &column = pasteGrid[i];
        for (int j = 0; j < column.Num(); j++){
            int xWorld = xStart + i;
            int yWorld = yStart + j;
            if(InBound(xWorld, yWorld, gridBase)){
                gridBase[xWorld][yWorld] = column[j];
            }
        }
    }
}