#include "Joint.h"
#include "GameCore/DebugHelper.h"

int Joint::id = 0;

Joint::Joint(UWorld *worldIn){
    child = nullptr;
    world = worldIn;
    ownId = id;
    id++;
}

Joint::Joint(UWorld *world, FVector &translationVector) : Joint(world){
    spatialTransform.setTranslation(translationVector);
}

Joint::Joint(Joint &other){
    if(this != &other){
        *this = other;
    }
}

Joint &Joint::operator=(Joint &other){
    if(this != &other){
        child = other.child;
        world = other.world;
        //spatialTransform = other.spatialTransform;
        ownId = other.ownId;
    }
    return *this;
}

Joint::~Joint(){
    child = nullptr;
}

void Joint::SetChild(Joint *childIn){
    if(childIn != nullptr){
        child = childIn;
    }
}


void Joint::addTorque(FVector &torqueIn){
    //sollte der start von allem sein


    //a = t * interia^-1
    //interia sollte vom child knochen kommen
    torque += torqueIn; //erst summieren
    recalculateAngularAcceleration();
}

void Joint::recalculateAngularAcceleration(){
    //acceleration
    angularAcceleration.X = skalarAngualarAcceleration(torque.X, 0.1f);
    angularAcceleration.Y = skalarAngualarAcceleration(torque.Y, 0.1f);
    angularAcceleration.Z = skalarAngualarAcceleration(torque.Z, 1.0f);
}

// muss schauen ob es später ein tensor wird.
float Joint::skalarAngualarAcceleration(float torqueVal, float interia){
    //a = t * interia^-1
    if(interia < 0.001f){
        return 0.0f;
    }
    float angularAccelerationSkalar = torqueVal * (1 / interia);
    return angularAccelerationSkalar;
}



void Joint::recalculateAngualarVelocity(float deltaTime){
    angularVelocity += angularAcceleration * deltaTime;
}


void Joint::copyDeltatime(float deltaTime){
    deltatime = deltaTime;
}


void Joint::Tick(float deltaTime, FVector &torqueIn){
    copyDeltatime(deltaTime);
    // falsch, deprecated, kommt später!
}




void Joint::Tick(float deltaTime, FVector &w, FVector &v){
    copyDeltatime(deltaTime);
    spatialTransform.forwardPluecker(w, v, deltaTime);
    if(child != nullptr){
        child->Tick(deltaTime, w, v); // erstmal einfach nicht weitergeben.
    }

    DebugHelper::showScreenMessage("joint tick ", ownId); //nur 2 zusehen...
}

void Joint::Build(MMatrix &inTransform){
    
    //from 6x6
    MMatrix result = spatialTransform * inTransform;
    transformCopy = result;

    //apply rotation to attached actor:
    FRotator actorRotation = result.extractRotator();
    FVector actorLocation = result.getTranslation();


    //build down chain to next link:
    if(child != nullptr){
        child->Build(result);
    }

    draw(inTransform, result);
}

void Joint::draw(MMatrix &a, MMatrix &b){
    FVector t1 = a.getTranslation();
    FVector t2 = b.getTranslation();
    FColor color = ownId % 2 == 0 ? FColor::Blue : FColor::Red;
    DebugHelper::showLineBetween(world, t1, t2, color, deltatime * 1.3f);
}



MMatrix Joint::endTransform(){
    if(child != nullptr){
        return child->endTransform();
    }
    return transformCopy;
}


int Joint::childCountDownstream(){
    int self = 1;
    if(child != nullptr){
        self += child->childCountDownstream();
    }
    return self;
}

bool Joint::isTwoBone(){

    return false;
}