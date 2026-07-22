#pragma once

#include "CoreMinimal.h"
#include "terrainPlugin/meshgen/water/waveShaders/WaveProperty.h"

//vertex shader base class (derived classes will change the shading function)
//for the procedural mesh actor.
//the performance is fairly stable with a not high vertex count
//and low render distance (up to 50  meters)
class TERRAINPLUGIN_API CpuWaterVertexShaderBase {

public:
    void applyShaderToVertex(FVector &vertex, const FVector &actorLocation);
    void applyShadersToVertex(
        FVector &vertex,
        const FVector &actorLocation,
        TArray<WaveProperty> &properties
    );

    // --- OVERRIDE ---
    virtual void applyShaderToVertex(
        FVector &vertex, 
        const FVector &actorLocation,
        WaveProperty &wavePropertyIn
    ){
        //to be overriden
    }
    // --- OVERRIDE ---

    void UpdateShaderRunningTime(float time);

    void SetWaveLength(float lambda);

    void SetWaveSpeed(float c);

    void SetAmplitude(float a);

protected:
    float shaderRunningTime = 0.0f;
    WaveProperty waveProperty;

    FVector2D WorldPos2D(const FVector &vertex, const FVector &actorLocation);
};