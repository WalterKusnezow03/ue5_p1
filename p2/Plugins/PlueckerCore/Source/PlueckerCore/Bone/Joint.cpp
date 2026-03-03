#include "Joint.h"
#include "DebugPlugin/DebugHelper.h"
#include "CoreMath/util/Raycaster.h"


Joint::Joint(){
    SetLogEnabled(false);
}

Joint::Joint(FVector translationVector){
    spatialTransform.setTranslation(translationVector);
    SetInteriaMatrixAuto();
    SetLogEnabled(false);
}

Joint::Joint(FVector translationVector, UWorld *worldIn){
    spatialTransform.setTranslation(translationVector);
    SetWorld(worldIn);
    SetInteriaMatrixAuto();
    SetLogEnabled(false);
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
        spatialTransform = other.spatialTransform;
        color = other.color;
        is6DOF = other.is6DOF;
        attachedActor = other.attachedActor;
        logEnabled = other.logEnabled;
        SetBoneTranslationDirection(other.BoneTranslationDirection());
        SetLogEnabled(other.logEnabled);
    }
    return *this;
}

Joint::~Joint(){
    
}

void Joint::SetLogEnabled(bool flag){
    logEnabled = flag;
    spatialTransform.bLogMessage = flag;
    for (int i = 0; i < children.Num(); i++)
    {
        Joint &current = children[i];
        current.SetLogEnabled(flag);
    }
    for (int i = 0; i < ChildsByPointer.Num(); i++){
        if(Joint *current = ChildsByPointer[i]){
            current->SetLogEnabled(flag);
        }
    }
}

