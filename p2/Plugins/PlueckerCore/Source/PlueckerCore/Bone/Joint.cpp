#include "Joint.h"
#include "DebugPlugin/DebugHelper.h"


Joint::Joint(){

}

Joint::Joint(FVector translationVector){
    spatialTransform.setTranslation(translationVector);
}

Joint::Joint(FVector translationVector, UWorld *worldIn){
    spatialTransform.setTranslation(translationVector);
    SetWorld(worldIn);
}

Joint::Joint(const Joint &other){
    if(this != &other){
        *this = other;
    }
}

Joint &Joint::operator=(const Joint &other){
    if(this != &other){
        children = other.children;
        world = other.world;
        //spatialTransform = other.spatialTransform;
        color = other.color;
        SetBoneTranslationDirection(other.BoneTranslationDirection());

    }
    return *this;
}

Joint::~Joint(){
    
}

void Joint::SetWorld(UWorld *worldIn){
    world = worldIn;
}

void Joint::SetBoneTranslationDirection(FVector direction){
    spatialTransform.setTranslation(direction);
}

FVector Joint::BoneTranslationDirection()const {
    return spatialTransform.getTranslation();
}






void Joint::AddChild(Joint &childIn){
    children.Add(childIn);
}

void Joint::copyDeltatime(float deltaTime){
    deltatime = deltaTime;
}






//FVector &angularVelocity, //w
//FVector &linearVelocity,  //v
void Joint::Tick(float deltaTime, FVector &w, FVector &v){
    copyDeltatime(deltaTime);
    spatialTransform.forwardPluecker(w, v, deltaTime);

    //since more than one child can change wAngularVelocity and vLinVelcoity
    //it must be copied
    for (int i = 0; i < children.Num(); i++)
    {
        FVector wCopy = w;
        FVector vCopy = v;
        Joint &current = children[i];
        current.Tick(deltaTime, wCopy, vCopy);
    }
}

void Joint::Build(MMatrix &inTransform){
    
    //from 6x6
    MMatrix result = spatialTransform * inTransform;
    transformCopy = result;

    //apply rotation to attached actor:
    FRotator actorRotation = result.extractRotator();
    FVector actorLocation = result.getTranslation();


    //build down chain to next links:
    for (int i = 0; i < children.Num(); i++){
        Joint &current = children[i];
        current.Build(result);
    }
    
    draw(inTransform, result);
}

void Joint::draw(MMatrix &a, MMatrix &b){
    FVector t1 = a.getTranslation();
    FVector t2 = b.getTranslation();
    DebugHelper::showLineBetween(world, t1, t2, color, deltatime * 1.3f);
}





void Joint::SetDrawColor(FColor colorIn){
    color = colorIn;
}

void Joint::SetDrawColorRecursive(FColor colorA, FColor colorB, int layer){
    bool colorPick = layer % 2 == 0;
    layer++;
    FColor result = colorPick ? colorA : colorB;
    SetDrawColor(result);
    for (int i = 0; i < children.Num(); i++){
        Joint &current = children[i];
        current.SetDrawColorRecursive(colorA, colorB, layer);
    }
}

/// --- DEBUG ---
void Joint::DrawJointLocation(float deltaTime){
    if(world){
        DebugHelper::showLineBetween(
            world,
            FVector(0, 0, 0),
            transformCopy.getTranslation(),
            FColor::Green,
            deltaTime * 2.0f
        );
    }
}