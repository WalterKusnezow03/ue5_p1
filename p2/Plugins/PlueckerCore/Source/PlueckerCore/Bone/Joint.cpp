#include "Joint.h"
#include "DebugPlugin/DebugHelper.h"
#include "CoreMath/util/Raycaster.h"


Joint::Joint(){

}

Joint::Joint(FVector translationVector){
    spatialTransform.setTranslation(translationVector);
    SetInteriaMatrixAuto();
}

Joint::Joint(FVector translationVector, UWorld *worldIn){
    spatialTransform.setTranslation(translationVector);
    SetWorld(worldIn);
    SetInteriaMatrixAuto();
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
    SetInteriaMatrixAuto();
}

FVector Joint::BoneTranslationDirection()const {
    return spatialTransform.getTranslation();
}



void Joint::SetInteriaMatrixAuto(){
    //float I = mass * length * length * 0.3f;
    FVector boneTranslation = spatialTransform.getTranslation();
    centerOfMass = boneTranslation * 0.5f; //at bottom

    float lengthBone = boneTranslation.Size() * 0.5f;
    float l2 = lengthBone * lengthBone;
    float scale = mass * l2 * 0.3f;
    float s1 = 1.0f / scale;
    float rotationAroundZ = 0.000000000001f;


    //testing needed if stable!
    Matrix3x3 I1;
    I1.scale(s1,s1,rotationAroundZ);
    interiaInverse = I1;


    /*
    //was stable but slow!
    s1 = rotationAroundZ;
    Matrix3x3 I1;
    I1.scale(s1,s1,rotationAroundZ);
    interiaInverse = I1;*/
}

void Joint::SetInteriaMatrix(const Matrix3x3 &interiaIn){
    interiaInverse = interiaIn.jordanInverse();
}



void Joint::AddChild(Joint &childIn){
    children.Add(childIn);
}



//external build of chain
MMatrix Joint::TickAndBuildThisJoint(FJointKinematicPropagatePackage &package){
    return TickAndBuildThisJoint(
        package.deltatime,
        package.w, // angular velocity -> is updated for next joint
        package.v, // linear velocity -> is updated for next joint
        package.transform 
    );
}


MMatrix Joint::TickAndBuildThisJoint(
    float deltaTime,
    const MMatrix &inTransform
){
    FVector wIgnored;
    FVector vIgnored;
    return TickAndBuildThisJoint(deltaTime, wIgnored, vIgnored, inTransform);
}

MMatrix Joint::TickAndBuildThisJoint(
    float deltaTime,
    FVector &w, //angularVelocity, is updated
    FVector &v, //linearVelocity, is updated
    const MMatrix &inTransform
){
    //add gravity force to spatial vector
    TickGravityAndAddUpdateToSptialVector(deltaTime);

    //add incoming velocities and copy back inside for propagation
    UpdateSpatialVelocityAndPassedVelocities(w, v); 

    //update spatial transform and return new world update
    spatialTransform.forwardPluecker(w, v, deltaTime);
    MMatrix result = spatialTransform * inTransform;
    transformCopy = result; //cache
    return transformCopy;
}


/// --- FORCE ---

void Joint::AddForce(const FVector &force, float deltaTime){
    spatialVelocity.AddForce(force, mass, deltaTime);
    FVector torque = Torque(force);
    AddTorque(torque, deltaTime);
}

void Joint::AddTorque(const FVector &torque, float deltaTime){
    spatialVelocity.AddTorque(torque, interiaInverse, deltaTime);
}

/// --- FORCE ---

