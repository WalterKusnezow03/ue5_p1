#include "Joint.h"
#include "DebugPlugin/DebugHelper.h"
#include "CoreMath/util/Raycaster.h"


Joint::Joint(){
    
}

Joint::Joint(FVector translationVector){
    Setup(translationVector);
}

Joint::Joint(FVector translationVector, UWorld *worldIn){
    Setup(translationVector, worldIn);
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
        spatialTransformBase = other.spatialTransformBase;
        color = other.color;
        attachedActor = other.attachedActor;
        logEnabled = other.logEnabled;
        drawEnabled = other.drawEnabled;
        SetBoneTranslationDirection(other.BoneTranslationDirection());
    }
    return *this;
}

Joint::~Joint(){
    
}


void Joint::Setup(FVector &translationVector){
    GetSpatialTransform().setTranslation(translationVector);
    SetInteriaMatrixAuto();
}

void Joint::Setup(FVector &translationVector, UWorld *worldIn){
    SetWorld(worldIn);
    Setup(translationVector);
}

void Joint::SetWorld(UWorld *worldIn){
    world = worldIn;
    GetSpatialTransform().SetWorld(worldIn);

    FString message = worldIn != nullptr ? TEXT("Joint::SetWorld VALID") : TEXT("Joint::SetWorld INVALID");
    DebugHelper::logMessage(message);
}

void Joint::SetBoneTranslationDirection(FVector direction){
    GetSpatialTransform().setTranslation(direction);
    SetInteriaMatrixAuto();
}

FVector Joint::BoneTranslationDirection()const {
    return GetSpatialTransformConst().getTranslation();
}



void Joint::SetInteriaMatrixAuto(){



    //float I = mass * length * length * 0.3f;
    FVector boneTranslation = GetSpatialTransform().getTranslation();
    if(BoneTranslationValidForInteriaMatrix(boneTranslation)){
        SetInteriaMatrixAuto(boneTranslation);
    }
    else
    {
        SetInteriaMatrixSphere();
    }
}

void Joint::SetInteriaMatrixAuto(const FVector &boneTranslation){
    centerOfMass = boneTranslation * 0.5f;

    float L = boneTranslation.Size();

    float Iperp = mass * L * L * (1.0f / 12.0f);
    float Iaxis = Iperp / 10.0f; // Iperp * 5000.0f; // 0.05f;   // kleine aber stabile Trägheit

    Matrix3x3 I;
    I.scale(Iperp, Iperp, Iaxis);  // Z = Knochenachse
    interia = I;

    Matrix3x3 Iinv;
    Iinv.scale(
        1.0f / Iperp,
        1.0f / Iperp,
        1.0f / Iaxis
    );
    interiaInverse = Iinv;
    
}


bool Joint::BoneTranslationValidForInteriaMatrix(const FVector &size){
    return size.Size() > 1.0f;
}

//if the scaling elements are all near zero, any incoming torque will be killed.
void Joint::SetInteriaMatrixSphere(){
    centerOfMass = FVector(0, 0, 0);
    float r = 100.0f; // effektiver Radius //10
    float Ival = 0.4f * mass * r * r;

    Matrix3x3 I;
    I.scale(Ival, Ival, Ival);
    interia = I;

    Matrix3x3 Iinv;
    float inv = 1.0f / Ival;
    Iinv.scale(inv, inv, inv);
    interiaInverse = Iinv;
}


void Joint::AddChild(Joint &childIn){
    children.Add(childIn);
}


MMatrix Joint::GetRotation()const{
    MMatrix result;
    GetSpatialTransformConst().CopyRotationTo(result);
    return result;
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
    AddOwnJointVelociyTo(w, v); 

    //update spatial transform and return new world update
    GetSpatialTransform().forwardPluecker(w, v, deltaTime);

    MMatrix result = GetSpatialTransform() * inTransform;


    //update attached actor.
    UpdateActorTransform();
    draw(inTransform, result, deltaTime);

    //DO NOT REMOVE
    //VERY FUCKING IMPORTANT FOR ROOT JOINTS, internal transform cache to prevent 
    //extern reliance on a matrix.
    transformCopy = inTransform; //cache startingJoint

    return result;
}


