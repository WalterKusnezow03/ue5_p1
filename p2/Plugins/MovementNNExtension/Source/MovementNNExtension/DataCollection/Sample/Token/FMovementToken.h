#pragma once

#include "CoreMinimal.h"
#include "ActorTracking/Tracking/TrajectoryCollection/Container/Trajectory.h"

struct MOVEMENTNNEXTENSION_API FMovementToken {

public:
    float deltaX = 0.0f;
    float deltaY = 0.0f;
    float deltaZ = 0.0f;
    float deltaT = 0.0f;

    void Generate(const Trajectory &current, const Trajectory &next);

    /** generate from buffer [x,y,z,t] where index is at 't' */
    void Generate(const TArray<float> &buffer, int lastIndex);
    void Generate(float x, float y, float z, float t);
    void Reset();

    static int NumFloats(){
        return 4;
    }

    void AppendToBuffer(TArray<uint8> &buffer) const;

    FVector GetDirection() const;

    //norman distance
    float DistanceSquared(const FMovementToken &other) const;
    

private:



};