void Joint::UpdateSpatialVelocityAndPassedVelocities(FVector &w, FVector &v){
    spatialVelocity.AddVelocity(w,v); //update self joint
    spatialVelocity.copy(w, v); //update summed values for spatial transform update 
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



FJointKinematicPropagatePackage Joint::GeneratePackage(
    MMatrix &transform,
    float deltaTime
){
    FJointKinematicPropagatePackage outPackage;
    outPackage.transform = transform;
    outPackage.deltatime = deltaTime;
    spatialVelocity.copy(outPackage.w, outPackage.v); //paste own velocities inside
    return outPackage;
}






void Joint::TickAndBuildRecursive(
    float deltaTime,
    FVector &w,
    FVector &v,
    MMatrix &transform
){
    //Includes gravity update.
    MMatrix result = TickAndBuildThisJoint(
        deltaTime,
        w, //angularVelocity, is updated
        v, //linearVelocity, is updated
        transform
    );

    //log sptail velocities
    LogSpatialVelocities("Joint::TickAndBuildRecursive",w, v);

    //since more than one child can change wAngularVelocity and vLinVelcoity
    //it must be copied
    for (int i = 0; i < children.Num(); i++)
    {
        FVector wCopy = w;
        FVector vCopy = v;
        MMatrix resultCopy = result;
        Joint &current = children[i];
        current.TickAndBuildRecursive(deltaTime, wCopy, vCopy, resultCopy);
    }

    draw(transform, result, deltaTime);
}

void Joint::LogSpatialVelocities(FString Prefix, const FVector &w, const FVector &v){
    FString message = FString::Printf(
        TEXT("%s w(%.2f, %.2f, %.2f), v(%.2f, %.2f, %.2f)"),
        *Prefix,
        w.X, w.Y, w.Z,
        v.X, v.Y, v.Z
    );
    DebugHelper::showScreenMessage(message, FColor::Cyan);
}


void Joint::draw(MMatrix &a, MMatrix &b, float deltaTime){
    deltaTime = std::max(deltaTime, 1.0f / 60.0f);
    FVector t1 = a.getTranslation();
    FVector t2 = b.getTranslation();
    DebugHelper::showLineBetween(world, t1, t2, color, deltaTime * 1.3f);
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





/// --- GRAVITY ---

/// --- Grounded flag ---

void Joint::OverrideWorldLocation(FVector pos){
    transformCopy.setTranslation(pos);
}

void Joint::TickUpdateGroundedFlag(){
    if(world){
        DebugHelper::showScreenMessage("Joint::TickUpdateGroundedFlag", FColor::Cyan);
        bIsGrounded = false;
        FVector Start = transformCopy.getTranslation();
        FVector dir(0, 0, -1.0f);
        FVector outputHit;

        

        Raycaster raycaster;
        if(raycaster.performRaycast(
            world, 
            Start, 
            dir,
            distanceToGroundedFlag, 
            outputHit,
            ignoreParams
        )){
            bIsGrounded = true;
            DebugHelper::showScreenMessage("Joint::ISGROUNDED", FColor::Green);
        }
    }
}

bool Joint::JointIsGrounded(){
    return bIsGrounded;
}


//w and v changed but no transform update in
//spatial transform.
void Joint::TickGravityAndAddUpdateToSptialVector(float deltaTime){
    //includes torque and force update, integrated to velocities.
    //joint rebuild needed.
    FVector g = GravityForce();
    AddForce(g, deltaTime);
}



//no degree unlock
void Joint::TickGravity(float deltaTime, MMatrix &updateTransform){
    MMatrix result = TickAndBuildThisJoint(deltaTime, updateTransform);
    updateTransform.setTranslation(result);
}

//unlock if update needed for transform
void Joint::TickGravity6DOF(float deltaTime, MMatrix &updateTransform){
    FJointConstraint &constraintOfJoint = GetConstraint();
    constraintOfJoint.UnLockPositionConstraint(); //unlock constraint
    TickGravity(deltaTime, updateTransform);
    constraintOfJoint.LockPositionConstraint(); //lock constraint
}





//torque = com x Fg

FVector Joint::Torque(FVector force){
    Matrix3x3 R1 = spatialTransform.GetRotation();
    R1.transpose(); //is not R^T = R^-1

    // centerOfMass ist lokal
    // GravityForce ist global, aber transformiere nur die Richtung in lokal
    FVector gravityLocal = R1 * force;
    return FVector::CrossProduct(centerOfMass, gravityLocal);
}


FVector Joint::GravityForce(){
    float damp = 1.0f;
    return FVector(0, 0, -981.0f) * mass * damp;
}