#include "SlipForceCache.h"

#include "DebugPlugin/DebugHelper.h"

int SlipForceCache::idGlobal = 0;

SlipForceCache::SlipForceCache(){
    cacheSetupflag = false;
    id = idGlobal++;
}

SlipForceCache::~SlipForceCache(){
    idGlobal = 0;
}

SlipForceCache::SlipForceCache(const SlipForceCache &other){
    if(this != &other){
        *this = other;
    }
}

SlipForceCache &SlipForceCache::operator=(const SlipForceCache &other){
    if(this != &other){
        forceFrames = other.forceFrames;
        flagged = other.flagged;
        timeOfAnimation = other.timeOfAnimation;
        IntegratedDT = other.IntegratedDT;
        cacheSetupflag = other.cacheSetupflag;
        latestIndex = other.latestIndex;
        currentIndex = other.currentIndex;
    }
    return *this;
}

bool SlipForceCache::CacheIsSetup(){
    return cacheSetupflag;
}

void SlipForceCache::ClearFrames(){
    
    // clear
    forceFrames.Empty();
    flagged.Empty();
    timeOfAnimation = 1.0f;
    IntegratedDT = 0.0f;
    cacheSetupflag = false;
    latestIndex = -1;
    currentIndex = 0;
}

void SlipForceCache::AddFrame(FVector &frame){
    forceFrames.Add(frame);
    flagged.Add(false);
}

void SlipForceCache::SetTime(float timeIn){
    timeOfAnimation = timeIn;
}



void SlipForceCache::SetScalarDAndConvertBufferToVelocity(float D, float mass){
    /*$$
    F_{raw \ cache}(t) \cdot \frac{D\cdot t}{m}  >= v
    $$*/

    float frac = (D * timeOfAnimation) / mass;
    scale(frac);

    if (forceFrames.Num() > 0)
    {
        cacheSetupflag = true;
    }
}

void SlipForceCache::scale(float num){
    for (int i = 0; i < forceFrames.Num(); i++){
        FVector &current = forceFrames[i];
        current *= num;
    }
}

void SlipForceCache::scaleZ(float num){
    for (int i = 0; i < forceFrames.Num(); i++){
        FVector &current = forceFrames[i];
        current.Z *= num;
    }
}




bool SlipForceCache::endReached(){
    if(!debugMode){
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("Slip cache result time(%.2f / %.2f)"),
                IntegratedDT,
                timeOfAnimation
            )
        );
    }
    
    return IntegratedDT >= timeOfAnimation;
}

FVector SlipForceCache::Tick(float deltatime){
    if(endReached()){
        return FVector::ZeroVector;
    }

    IntegratedDT += deltatime;
    float scalar = FMath::Clamp(IntegratedDT / timeOfAnimation, 0.0f, 1.0f);
    int index = FMath::Clamp(int(scalar * forceFrames.Num()), 0, forceFrames.Num() - 1);

    if(index == latestIndex) return FVector::ZeroVector;

    FVector integrated(0,0,0);
    for(int i = latestIndex +1; i <= index; i++){
        if(IndexIsValid(i) && !wasTicked(i)){
            integrated += forceFrames[i];
            flagged[i] = true;
        }
    }

    latestIndex = index;
    return integrated;
}


bool SlipForceCache::wasTicked(int i){
    if(IndexIsValid(i)){
        bool flag = flagged[i]; //copy flag
        flagged[i] = true; //set true for ticked now
        return flag; //return flag, might be false or true.
    }
    return true;
}

bool SlipForceCache::IndexIsValid(int i){
    return i >= 0 && i < forceFrames.Num();
}

void SlipForceCache::Debug(FString reason, FVector &compare){
    //all of them look identical.
    float dt = timeOfAnimation / 1000;
    Debug(reason, compare, dt);

    dt = timeOfAnimation / 400;
    Debug(reason, compare, dt);

    dt = timeOfAnimation / 100;
    Debug(reason, compare, dt);

    dt = timeOfAnimation / 10;
    Debug(reason, compare, dt);

    dt = timeOfAnimation;
    Debug(reason, compare, dt);

}

void SlipForceCache::Debug(FString reason, FVector &compare, float dt){
    DebugResetFlags();
    latestIndex = -1;
    debugMode = true;
    IntegratedDT = 0.0f;
    FVector result(0,0,0);
    while (!endReached())
    {
        result += Tick(dt);
    }
    IntegratedDT = 0.0f; //reset.
    FVector resultOther(0, 0, 0);
    for (int i = 0; i < forceFrames.Num(); i++){
        resultOther += forceFrames[i];
    }

    // log
    DebugHelper::logMessage(
        FString::Printf(
            TEXT("SlipForceCache %s Result NumFrames(%d) deltaTimeStep(%.2f): result ticked(%s) resultCompare %s"),
            *reason,
            forceFrames.Num(),
            dt,
            *result.ToString(),
            *compare.ToString()
        )
    );

    DebugResetFlags();
    debugMode = false;
}

void SlipForceCache::DebugResetFlags(){
    // reset flags
    for (int i = 0; i < flagged.Num(); i++){
        flagged[i] = false;
    }
}

