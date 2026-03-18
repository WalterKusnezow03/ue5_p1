#include "RpgThrowable.h"
#include "CoreMath/util/SphereCaster.h"



ARpgThrowable::ARpgThrowable() : Super() {
    throwableType = EThrowableEnum::ERpg;
}

void ARpgThrowable::BeginPlay(){
    Super::BeginPlay();
    explosiveHelper.Setup(
        50 * 100,//int explosionRadiusIn, (audio: 50m)
        50,
        10 * 100
    );
    resetIgnoredActors();
    StartTimer();
}

void ARpgThrowable::throwIntoDirection(FVector start, FVector direction){
    if (!isThrown){
        StartTimer();
    }
    Super::throwIntoDirection(start, direction);
}

void ARpgThrowable::StartTimer(){
    detonationTimer.Begin(minDetonationTimer, false);
    timerFinished = false;
}

bool ARpgThrowable::TickTimer(float deltatime){
    if(!timerFinished){
        if(isThrown && !isDetonated){
            detonationTimer.Tick(deltatime);
            if(detonationTimer.timesUp()){
                timerFinished = true;
                return true;
            }
        }
    }

    

    return false;
}

void ARpgThrowable::Tick(float deltatime){
    Super::Tick(deltatime);
    UpdateSphereCast();
    TickTimer(deltatime);
    UpdateVelocity(deltatime);
}

void ARpgThrowable::UpdateVelocity(float deltatime){
    if(!isThrown || hitObject || isDetonated){
        return;
    }

    FVector dir = latestThrowingDirection.GetSafeNormal();
    FVector vDir = dir * velocityThrow;

    FVector a(0, 0, -981.0f);
    float deltatime2 = deltatime * deltatime;

    FVector x = GetActorLocation();
    FVector xt = x + vDir * deltatime + a * deltatime2; // + Fg
    SetActorLocation(xt);
}



void ARpgThrowable::UpdateSphereCast(){
    if(!isThrown){
        return;
    }

    if(!isDetonated && !hitObject){
        SphereCaster caster;
        FVector location = GetActorLocation();
        if(caster.AnyHitInRadius(
            GetWorld(),
            location,
            sphereCastRadius,
            ignoredActors
        )){
            hitObject = true;
        }
    }
    if(!isDetonated && hitObject){
        if (timerFinished){
            Detonate();
        }
    }

}




void ARpgThrowable::Detonate(){
    FVector location = GetActorLocation();
    explosiveHelper.detonate(location, GetWorld());
    isDetonated = true;
    release(); // hides.
}

void ARpgThrowable::release(){
    resetIgnoredActors();
    Super::release();
}

void ARpgThrowable::reset(){
    Super::reset();
    isDetonated = false;
    timerFinished = true;
    hitObject = false;
    resetIgnoredActors();
}



void ARpgThrowable::resetIgnoredActors(){
    ignoredActors.Empty();
    ignoredActors.Add(this);
}

void ARpgThrowable::AddIgnoredActor(AActor *actor){
    if(actor){
        if(ignoredActors.Contains(actor) == false){
            ignoredActors.Add(actor);
        }
    }
}