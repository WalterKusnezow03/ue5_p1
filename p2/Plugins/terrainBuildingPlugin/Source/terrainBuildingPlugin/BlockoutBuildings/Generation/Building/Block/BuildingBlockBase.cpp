#include "BuildingBlockBase.h"



void BuildingBlockBase::SetProperty(FBlockProperty &propertyIn){
    property = propertyIn;
}

FBlockProperty &BuildingBlockBase::GetPropertyRef(){
    return property;
}


//set area where mesh data generation is allowed
void BuildingBlockBase::SetOffsetAndQuadArea(const FVector &pivotIn, float sizeQuad){
    areaPivot = pivotIn;
    sizeArea = sizeQuad;
}


EBuildingTypeBlock BuildingBlockBase::GetTypeOfBlock() const {
    return property.typeBlockProperty;
}

bool BuildingBlockBase::CanHaveRotation() const {
    return property.CanHaveRotation();
}

bool BuildingBlockBase::IsInsideType() const {
    return property.IsInsideType();
}

void BuildingBlockBase::FaceRight(){
    rotation = 1;
}
void BuildingBlockBase::FaceLeft(){
    rotation = 3;
}
void BuildingBlockBase::FaceUp(){
    rotation = 0;
}
void BuildingBlockBase::FaceDown(){
    rotation = 2;
}

void BuildingBlockBase::UpdateRoationAndCornerFlag(
    const BuildingBlockBase &topOf, 
    const BuildingBlockBase &bottomOf, 
    const BuildingBlockBase &rightOf, 
    const BuildingBlockBase &leftOf 
){
    //logically:
    //^  x0x
    //y  0t0 //t -> this
    //|  x0x
    //|
    //.---x--->

    if(CanHaveRotation()){
        //if others can have rotation too, it must be aligned.
        isCorner = false;

        bool bottomOfRotation = bottomOf.CanHaveRotation();
        bool topOfRotation = topOf.CanHaveRotation();
        bool rigthOfRotation = rightOf.CanHaveRotation();
        bool leftOfRotation = leftOf.CanHaveRotation();

        //x1x
        //0t0
        //x1x
        if(bottomOfRotation && topOfRotation){
            if(IsInsideType()){
                FaceLeft(); //<- face
            }else{
                FaceRight(); //-> face
            }
        }
        //x0x
        //1t1
        //x0x
        if(leftOfRotation && rigthOfRotation){
            if(IsInsideType()){
                FaceDown();
            }else{
                FaceUp();
            }
        }


        //x0x
        //0t1
        //x1x
        if(bottomOfRotation && rigthOfRotation){
            FaceUp(); //default corner
            isCorner = true;
        }

        //x1x
        //0t1
        //x0x
        if(topOfRotation && rigthOfRotation){
            FaceDown(); //flip corner
            isCorner = true;
        }

        //x1x
        //1tx
        //x0x
        if(topOfRotation && rigthOfRotation){
            FaceDown(); //flip corner left
            isCorner = true;
        }


        //x0x
        //1tx
        //x1x
        if(topOfRotation && rigthOfRotation){
            FaceLeft(); //flip corner left
            isCorner = true;
        }


    }
}