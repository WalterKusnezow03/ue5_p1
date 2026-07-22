#include "CpuWaterVertexShaderBase.h"

void CpuWaterVertexShaderBase::applyShaderToVertex(FVector &vertex, const FVector &actorLocation){
    applyShaderToVertex(vertex, actorLocation, waveProperty);
}

void CpuWaterVertexShaderBase::applyShadersToVertex(
    FVector &vertex,
    const FVector &actorLocation,
    TArray<WaveProperty> &properties
){
    //multiple
    for (int i = 0; i < properties.Num(); i++){
        WaveProperty &currentWave = properties[i];
        applyShaderToVertex(vertex, actorLocation, currentWave);
    }
}


void CpuWaterVertexShaderBase::UpdateShaderRunningTime(float time){
    shaderRunningTime = time;
}

void CpuWaterVertexShaderBase::SetWaveLength(float lambda){
    //f = c / y
    //y = c / f
    waveProperty.SetWaveLength(lambda);
}

void CpuWaterVertexShaderBase::SetWaveSpeed(float c){
    //f = c / y
    waveProperty.SetWaveSpeed(c);
}

void CpuWaterVertexShaderBase::SetAmplitude(float a){
    waveProperty.SetAmplitude(a);
}

FVector2D CpuWaterVertexShaderBase::WorldPos2D(const FVector &vertex, const FVector &actorLocation){
    return FVector2D(vertex.X + actorLocation.X, vertex.Y + actorLocation.Y);
}