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


//external build of chain
MMatrix Joint::TickAndBuildThisJoint(FJointKinematicPropagatePackage &package){
    return TickAndBuildThisJoint(
        package.deltatime,
        package.w, // angular velocity
        package.v, // linear velocity
        package.transform
    );
}



MMatrix Joint::TickAndBuildThisJoint(
    float deltaTime,
    FVector &w, //angularVelocity, is updated
    FVector &v, //linearVelocity, is updated
    const MMatrix &inTransform
){
    spatialTransform.forwardPluecker(w, v, deltaTime);
    MMatrix result = spatialTransform * inTransform;
    transformCopy = result;
    return transformCopy;
}

void Joint::OverrideJointRotation(const MMatrix &rotationMatrix){
    spatialTransform.OverrideRotation(rotationMatrix);
}


void Joint::OverrideJointRotationTransposed(const Joint &other){
    Matrix3x3 rotationOther = other.spatialTransform.GetRotation();
    rotationOther.transpose(); //R^T = R^-1 bei R SO3
    spatialTransform.OverrideRotation(rotationOther);
}

//external build of chain





//FVector &angularVelocity, //w (roll pitch yaw)(?)
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

/// constraints
void Joint::OverrideConstraint(FJointConstraint &in){
    spatialTransform.OverrideConstraint(in);
}

FJointConstraint &Joint::GetConstraint(){
    return spatialTransform.GetConstraint();
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