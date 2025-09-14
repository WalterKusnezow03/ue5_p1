#pragma once

#include "CoreMinimal.h"

/// @brief frames added are force, once scalar D is setup, its an velocity
class IKHUMANOIDMODELL_API SlipForceCache{

private:
    static int idGlobal;
    int id = 0;
    bool debugMode = false;


public:
    SlipForceCache();
    ~SlipForceCache();
    SlipForceCache(const SlipForceCache &other);
    SlipForceCache &operator=(const SlipForceCache &other);

    bool CacheIsSetup();

    void ClearFrames();
    void AddFrame(FVector &frame);


    
    
    void SetTime(float timeIn);

    bool endReached();

    /// @brief if Multiplied with Scalar D: Returned IMPULSE, else Force.
    /// @param deltatime 
    /// @return 
    FVector Tick(float deltatime);

    void Debug(FString reason, FVector &compare);
    void Debug(FString reason, FVector &compare, float dt);

    //new

    /// @brief converts buffer to velocity buffer which can be added up.
    /// @param D 
    /// @param mass 
    void SetScalarDAndConvertBufferToVelocity(float D, float mass);


    void scale(float num);
    void scaleZ(float num);

    

private:
    TArray<FVector> forceFrames;
    TArray<bool> flagged;

    bool cacheSetupflag = false;

    int currentIndex = 0;
    int latestIndex = -1;

    float timeOfAnimation = 0.0f;
    float IntegratedDT = 0.0f;

    ///@brief ticks time and returns frame index, may be invalid.
    bool IndexChanged(float deltatime);

    bool wasTicked(int i);
    bool IndexIsValid(int i);

    void DebugResetFlags();
};