void Joint::SetWorld(UWorld *worldIn){
    world = worldIn;
    spatialTransform.SetWorld(worldIn);

    FString message = worldIn != nullptr ? TEXT("Joint::SetWorld VALID") : TEXT("Joint::SetWorld INVALID");
    DebugHelper::logMessage(message);
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
    //-- CAUTION -> MOVING TO BACKWARDS PROPAGATION! --
    //add gravity force to spatial vector
    //TickGravityAndAddUpdateToSptialVector(deltaTime);

    //add incoming velocities and copy back inside for propagation
    UpdateSpatialVelocityAndPassedVelocities(w, v); 

    //update spatial transform and return new world update
    spatialTransform.forwardPluecker(w, v, deltaTime);

    MMatrix result = spatialTransform * inTransform;


    //update attached actor.
    UpdateActorTransform(inTransform, result);
    draw(inTransform, result, deltaTime);

    transformCopy = inTransform; //cache startingJoint

    return result;
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


void Joint::ReactToDamage(const FCustomHitResult &hitResult){
    if(true){
        return;
    }

    const FVector &dir = hitResult.Direction();

    float sizeForce = 1.0f;
    FVector asForce = dir.GetSafeNormal() * sizeForce;
    AddForce(asForce, hitResult.DeltaTime());
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

void Joint::OverrideJointWorldTransform(FVector pos, FRotator roation){
    OverrideWorldLocation(pos);
    spatialTransform.OverrideRotation(roation);
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
    float deltaTime
){
    MMatrix m = transformCopy;
    FVector w(0, 0, 0);
    FVector v(0, 0, 0);
    TickAndBuildRecursive(deltaTime, w, v, m);
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
    //LogSpatialVelocities("Joint::TickAndBuildRecursive",w, v);
    if(HasChildren()){
        TickAndBuildAll(AllChildren(), w, v, result, deltaTime); //includes parents.
    }else{
        PropagateWrench(deltaTime);
    }
}

TArray<Joint *> Joint::AllChildren(){
    TArray<Joint *> outArray = ChildsByPointer;
    for (int i = 0; i < children.Num(); i++){
        Joint *current = &children[i];
        outArray.Add(current);
    }
    return outArray;
}




bool Joint::HasChildren(){
    return children.Num() > 0 || ChildsByPointer.Num() > 0;
}


void Joint::TickAndBuildAll(TArray<Joint*> array, FVector w, FVector v, MMatrix root, float deltatime){
    for (int i = 0; i < array.Num(); i++){
        if(Joint *current = array[i]){
            TickAndBuildChild(*current, w, v, root, deltatime);
        }
    }
}

void Joint::TickAndBuildChild(Joint &joint, FVector w, FVector v, MMatrix root, float deltaTime){
    joint.TickAndBuildRecursive(deltaTime, w, v, root);
}






void Joint::PropagateWrench(float deltatime){
    //FVector f = GravityForce();
    //FVector n = GravityTorque();

    FVector f = spatialTransform.Force(mass);
    FVector n = spatialTransform.Torque(f, centerOfMass);
    PropagateWrench(n, f, deltatime);
}

void Joint::PropagateWrench(FVector &n, FVector &f, float deltaTime){
   
    //F_i = F_{\text{eigene externe}} + \sum X^T F_{child}
    
    //propagate up
    spatialTransform.backwardWrench( //X^T * (n,f)
        n, //n
        f, //f
        deltaTime
    );

    f += spatialTransform.Force(mass);
    n += spatialTransform.Torque(f, centerOfMass);

    //integrate, will be build in next frame!
    spatialVelocity.AddForce(f, mass, deltaTime);
    spatialVelocity.AddTorque(n, interiaInverse, deltaTime); 

    PropagateWrench(n, f, deltaTime, ParentsByPointer);
    
}

void Joint::PropagateWrench(FVector &n, FVector &f, float deltatime, TArray<Joint*> &parents){
    for (int i = 0; i < parents.Num(); i++){
        if(Joint *current = parents[i]){
            FVector nCopy = n;
            FVector fCopy = f;
            current->PropagateWrench(nCopy, fCopy, deltatime);
        }
    }
}








//as root
void Joint::TickAndBuildRecursiveAsRoot(float deltaTime){
    TickGravityAndAddUpdateToSptialVector(deltaTime); //a nur in root
    TickAndBuildRecursive(deltaTime);
    LogPosition("Joint::TickAndBuildRecursiveAsRoot Result-> ");
}

void Joint::TickAndBuildRecursiveAsRoot(
    float deltaTime,
    FVector &w,
    FVector &v,
    MMatrix &transform
){
    TickGravityAndAddUpdateToSptialVector(deltaTime); //a nur in root
    TickAndBuildRecursive(deltaTime,w,v,transform);
}







void Joint::LogSpatialVelocities(FString Prefix, const FVector &w, const FVector &v){
    if(logEnabled){
        FString message = FString::Printf(
            TEXT("%s w(%.2f, %.2f, %.2f), v(%.2f, %.2f, %.2f)"),
            *Prefix,
            w.X, w.Y, w.Z,
            v.X, v.Y, v.Z
        );
        DebugHelper::showScreenMessage(message, FColor::Cyan);
    }
}

void Joint::LogPosition(FString Prefix){
    if(logEnabled){
        FVector pos = transformCopy.getTranslation();
        FString message = FString::Printf(
            TEXT("%s pos(%.2f, %.2f, %.2f)"),
            *Prefix,
            pos.X, pos.Y, pos.Z
        );
        DebugHelper::showScreenMessage(message, FColor::Cyan);
    }
}




void Joint::draw(const MMatrix &a, const MMatrix &b, float deltaTime){
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

void Joint::OverrideWorldLocation(MMatrix mat){
    transformCopy.setTranslation(mat.getTranslation());
}

void Joint::OverrideWorldLocation(FVector pos){
    transformCopy.setTranslation(pos);
}

//w and v changed but no transform update in
//spatial transform.
void Joint::TickGravityAndAddUpdateToSptialVector(float deltaTime){
    
    //includes torque and force update, integrated to velocities.
    //joint rebuild needed.
    FVector g = GravityForce();
    AddForce(g, deltaTime);
}





//torque = com x Fg
FVector Joint::GravityTorque(){
    return Torque(GravityForce());
}

FVector Joint::Torque(FVector force){
    return spatialTransform.Torque(force, centerOfMass);
}


FVector Joint::GravityForce(){
    float damp = 1.0f; //debug
    return FVector(0, 0, -981.0f) * mass * damp;
}






/// --- new add childs by pointer ! ---
void Joint::AddChildByPointer(Joint *jIn){
    if(jIn){
        if(ChildsByPointer.Contains(jIn) == false){
            ChildsByPointer.Add(jIn);

            //new.
            jIn->AddParentByPointer(this);
        }
    }
}

void Joint::AddChildsByPointer(TArray<Joint*> childs){
    for (int i = 0; i < childs.Num(); i++)
    {
        AddChildByPointer(childs[i]);
    }
}


void Joint::AddParentByPointer(Joint *jIn){
    if(jIn){
        if(ParentsByPointer.Contains(jIn) == false){
            ParentsByPointer.Add(jIn);

            //could add as child aswell for safety.
        }
    }
}

void Joint::BuildParentingRecursive(){
    
    TArray<Joint *> array = AllChildren();
    for (int i = 0; i < array.Num(); i++)
    {
        if(Joint *current = array[i]){
            current->AddParentByPointer(this);
            current->BuildParentingRecursive();
        }
    }
}



//t and R, in is always translation, updated always rotation.
void Joint::UpdateActorTransform(const MMatrix &transformIn, const MMatrix &transformUpdate){
    if(attachedActor){
        FVector t = transformIn.getTranslation();
        FRotator r = transformUpdate.extractRotator();
        attachedActor->SetActorLocation(t);
        attachedActor->SetActorRotation(r);
    }
}
void Joint::SetActor(AActor *attachActor){
    attachedActor = attachActor;
}






void Joint::UpdateIgnoreParams(FCollisionQueryParams &ignoreParamsIn){
    spatialTransform.UpdateIgnoreParams(ignoreParamsIn);
}
void Joint::UpdateIgnoreParamsRecursive(FCollisionQueryParams &ignoreParamsIn){
    UpdateIgnoreParams(ignoreParamsIn);
    UpdateIgnoreParamsUpStream(ignoreParamsIn);
    UpdateIgnoreParamsDownStream(ignoreParamsIn);
}

void Joint::UpdateIgnoreParamsDownStream(FCollisionQueryParams &params){
    TArray<Joint *> array = AllChildren();
    for (int i = 0; i < array.Num(); i++){
        if(Joint *joint = array[i]){
            joint->UpdateIgnoreParams(params);
            joint->UpdateIgnoreParamsDownStream(params);
        }
    }
}

void Joint::UpdateIgnoreParamsUpStream(FCollisionQueryParams &params){
    for (int i = 0; i < ParentsByPointer.Num(); i++){
        if(Joint *joint = ParentsByPointer[i]){
            joint->UpdateIgnoreParams(params);
            joint->UpdateIgnoreParamsUpStream(params);
        }
    }
}