/// --- FORCE ---

void Joint::AddForce(const FVector &force, float deltaTime){
    spatialVelocity.AddForce(force, mass, deltaTime);
    FVector torque = GetSpatialTransform().Torque(force, centerOfMass);
    spatialVelocity.AddTorque(torque, interiaInverse, deltaTime);
}


void Joint::ReactToDamage(const FCustomHitResult &hitResult){
   
    DebugHelper::showScreenMessage("Joint::ReactToDamage", FColor::Orange);

    const FVector &dir = hitResult.Direction();
    float sizeForce = 300.0f;
    FVector asForce = dir.GetSafeNormal() * sizeForce;
    asForce.Z *= -1.0f; //debug

    AddForce(asForce, hitResult.DeltaTime());


    //draw
    FVector worldLocation = GetWorldLocation();
    DebugHelper::showLineBetween(
        world,
        worldLocation,
        worldLocation + asForce,
        FColor::Purple,
        1.0f);
}

/// --- FORCE END ---

void Joint::AddOwnJointVelociyTo(FVector &w, FVector &v){
    //aus kraft integration eigene velocity anfügen
    spatialVelocity.AddOwnVelocityTo(w, v);
}









void Joint::OverrideJointRotation(const MMatrix &rotationMatrix){
    GetSpatialTransform().OverrideRotation(rotationMatrix);
}
void Joint::OverrideJointRotation(const FRotator &r){
    GetSpatialTransform().OverrideRotation(r);
}

//only for joints like this.
FVector Joint::GetWorldLocation() const {
    return GetSpatialTransformConst().ActorTranslationFromCache();
}



//external build of chain




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

    //FVector f = spatialTransform.Force(mass);
    //FVector n = spatialTransform.Torque(f, centerOfMass);

    //no motorized torque or force in end limb, added later.
    FVector f(0, 0, 0);
    FVector n(0, 0, 0);
    PropagateWrench(n, f, deltatime);
}

void Joint::PropagateWrench(FVector &n, FVector &f, float deltaTime){
   
    //F_i = F_{\text{eigene externe}} + \sum X^T F_{child}
    
    //propagate up
    //X^T * (n,f)
    GetSpatialTransform().backwardWrench(n,f);
    AddAndIntegrateOwnSptialForce(n, f, deltaTime);

    //global if is root
    if(ParentsByPointer.Num() == 0){
        spatialVelocity.AddForce(f, mass, deltaTime);
        spatialVelocity.AddTorque(n, interiaInverse, deltaTime);

        //draw
        FVector start = GetWorldLocation();
        DebugHelper::showLineBetween(
            world,
            start,
            start + f.GetSafeNormal() * 100.0f,
            FColor::Cyan,
            deltaTime * 2.0f
        );

        FVector fMeters = f / 100.0f;
        //DebugHelper::showScreenMessage("Joint::Root Force in meters ", fMeters);
    }

    PropagateWrench(n, f, deltaTime, ParentsByPointer);    
}


void Joint::AddAndIntegrateOwnSptialForce(FVector &outN, FVector &outF, float deltaTime){
    FVector forceSelf;
    FVector torqueSelf;
    FindSelfInteriaAndGravitySpatialMoment(torqueSelf, forceSelf);

    //integrate for next forward pass if has parents
    if(ParentsByPointer.Num() > 0){
        spatialVelocity.AddForce(forceSelf, mass, deltaTime);
        spatialVelocity.AddTorque(torqueSelf, interiaInverse, deltaTime); 
    }

    outF += forceSelf;
    outN += torqueSelf;
}



