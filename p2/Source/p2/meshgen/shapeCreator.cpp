// Fill out your copyright notice in the Description page of Project Settings.

#include "p2/util/FVectorUtil.h"
#include "shapeCreator.h"

shapeCreator::shapeCreator()
{
}

shapeCreator::~shapeCreator()
{
}


/// @brief will fill an array and override all data with a new shape
/// @param output all data will be overriden and vector resized
void shapeCreator::createShape(std::vector<FVector2D> &output){

    std::vector<FVector2D> copy;
    int random = FVectorUtil::randomNumber(0, 1);
    if(random == 0){
        copy = shape0();
    }
    if(random == 1){
        copy = shape1();
    }

    //copy data
    for (int i = 0; i < copy.size(); i++){
        if(i < output.size()){
            output.at(i) = copy.at(i);
        }else{
            output.push_back(copy.at(i));
        }
    }

    //erase left over
    if(copy.size() < output.size()){
        output.erase(output.begin() + copy.size(), output.end());
    }
}





std::vector<FVector2D> shapeCreator::shape1(){
    return shape0();
}

/// @brief creates a set of sample points
/// @return as vector of FVector2D
std::vector<FVector2D> shapeCreator::shape0(){
    std::vector<FVector2D> output;

    //One meter. Dont Change.
    int scale = terrainCreator::ONEMETER; //upscale to meters 

    //testing simple line shape
    int xstep = 2;
    int x = 0;
    int nums[] = {2, 1, 4, 5, 2, 3, 1, 1, 2};
    int size =  sizeof(nums) / sizeof(nums[0]);
    bool flipped = false;

    for (int i : nums){
        FVector2D a(x, i);
        if(x / 2 < size / 2 && !flipped){
            x += xstep;
        }else{
            flipped = true;
            x -= xstep;
        }
       
        a *= scale; //upscale
        output.push_back(a);
    }

    
    return output;
}