void Joint::FindSelfInteriaAndGravitySpatialMoment(
    FVector &outN, // torque
    FVector &outF // force
){
    //DebugHelper::showScreenMessage("Joint BASE FindSelfInteriaAndGravitySpatialMoment ", FColor::Yellow);
    FVector forceExternal = GetSpatialTransform().Force(mass);
    FVector torqueExternal = GetSpatialTransform().Torque(forceExternal, centerOfMass);

    FVector forceInteria(0,0,0);
    FVector torqueInteria(0,0,0);
    if(!disableCoriolisForce){
        spatialVelocity.ExtractCurrentForce(
            interia,
            mass,
            torqueInteria,
            forceInteria
        );
    }

    outN = torqueInteria + torqueExternal;
    outF = forceInteria + forceExternal;
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














void Joint::draw(const MMatrix &a, const MMatrix &b, float deltaTime){
    if(drawEnabled){
        deltaTime = std::max(deltaTime, 1.0f / 60.0f);
        FVector t1 = a.getTranslation();
        FVector t2 = b.getTranslation();
        DebugHelper::showLineBetween(world, t1, t2, color, deltaTime * 1.3f);
    }
    
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


FJointConstraint &Joint::GetConstraint(){
    return GetSpatialTransform().GetConstraint();
}

/// --- DEBUG ---
void Joint::DrawJointLocation(float deltaTime){
    if(world){
        DebugHelper::showLineBetween(
            world,
            FVector(0, 0, 0),
            GetWorldLocation(),
            FColor::Green,
            deltaTime * 2.0f
        );
    }
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
void Joint::UpdateActorTransform(){
    if(attachedActor){
        FVector t = GetSpatialTransform().ActorTranslationFromCache();
        FRotator r = GetSpatialTransform().ActorRotationFromCache();
        attachedActor->SetActorLocation(t);
        attachedActor->SetActorRotation(r);
    }
}
void Joint::SetActor(AActor *attachActor){
    attachedActor = attachActor;
}






void Joint::UpdateIgnoreParams(FCollisionQueryParams &ignoreParamsIn){
    GetSpatialTransform().UpdateIgnoreParams(ignoreParamsIn);
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


void Joint::SetDrawingEnabledRecursive(bool flag){
    SetDrawingEnabled(flag);
    SetDrawingEnabledUpStream(flag);
    SetDrawingEnabledDownStream(flag);
}

void Joint::SetDrawingEnabled(bool flag){
    drawEnabled = flag;
}


void Joint::SetDrawingEnabledDownStream(bool flag){
    TArray<Joint *> array = AllChildren();
    for (int i = 0; i < array.Num(); i++){
        if(Joint *joint = array[i]){
            joint->SetDrawingEnabled(flag);
            joint->SetDrawingEnabledDownStream(flag);
        }
    }
}

void Joint::SetDrawingEnabledUpStream(bool flag){
    for (int i = 0; i < ParentsByPointer.Num(); i++){
        if(Joint *joint = ParentsByPointer[i]){
            joint->SetDrawingEnabled(flag);
            joint->SetDrawingEnabledUpStream(flag);
        }
    }
}






void Joint::logGroundedState(FString prefix, FColor trueColor){
    bool result = GetSpatialTransform().bIsGrounded();
    FString a = prefix + " TRUE";
    FString b = prefix + " FALSE";
    DebugHelper::showScreenMessage(result, a, b, trueColor, FColor::Red);
}











/// recurive center of mass for root forces



//comAll = sum(m_i * c_i) / sum(m_i)
FVector Joint::CenterOfMassWorldWeightedRecursive(){
    FVector root = GetWorldLocation();
    float SumMass = 0.0f;
    FVector com = CenterOfMassWorldWeightedRecursive(root, SumMass);
    return com / SumMass;
}

FVector Joint::CenterOfMassWorldWeightedRecursive(const FVector &root, float &sumMass){
    //self
    FVector com = CenterOfMassWorldWeightedRelativeTo(root);
    sumMass += mass;

    //childs
    TArray<Joint *> array = AllChildren();
    for (int i = 0; i < array.Num(); i++){
        if(Joint *current = array[i]){
            com += current->CenterOfMassWorldWeightedRecursive(root, sumMass);
        }
    }
    return com;
}

FVector Joint::CenterOfMassWorldWeightedRelativeTo(const FVector &pos) const {
    return CenterOfMassWorldRelativeTo(pos) * mass;
}

FVector Joint::CenterOfMassWorldRelativeTo(const FVector &pos) const {
    //AB = B - A
    return CenterOfMassWorld() - pos;
}

FVector Joint::CenterOfMassWorld() const {
    return GetSpatialTransformConst().centerOfMassWorld(centerOfMass);
}